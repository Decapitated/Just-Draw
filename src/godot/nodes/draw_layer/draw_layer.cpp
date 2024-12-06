#include "draw_layer.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/camera2d.hpp>

#include "godot/nodes/draw_canvas/draw_canvas.hpp"

using namespace JustDraw;
using namespace godot;

void DrawLayer::_bind_methods()
{
    #pragma region Getters and Setters

    ClassDB::bind_method(D_METHOD("set_active", "p_active"), &DrawLayer::set_active);
    ClassDB::bind_method(D_METHOD("get_active"), &DrawLayer::get_active);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");

    ClassDB::bind_method(D_METHOD("get_layer_data"), &DrawLayer::get_layer_data);
    ClassDB::bind_method(D_METHOD("load_layer_data", "p_layer_data"), &DrawLayer::load_layer_data);

    #pragma endregion
}

DrawLayer::DrawLayer()
{
    set_mouse_filter(MOUSE_FILTER_IGNORE);
}

DrawLayer::~DrawLayer() {}

PackedStringArray DrawLayer::_get_configuration_warnings() const
{
    auto warnings = PackedStringArray();
    auto parent = dynamic_cast<DrawCanvas*>(get_parent_control());
    if(parent == nullptr)
    {
        warnings.push_back("DrawLayer must be a child of a DrawCanvas.");
    }
    return warnings;
}

void DrawLayer::_unhandled_input(const Ref<InputEvent> &p_event)
{
    if(active && p_event->get_device() != InputEvent::DEVICE_ID_EMULATION)
    {
        const Ref<InputEvent> event = make_input_local(*p_event);
        if(auto e = dynamic_cast<InputEventMouseButton*>(*event))
        {
            HandleMouseButton(*e);
        }
        else if(auto e = dynamic_cast<InputEventMouseMotion*>(*event))
        {
            HandleMouseMotion(*e);
        }
        else if(auto e = dynamic_cast<InputEventKey*>(*event))
        {
            HandleKey(*e);
        }
    }
}

void DrawLayer::_process(double p_delta)
{
    set_position(Vector2());
    set_size(get_parent_control()->get_size());
}

void DrawLayer::HandleMouseButton(const InputEventMouseButton &event)
{
    if(mode == NONE && event.is_pressed())
    {
        const Vector2 pen_position = event.get_position();
        if(event.get_button_index() == MOUSE_BUTTON_LEFT && !event.is_double_click())
        {
            StartDraw(pen_position);
        }
        else if(event.get_button_index() == MOUSE_BUTTON_RIGHT)
        {
            StartErase(pen_position);
        }
        queue_redraw();
    }
}

void DrawLayer::HandleMouseMotion(const InputEventMouseMotion &event)
{
    const bool is_left_button_pressed = (event.get_button_mask() & MOUSE_BUTTON_MASK_RIGHT) == MOUSE_BUTTON_MASK_RIGHT;
    const float pen_pressure = event.get_pressure() || is_left_button_pressed;
    const float is_pen_inverted = event.get_pen_inverted() || is_left_button_pressed;
    const Vector2 pen_position = event.get_position();
    if(mode == NONE)
    {
        if(pen_pressure > 0.0f)
        {
            // If eraser.
            if(is_pen_inverted)
            {
                StartErase(pen_position);
            }
            else
            {
                StartDraw(pen_position);
            }
            queue_redraw();
        }
    }
    else if(mode == ERASE)
    {
        if(is_pen_inverted && pen_pressure > 0.0f)
        {
            UpdateErase(pen_position);
            queue_redraw();
        } else { mode = NONE; }
    }
    else if(mode == DRAW)
    {
        if(!is_pen_inverted && pen_pressure > 0.0f)
        {
            UpdateDraw(pen_position);
            queue_redraw();
        } else { mode = NONE; }
    }
}

void DrawLayer::HandleKey(const InputEventKey &event) {}

void DrawLayer::_draw()
{
    auto parent = get_parent(); 
    for(CappedPenLine& line : lines)
    {
        if(line.size() >= 2)
        {
            draw_circle(line[0], line.cap_radius, line.color);
            draw_polyline(line, line.color, line.width);
            draw_circle(line[line.size() - 1], line.cap_radius, line.color);
        }
        else if(line.size() == 1)
        {
            draw_circle(line[0], line.width / 2.0f, line.color);
        }
    }
}

void DrawLayer::StartDraw(Vector2 pen_position)
{
    auto canvas = dynamic_cast<DrawCanvas*>(get_parent_control());
    if(canvas != nullptr)
    {
        mode = DRAW;
        // Create a new pen line, and set line properties.
        auto line = CappedPenLine(canvas->pen_color, canvas->line_width, (canvas->line_width / 2.0f) * canvas->cap_scale);
        // Add the mouse position as the first point of the line.
        line.append(pen_position);
        // Add the line to the list.
        lines.push_back(line);
    }
}

void DrawLayer::StartErase(Vector2 pen_position)
{
    mode = ERASE;
    UpdateErase(pen_position);
}

void DrawLayer::UpdateDraw(Vector2 pen_position)
{
    auto canvas = dynamic_cast<DrawCanvas*>(get_parent_control());
    if(canvas != nullptr)
    {
        auto cam = get_viewport()->get_camera_2d();
        const float min_dist = canvas->min_draw_distance * (1.0f / cam->get_zoom().x);
        CappedPenLine& line = lines.back();
        auto prev_pos = line[line.size() - 1];
        auto dist = prev_pos.distance_squared_to(pen_position);
        if(dist >= powf(min_dist, 2.0))
        {
            if(line.size() >= 2)
            {
                Vector2 prev = line[line.size() - 2], curr = prev_pos, next = pen_position;
                float curr_dot = (curr - prev).normalized().dot((next - curr).normalized());
                curr_dot = (1.0f - (curr_dot * 0.5f + 0.5f)) * 180.0f;
                if(curr_dot > canvas->max_draw_angle)
                {
                    // Create a new pen line, and set line properties.
                    auto new_line = CappedPenLine(line.color, line.width, line.cap_radius);
                    new_line.append(line[line.size() - 1]);
                    // Add the mouse position as the second point of the line.
                    new_line.append(next);
                    // Add the line to the list.
                    lines.push_back(new_line);
                    return;
                }
            }
            line.append(pen_position);
            // If the line has 3 or more points, smooth it.
            if(line.size() >= 3)
            {
                for(int i = 0; i < canvas->smooth_steps; i++)
                {
                    int smooth_start = line.size() - 2;
                    static_cast<Line&>(line) = canvas->SmoothLine(line, smooth_start);
                }
            }
        }
    }
}

void DrawLayer::UpdateErase(Vector2 pen_position)
{
    LineIterator line_it = lines.begin();
    int loop_count = 0;
    while(line_it != lines.end())
    {
        bool should_erase = UpdateErase(pen_position, line_it);
        if(should_erase)
        {
            line_it = lines.erase(line_it);
        }
        else
        {
            line_it++;
        }
    }
}

bool DrawLayer::UpdateErase(Vector2 pen_position, LineIterator line_it)
{
    bool sliced = false;
    auto canvas = dynamic_cast<DrawCanvas*>(get_parent_control());
    if(canvas != nullptr)
    {
        int slice_start = 0;
        for(int i = 0; i < line_it->size(); i++)
        {
            auto curr = (*line_it)[i];
            bool is_last_slice = slice_start > 0 && i == line_it->size() - 1;
            if(is_last_slice || (curr.distance_squared_to(pen_position) < powf(canvas->eraser_size, 2.0f)))
            {
                sliced = true;
                int end = is_last_slice ? i + 1 : i;
                auto new_line = CappedPenLine(line_it->slice(slice_start, end), line_it->color, line_it->width, line_it->cap_radius);
                auto new_line_it = lines.insert(line_it, new_line);
                bool should_erase = UpdateErase(pen_position, new_line_it);
                if(should_erase)
                {
                    lines.erase(new_line_it);
                }
                slice_start = i + 1;
            }
        }
    }
    return sliced || line_it->size() == 0;
}

TypedArray<PackedVector2Array> DrawLayer::GetLines()
{
    TypedArray<PackedVector2Array> lines_array;
    for(const auto &line : lines)
    {
        lines_array.append(line);
    }
    return lines_array;
}

TypedArray<Dictionary> DrawLayer::GetPens()
{
    TypedArray<Dictionary> pens_array;
    for(const auto &line : lines)
    {
        auto pen = Dictionary();
        pen["color"] = line.color;
        pen["width"] = line.width;
        pen["cap_radius"] = line.cap_radius;
        pens_array.append(pen);
    }
    return pens_array;
}

void DrawLayer::load_layer_data(Ref<LayerData> p_layer_data)
{
    Lines new_lines = Lines();
    auto p_lines = p_layer_data->get_lines();
    auto p_pens = p_layer_data->get_pens();
    for(int i = 0; i < p_lines.size(); i++)
    {
        auto color = static_cast<Color>(p_pens[i].get("color"));
        auto width = static_cast<float>(p_pens[i].get("width"));
        auto cap_radius = static_cast<float>(p_pens[i].get("cap_radius"));
        new_lines.push_back(CappedPenLine(p_lines[i], color, width, cap_radius));
    }
    lines = new_lines;
    queue_redraw();
}
