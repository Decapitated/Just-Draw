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
        if(event.is_pressed())
        {
            is_drawing = true;
            // Create a new pen line, and set line properties.
            auto line = make_shared<CappedPenLine>();
            line->cap_radius = (line_width / 2.0f) * cap_scale;
            line->width = line_width;
            // Add the mouse position as the first point of the line.
            line->append(get_local_mouse_position());
            // Add the line to the list.
            lines->push_back(line);
            queue_redraw();
        } 
        else
        {
            is_drawing = false;
            auto line = lines->back();
            for(int i = 0; i < 3; i++)
            {
                SmoothLine(*line, 0.333333f, 0.1f);
            }
            queue_redraw();
        }
    }
}

void DrawLayer::HandleMouseMotion(const InputEventMouseMotion &event)
{
    auto cam = get_viewport()->get_camera_2d();
    const float min_dist = 5.0f * (1.0f / cam->get_zoom().x);
    if(is_drawing)
    {
        auto line = lines->back();
        auto current_pos = get_local_mouse_position();
        auto prev_pos = (*line)[line->size() - 1];
        auto dist = prev_pos.distance_squared_to(current_pos);
        if(dist >= powf(min_dist, 2.0))
        {
            line->append(get_local_mouse_position());
            queue_redraw();
        }
    }
}

void DrawLayer::HandleKey(const InputEventKey &event)
{

}

void DrawLayer::_draw()
{
    for(auto line : *lines)
    {
        if(line->size() >= 2)
        {
            draw_circle((*line)[0], line->cap_radius, line->color);
            draw_polyline(*line, line->color, line->width);
            draw_circle((*line)[line->size() - 1], line->cap_radius, line->color);
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
