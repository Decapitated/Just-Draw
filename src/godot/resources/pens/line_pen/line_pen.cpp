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

void LinePen::_draw(const RID &p_parent_item, const RID &p_canvas_item, const int p_index, const Line &p_line)
{
    auto rs = RenderingServer::get_singleton();
    if(rs == nullptr) return;
    if(p_line.size() == 1)
    {
        rs->canvas_item_add_circle(p_canvas_item, p_line[0], width / 2.0f, color);
    }
    else if(p_line.size() >= 2)
    {
        auto colors = PackedColorArray();
        colors.resize(p_line.size());
        colors.fill(color);
        rs->canvas_item_add_polyline(p_canvas_item, p_line, colors, width);
        rs->canvas_item_add_circle(p_canvas_item, p_line[0], width / 2.0f * cap_scale, color);
        rs->canvas_item_add_circle(p_canvas_item, p_line[p_line.size() - 1], width / 2.0f * cap_scale, color);
    }
}

Line LinePen::_finish_draw(const Line &p_line)
{
    auto smoothed_line = p_line;
    if(p_line.size() > 2) smoothed_line = SmoothLine(smoothed_line);
    return smoothed_line;
}
