#include "draw_layer.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace JustDraw;

void DrawLayer::_bind_methods() {}

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
        if(lines->size() > 0)
        {
            auto line = (*lines)[0];
            UtilityFunctions::print("mouse motion", line->line);
        }
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
            auto line = make_shared<PenLine>();
            line->line.append(event.get_global_position());
            lines->push_back(line);
        } 
        else
        {
            is_drawing = false;
        }
    }
}

void DrawLayer::HandleMouseMotion(const InputEventMouseMotion &event)
{
    if(is_drawing)
    {
        auto line = (*lines)[lines->size() - 1];
        line->line.append(event.get_global_position());
    }
}

void DrawLayer::HandleKey(const InputEventKey &event)
{

}

void DrawLayer::_draw()
{
    for(auto line : *lines)
    {
        draw_polyline(line->line, line->color, line->width);
    }
}