#include "draw_layer.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/camera2d.hpp>

using namespace JustDraw;
using namespace godot;

void SmoothLine(Line &line, float ratio, float min_dist);

void DrawLayer::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_line_width", "p_width"), &DrawLayer::set_line_width);
    ClassDB::bind_method(D_METHOD("get_line_width"), &DrawLayer::get_line_width);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "line_width"), "set_line_width", "get_line_width");

	ClassDB::bind_method(D_METHOD("set_cap_scale", "p_width"), &DrawLayer::set_cap_scale);
    ClassDB::bind_method(D_METHOD("get_cap_scale"), &DrawLayer::get_cap_scale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cap_scale"), "set_cap_scale", "get_cap_scale");
}

DrawLayer::DrawLayer() {}

DrawLayer::~DrawLayer() {}

void DrawLayer::_unhandled_input(const Ref<InputEvent> &p_event)
{
    if(auto e = dynamic_cast<InputEventMouseButton*>(*p_event))
    {
        HandleMouseButton(*e);
    }
    else if(auto e = dynamic_cast<InputEventMouseMotion*>(*p_event))
    {
        HandleMouseMotion(*e);
    }
    else if(auto e = dynamic_cast<InputEventKey*>(*p_event))
    {
        HandleKey(*e);
    }
}

void DrawLayer::HandleMouseButton(const InputEventMouseButton &event)
{
    if(event.get_button_index() == MouseButton::MOUSE_BUTTON_LEFT)
    {
        if(mode == NONE && event.is_pressed())
        {
            mode = DRAW;
            // Create a new pen line, and set line properties.
            auto line = CappedPenLine();
            line.cap_radius = (line_width / 2.0f) * cap_scale;
            line.width = line_width;
            // Add the mouse position as the first point of the line.
            line.append(get_local_mouse_position());
            // Add the line to the list.
            lines.push_back(line);
            queue_redraw();
        } 
        else if(mode == DRAW && !event.is_pressed())
        {
            mode = NONE;
            CappedPenLine& line = lines.back();
            for(int i = 0; i < 3; i++)
            {
                SmoothLine(line, 0.333333f, 0.1f);
            }
            queue_redraw();
        }
    }
    else if(event.get_button_index() == MouseButton::MOUSE_BUTTON_RIGHT)
    {
        if(mode == NONE && event.is_pressed())
        {
            mode = ERASE;
            Erase(get_local_mouse_position());
            queue_redraw();
        } 
        else if(mode == ERASE && !event.is_pressed())
        {
            mode = NONE;
        }
    }
}

void DrawLayer::HandleMouseMotion(const InputEventMouseMotion &event)
{
    auto cam = get_viewport()->get_camera_2d();
    const float min_dist = 5.0f * (1.0f / cam->get_zoom().x);
    if(mode == DRAW)
    {
        CappedPenLine& line = lines.back();
        auto current_pos = get_local_mouse_position();
        auto prev_pos = line[line.size() - 1];
        auto dist = prev_pos.distance_squared_to(current_pos);
        if(dist >= powf(min_dist, 2.0))
        {
            line.append(get_local_mouse_position());
            queue_redraw();
        }
    }
    else if(mode == ERASE)
    {
        Erase(get_local_mouse_position());
        queue_redraw();
    }
}

void DrawLayer::HandleKey(const InputEventKey &event)
{

}

void DrawLayer::_draw()
{
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

void DrawLayer::Erase(Vector2 pos)
{
    list<CappedPenLine>::iterator line_it = lines.begin();
    int loop_count = 0;
    while(line_it != lines.end())
    {
        bool sliced = false;
        int slice_start = 0;
        for(int i = 0; i < line_it->size(); i++)
        {
            auto curr = (*line_it)[i];
            bool is_last_slice = slice_start > 0 && i == line_it->size() - 1;
            if(is_last_slice || (curr.distance_squared_to(pos) < powf(10.0f, 2.0f)))
            {
                sliced = true;
                int end = is_last_slice ? i + 1 : i;
                auto new_line = CappedPenLine(line_it->slice(slice_start, end));
                new_line.cap_radius = line_it->cap_radius;
                new_line.width = line_it->width;
                new_line.color = line_it->color;
                lines.insert(line_it, new_line);
                slice_start = i + 1;
            }
        }
        if(sliced)
        {
            line_it = lines.erase(line_it);
        }
        else
        {
            line_it++;
        }
    }
}

void SmoothLine(Line &line, float ratio, float min_dist)
{
    // Clamp ratio, then minus one so it doesn't go past midpoint.
    ratio = fmaxf(0.0, fminf(1.0, ratio));
    if (ratio > 0.5f) ratio = 1.0f - ratio;
    auto smoothed_line = std::make_shared<Line>();
    smoothed_line->push_back(line[0]); // Add first point to new line.
    for (int i = 1; i < line.size() - 1; i++)
    {
        Vector2 prev = line[i - 1], curr = line[i], next = line[i + 1];
        if (prev.distance_squared_to(next) < powf(min_dist, 2.0f))
        {
            smoothed_line->push_back(curr);
            continue;
        }
        smoothed_line->push_back(curr.lerp(prev, ratio));
        smoothed_line->push_back(curr.lerp(next, ratio));
    }
    smoothed_line->push_back(line[line.size() - 1]); // Add last point to new line.
    line = *smoothed_line;
}
