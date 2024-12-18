#include "draw_canvas.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace JustDraw;
using namespace godot;

const char* DrawCanvas::DATA_LOADED_SIGNAL = "data_loaded";

void DrawCanvas::_bind_methods()
{
    #pragma region Getters and Setters

    ClassDB::bind_method(D_METHOD("set_pen", "p_pen"), &DrawCanvas::set_pen);
    ClassDB::bind_method(D_METHOD("get_pen"), &DrawCanvas::get_pen);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "pen", PROPERTY_HINT_RESOURCE_TYPE, "Pen"),
                 "set_pen", "get_pen");

    ClassDB::bind_method(D_METHOD("set_eraser_size", "p_width"), &DrawCanvas::set_eraser_size);
    ClassDB::bind_method(D_METHOD("get_eraser_size"), &DrawCanvas::get_eraser_size);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "eraser_size"),
                 "set_eraser_size", "get_eraser_size");

    ClassDB::bind_method(D_METHOD("set_min_draw_distance", "p_distance"), &DrawCanvas::set_min_draw_distance);
    ClassDB::bind_method(D_METHOD("get_min_draw_distance"), &DrawCanvas::get_min_draw_distance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "minimum_draw_distance"),
                 "set_min_draw_distance", "get_min_draw_distance");

    ClassDB::bind_method(D_METHOD("set_max_draw_angle", "p_angle"), &DrawCanvas::set_max_draw_angle);
    ClassDB::bind_method(D_METHOD("get_max_draw_angle"), &DrawCanvas::get_max_draw_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "maximum_draw_angle"),
                 "set_max_draw_angle", "get_max_draw_angle");

    ClassDB::bind_method(D_METHOD("set_smooth_steps", "p_steps"), &DrawCanvas::set_smooth_steps);
    ClassDB::bind_method(D_METHOD("get_smooth_steps"), &DrawCanvas::get_smooth_steps);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "smooth_steps"),
                 "set_smooth_steps", "get_smooth_steps");

    ClassDB::bind_method(D_METHOD("set_smooth_ratio", "p_ratio"), &DrawCanvas::set_smooth_ratio);
    ClassDB::bind_method(D_METHOD("get_smooth_ratio"), &DrawCanvas::get_smooth_ratio);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "smooth_ratio"),
                 "set_smooth_ratio", "get_smooth_ratio");
    
    ClassDB::bind_method(D_METHOD("get_active_layer"), &DrawCanvas::get_active_layer);
    ClassDB::bind_method(D_METHOD("set_active_layer", "p_layer"), &DrawCanvas::set_active_layer);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "active_layer", PROPERTY_HINT_NODE_TYPE, "DrawLayer",
                              PROPERTY_USAGE_NO_EDITOR),
                 "set_active_layer", "get_active_layer");

    #pragma endregion

    ClassDB::bind_method(D_METHOD("create_canvas_data"), &DrawCanvas::create_canvas_data);
    ClassDB::bind_method(D_METHOD("load_canvas_data", "p_canvas_data"), &DrawCanvas::load_canvas_data);
    ClassDB::bind_method(D_METHOD("clear_canvas"), &DrawCanvas::clear_canvas);

    ClassDB::bind_method(D_METHOD("scale_layers", "p_scale"), &DrawCanvas::scale_layers);
    ClassDB::bind_method(D_METHOD("offset_layers", "p_offset"), &DrawCanvas::offset_layers);

    ADD_SIGNAL(MethodInfo(DATA_LOADED_SIGNAL));
}

DrawCanvas::DrawCanvas()
{
    set_mouse_filter(MOUSE_FILTER_PASS);
}

DrawCanvas::~DrawCanvas() {}

void DrawCanvas::CallOnLayers(LayerCallback callback)
{
    auto draw_layers = find_children("*", "DrawLayer", false, false);
    for(int i = 0; i < draw_layers.size(); i++)
    {
        auto draw_layer = dynamic_cast<DrawLayer*>((Object*)draw_layers[i]);
        if(draw_layer != nullptr)
        {
            bool should_break = callback(draw_layer);
            if(should_break) break;
        }
    }
}

Ref<CanvasData> JustDraw::DrawCanvas::create_canvas_data()
{
    auto layers = TypedArray<LayerData>();
    CallOnLayers([&layers](DrawLayer* draw_layer)
    {
        layers.push_back(draw_layer->create_layer_data());
        return false;
    });

    Ref<CanvasData> canvas_data = memnew(CanvasData(get_size(), layers, get_color()));
    return canvas_data;
}

void DrawCanvas::load_canvas_data(Ref<CanvasData> canvas_data)
{
    clear_canvas();

    set_color(canvas_data->get_color());
    set_custom_minimum_size(canvas_data->get_size());
    auto layers = canvas_data->get_layers();
    for(int i = 0; i < layers.size(); i++)
    {
        auto draw_layer = memnew(DrawLayer);
        if(i == 0) draw_layer->set_active(true);
        add_child(draw_layer);
        draw_layer->load_layer_data(layers[i]);
    }
    emit_signal(DATA_LOADED_SIGNAL);
}

void DrawCanvas::clear_canvas()
{
    CallOnLayers([](DrawLayer* draw_layer)
    {
        memdelete(draw_layer);
        return false;
    });
}

DrawLayer* DrawCanvas::get_active_layer()
{
    DrawLayer* active_layer = nullptr;
    CallOnLayers([&active_layer](DrawLayer* draw_layer)
    {
        if(draw_layer->get_active())
        {
            active_layer = draw_layer;
            return true;
        }
        return false;
    });
    return active_layer;
}

void DrawCanvas::set_active_layer(DrawLayer *draw_layer)
{
    DrawLayer* active_layer = get_active_layer();
    if(active_layer != nullptr)
    {
        active_layer->set_active(false);
    }
    draw_layer->set_active(true);
}

void DrawCanvas::scale_layers(Vector2 p_scale)
{
    CallOnLayers([p_scale](DrawLayer* draw_layer)
    {
        draw_layer->scale_lines(p_scale);
        return false;
    });
}

void DrawCanvas::offset_layers(Vector2 p_offset)
{
    CallOnLayers([p_offset](DrawLayer* draw_layer)
    {
        draw_layer->offset_lines(p_offset);
        return false;
    });
}
