#include "line_pen.hpp"

using namespace JustDraw;

void LinePen::_bind_methods()
{
    #pragma region Getters and Setters

    ClassDB::bind_method(D_METHOD("get_color"), &LinePen::get_color);
    ClassDB::bind_method(D_METHOD("set_color", "p_color"), &LinePen::set_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"),
                 "set_color", "get_color");

    ClassDB::bind_method(D_METHOD("get_width"), &LinePen::get_width);
    ClassDB::bind_method(D_METHOD("set_width", "p_width"), &LinePen::set_width);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "width"),
                 "set_width", "get_width");

    ClassDB::bind_method(D_METHOD("get_cap_scale"), &LinePen::get_cap_scale);
    ClassDB::bind_method(D_METHOD("set_cap_scale", "p_cap_scale"), &LinePen::set_cap_scale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cap_scale"),
                 "set_cap_scale", "get_cap_scale");

    ClassDB::bind_method(D_METHOD("get_smooth_steps"), &LinePen::get_smooth_steps);
    ClassDB::bind_method(D_METHOD("set_smooth_steps", "p_smooth_steps"), &LinePen::set_smooth_steps);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "smooth_steps"),
                 "set_smooth_steps", "get_smooth_steps");
    
    ClassDB::bind_method(D_METHOD("get_smooth_ratio"), &LinePen::get_smooth_ratio);
    ClassDB::bind_method(D_METHOD("set_smooth_ratio", "p_smooth_ratio"), &LinePen::set_smooth_ratio);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "smooth_ratio"),
                 "set_smooth_ratio", "get_smooth_ratio");
    
    ClassDB::bind_method(D_METHOD("get_smooth_min_distance"), &LinePen::get_smooth_min_distance);
    ClassDB::bind_method(D_METHOD("set_smooth_min_distance", "p_smooth_min_distance"), &LinePen::set_smooth_min_distance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "smooth_min_distance"),
                 "set_smooth_min_distance", "get_smooth_min_distance");

    #pragma endregion

    ClassDB::bind_method(D_METHOD("start_draw", "pen_position", "rs_pen"), &LinePen::start_draw);
    ClassDB::bind_method(D_METHOD("update_draw", "pen_position", "cam_scale", "rs_pen"), &LinePen::update_draw);
    ClassDB::bind_method(D_METHOD("finish_draw", "pen_position", "rs_pen"), &LinePen::finish_draw);
    ClassDB::bind_method(D_METHOD("update_erase", "pen_position", "eraser_size", "rs_pen"), &LinePen::update_erase);
    ClassDB::bind_method(D_METHOD("scale_data", "scale", "rs_pen"), &LinePen::scale_data);
    ClassDB::bind_method(D_METHOD("offset_data", "offset", "rs_pen"), &LinePen::offset_data);
    ClassDB::bind_method(D_METHOD("draw_cursor", "canvas_item", "pen_position", "eraser_size", "is_eraser"), &LinePen::draw_cursor);

    ClassDB::bind_method(D_METHOD("smooth_line", "line"), &LinePen::SmoothLine);
    ClassDB::bind_static_method(get_class_static(), D_METHOD("smooth_line_step", "line", "smooth_ratio", "smooth_min_distance"), &LinePen::SmoothLineStep);
}

Line LinePen::SmoothLine(Line p_line)
{
    for (int i = 0; i < smooth_steps; i++)
    {
        p_line = SmoothLineStep(p_line, smooth_ratio, smooth_min_distance);
    }
    return p_line;
}

Line LinePen::SmoothLineStep(Line p_line, float p_smooth_ratio, float p_smooth_min_distance)
{
    // Clamp ratio, then minus one so it doesn't go past midpoint.
    float ratio = fmaxf(0.0, fminf(1.0, p_smooth_ratio));
    if (ratio > 0.5f) ratio = 1.0f - ratio;
    auto smoothed_line = Line();
    smoothed_line.push_back(p_line[0]); // Add first point to new line.
    for (int i = 1; i < p_line.size() - 1; i++)
    {
        Vector2 prev = p_line[i - 1], curr = p_line[i], next = p_line[i + 1];
        float dist_squared = prev.distance_squared_to(curr);
        if(dist_squared < powf(p_smooth_min_distance, 2.0f))
        {
            smoothed_line.push_back(curr);
            continue;
        }
        smoothed_line.push_back(curr.lerp(prev, ratio));
        smoothed_line.push_back(curr.lerp(next, ratio));
    }
    smoothed_line.push_back(p_line[p_line.size() - 1]); // Add last point to new line.
    return smoothed_line;
}

void LinePen::_draw(const RID &p_parent_item, const RID &p_canvas_item, const int p_index, const Variant &p_data)
{
    auto rs = RenderingServer::get_singleton();
    if(rs == nullptr || p_data.get_type() != Variant::PACKED_VECTOR2_ARRAY) return;
    Line line = p_data;
    if(line.size() == 1)
    {
        rs->canvas_item_add_circle(p_canvas_item, line[0], width / 2.0f, color);
    }
    else if(line.size() >= 2)
    {
        auto colors = PackedColorArray();
        colors.resize(line.size());
        colors.fill(color);
        rs->canvas_item_add_polyline(p_canvas_item, line, colors, width);
        rs->canvas_item_add_circle(p_canvas_item, line[0], width / 2.0f * cap_scale, color);
        rs->canvas_item_add_circle(p_canvas_item, line[line.size() - 1], width / 2.0f * cap_scale, color);
    }
}

Variant LinePen::_start_draw(const Vector2 &p_pen_position, const Ref<RSPen> &p_rs_pen)
{
    auto new_line = Line();
    new_line.append(p_pen_position);
    return new_line;
}

Variant LinePen::start_draw(const Vector2 &p_pen_position, const Ref<RSPen> &p_rs_pen)
{
    return _start_draw(p_pen_position, p_rs_pen);
}

Array LinePen::_update_draw(const Vector2 &p_pen_position, const float &p_cam_scale, const Ref<RSPen> &p_rs_pen)
{
    auto new_data = Array();
    if(p_rs_pen->data.get_type() != Variant::PACKED_VECTOR2_ARRAY) return new_data;
    Line line = p_rs_pen->data;
    auto prev_pos = line[line.size() - 1];
    auto dist = prev_pos.distance_squared_to(p_pen_position);
    const float min_dist = 10.0f * p_cam_scale; // ALERT: Update to use min_dist property.
    if(dist >= powf(min_dist, 2.0))
    {
        // Clamp the angle between the line and the pen.
        if(line.size() >= 2)
        {
            Vector2 prev = line[line.size() - 2], curr = prev_pos, next = p_pen_position;
            float curr_dot = (curr - prev).normalized().dot((next - curr).normalized());
            curr_dot = (1.0f - (curr_dot * 0.5f + 0.5f)) * 180.0f;
            if(curr_dot > 135.0f) // ALERT: Change to pen property.
            {
                auto new_line = Line();
                new_line.append(line[line.size() - 1]);
                new_line.append(next);
                new_data.append(new_line);
                return new_data;
            }
        }
        line.append(p_pen_position);
        p_rs_pen->data = line;
    }
    return new_data;
}

Array LinePen::update_draw(const Vector2 &p_pen_position, const float &p_cam_scale, const Ref<RSPen> &p_rs_pen)
{
    return _update_draw(p_pen_position, p_cam_scale, p_rs_pen);
}

bool LinePen::_finish_draw(const Vector2 &p_pen_position, const Ref<RSPen> &p_rs_pen)
{
    if(p_rs_pen->data.get_type() != Variant::PACKED_VECTOR2_ARRAY) return true;
    Line smoothed_line = p_rs_pen->data;
    if(smoothed_line.size() > 2) smoothed_line = SmoothLine(smoothed_line);
    p_rs_pen->data = smoothed_line;
    return false;
}

bool LinePen::finish_draw(const Vector2 &p_pen_position, const Ref<RSPen> &p_rs_pen)
{
    return _finish_draw(p_pen_position, p_rs_pen);
}

Variant LinePen::_update_erase(const Vector2 &p_pen_position, const float &p_eraser_size, const Ref<RSPen> &p_rs_pen)
{
    if(p_rs_pen->data.get_type() != Variant::PACKED_VECTOR2_ARRAY) return true;
    Line line = p_rs_pen->data;
    auto new_data = Array();
    bool sliced = false;
    int slice_start = 0;
    bool slicing = false;
    for(int i = 0; i < line.size(); i++)
    {
        auto curr = line[i];
        bool is_last_slice = i == line.size() - 1 && slice_start > 0;
        if(is_last_slice || (curr.distance_squared_to(p_pen_position) < powf(p_eraser_size + (width / 2.0f), 2.0f)))
        {
            if(slicing)
            {
                int end = is_last_slice ? i + 1 : i;
                auto new_line = line.slice(slice_start, end);
                new_data.append(new_line);
                slicing = false;
            }
            sliced = true;
        }
        else if(!slicing)
        {
            slice_start = i;
            slicing = true;
        }
    }
    if(new_data.size() > 0)
    {
        return new_data;
    }
    return sliced || line.size() == 0;
}

Variant LinePen::update_erase(const Vector2 &p_pen_position, const float &p_eraser_size, const Ref<RSPen> &p_rs_pen)
{
    return _update_erase(p_pen_position, p_eraser_size, p_rs_pen);
}

void LinePen::_scale_data(const Vector2 &p_scale, const Ref<RSPen> &p_rs_pen)
{
    if(p_rs_pen->data.get_type() != Variant::PACKED_VECTOR2_ARRAY) return;
    Line line = p_rs_pen->data;
    for(int i = 0; i < line.size(); i++)
    {
        line[i] *= p_scale;
    }
    p_rs_pen->data = line;
}

void LinePen::scale_data(const Vector2 &p_scale, const Ref<RSPen> &p_rs_pen)
{
    _scale_data(p_scale, p_rs_pen);
}

void LinePen::_offset_data(const Vector2 &p_offset, const Ref<RSPen> &p_rs_pen)
{
    if(p_rs_pen->data.get_type() != Variant::PACKED_VECTOR2_ARRAY) return;
    Line line = p_rs_pen->data;
    for(int i = 0; i < line.size(); i++)
    {
        line[i] += p_offset;
    }
    p_rs_pen->data = line;
}

void LinePen::offset_data(const Vector2 &p_offset, const Ref<RSPen> &p_rs_pen)
{
    _offset_data(p_offset, p_rs_pen);
}

const Color DARK_GREY = Color(0.662745f, 0.662745f, 0.662745f, 1.0f);
void LinePen::_draw_cursor(CanvasItem* p_canvas_item, const Vector2 &p_pen_position, const float &p_eraser_size, const bool &p_is_eraser)
{
    if(p_canvas_item == nullptr) return;
    float radius = (p_is_eraser) ? p_eraser_size : width / 2.0f;
    p_canvas_item->draw_circle(p_pen_position, radius, DARK_GREY, false, 1.0f);
}

void LinePen::draw_cursor(CanvasItem* p_canvas_item, const Vector2 &p_pen_position, const float &p_eraser_size, const bool &p_is_eraser)
{
    _draw_cursor(p_canvas_item, p_pen_position, p_eraser_size, p_is_eraser);
}
