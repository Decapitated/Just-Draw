#include "draw_canvas.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace JustDraw;
using namespace godot;

void DrawCanvas::_bind_methods()
{
    #pragma region Getters and Setters

    ClassDB::bind_method(D_METHOD("set_line_color", "p_color"), &DrawCanvas::set_line_color);
    ClassDB::bind_method(D_METHOD("get_line_color"), &DrawCanvas::get_line_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "line_color"), "set_line_color", "get_line_color");

	ClassDB::bind_method(D_METHOD("set_line_width", "p_width"), &DrawCanvas::set_line_width);
    ClassDB::bind_method(D_METHOD("get_line_width"), &DrawCanvas::get_line_width);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "line_width"), "set_line_width", "get_line_width");

	ClassDB::bind_method(D_METHOD("set_cap_scale", "p_width"), &DrawCanvas::set_cap_scale);
    ClassDB::bind_method(D_METHOD("get_cap_scale"), &DrawCanvas::get_cap_scale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cap_scale"), "set_cap_scale", "get_cap_scale");

    ClassDB::bind_method(D_METHOD("set_eraser_size", "p_width"), &DrawCanvas::set_eraser_size);
    ClassDB::bind_method(D_METHOD("get_eraser_size"), &DrawCanvas::get_eraser_size);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "eraser_size"), "set_eraser_size", "get_eraser_size");

    ClassDB::bind_method(D_METHOD("set_min_draw_distance", "p_distance"), &DrawCanvas::set_min_draw_distance);
    ClassDB::bind_method(D_METHOD("get_min_draw_distance"), &DrawCanvas::get_min_draw_distance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "minimum_draw_distance"), "set_min_draw_distance", "get_min_draw_distance");

    ClassDB::bind_method(D_METHOD("set_max_draw_angle", "p_angle"), &DrawCanvas::set_max_draw_angle);
    ClassDB::bind_method(D_METHOD("get_max_draw_angle"), &DrawCanvas::get_max_draw_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "maximum_draw_angle"), "set_max_draw_angle", "get_max_draw_angle");

    ClassDB::bind_method(D_METHOD("set_smooth_steps", "p_steps"), &DrawCanvas::set_smooth_steps);
    ClassDB::bind_method(D_METHOD("get_smooth_steps"), &DrawCanvas::get_smooth_steps);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "smooth_steps"), "set_smooth_steps", "get_smooth_steps");

    ClassDB::bind_method(D_METHOD("set_smooth_ratio", "p_ratio"), &DrawCanvas::set_smooth_ratio);
    ClassDB::bind_method(D_METHOD("get_smooth_ratio"), &DrawCanvas::get_smooth_ratio);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "smooth_ratio"), "set_smooth_ratio", "get_smooth_ratio");
    
    ClassDB::bind_method(D_METHOD("create_canvas_data"), &DrawCanvas::create_canvas_data);
    ClassDB::bind_method(D_METHOD("load_canvas_data", "p_canvas_data"), &DrawCanvas::load_canvas_data);
    ClassDB::bind_method(D_METHOD("clear_canvas"), &DrawCanvas::clear_canvas);
    ClassDB::bind_method(D_METHOD("get_active_layer"), &DrawCanvas::get_active_layer);

    ClassDB::bind_method(D_METHOD("scale_layers", "p_scale"), &DrawCanvas::scale_layers);
    ClassDB::bind_method(D_METHOD("offset_layers", "p_offset"), &DrawCanvas::offset_layers);

    #pragma endregion
}

DrawCanvas::DrawCanvas()
{
    set_mouse_filter(MOUSE_FILTER_PASS);
}

DrawCanvas::~DrawCanvas() {}

Ref<CanvasData> JustDraw::DrawCanvas::create_canvas_data() const
{
    auto layers = TypedArray<LayerData>();
    // Get layer data from children DrawLayers.
    auto draw_layers = find_children("*", "DrawLayer", false, false);
    for(int i = 0; i < draw_layers.size(); i++)
    {
        auto draw_layer = dynamic_cast<DrawLayer*>((Object*)draw_layers[i]);
        if(draw_layer != nullptr)
        {
            layers.push_back(draw_layer->get_layer_data());
        }
    }

    Ref<CanvasData> canvas_data = memnew(CanvasData);
    canvas_data->set_size(get_size());
    canvas_data->set_layers(layers);
    return canvas_data;
}

void DrawCanvas::load_canvas_data(Ref<CanvasData> canvas_data)
{
    clear_canvas();

    set_custom_minimum_size(canvas_data->get_size());
    auto layers = canvas_data->get_layers();
    for(int i = 0; i < layers.size(); i++)
    {
        auto draw_layer = memnew(DrawLayer);
        if(i == 0) draw_layer->set_active(true);
        add_child(draw_layer);
        draw_layer->load_layer_data(layers[i]);
    }
}

void DrawCanvas::clear_canvas()
{
    auto draw_layers = find_children("*", "DrawLayer", false, false);
    for(int i = 0; i < draw_layers.size(); i++)
    {
        auto draw_layer = dynamic_cast<DrawLayer*>((Object*)draw_layers[i]);
        if(draw_layer != nullptr)
        {
            draw_layer->queue_free();
        }
    }
}

DrawLayer* DrawCanvas::get_active_layer() const
{
    DrawLayer* active_layer = nullptr;
    auto draw_layers = find_children("*", "DrawLayer", false, false);
    for(int i = 0; i < draw_layers.size(); i++)
    {
        auto draw_layer = dynamic_cast<DrawLayer*>((Object*)draw_layers[i]);
        if(draw_layer != nullptr)
        {
            if(draw_layer->get_active())
            {
                active_layer = draw_layer;
                break;
            }
        }
    }
    return active_layer;
}

void DrawCanvas::scale_layers(Vector2 scale)
{
    auto draw_layers = find_children("*", "DrawLayer", false, false);
    for(int i = 0; i < draw_layers.size(); i++)
    {
        auto draw_layer = dynamic_cast<DrawLayer*>((Object*)draw_layers[i]);
        if(draw_layer != nullptr)
        {
            draw_layer->scale_lines(scale);
        }
    }
}

void DrawCanvas::offset_layers(Vector2 offset)
{
    auto draw_layers = find_children("*", "DrawLayer", false, false);
    for(int i = 0; i < draw_layers.size(); i++)
    {
        auto draw_layer = dynamic_cast<DrawLayer*>((Object*)draw_layers[i]);
        if(draw_layer != nullptr)
        {
            draw_layer->offset_lines(offset);
        }
    }
}
