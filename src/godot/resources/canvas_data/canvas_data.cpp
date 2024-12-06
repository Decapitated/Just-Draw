#include "canvas_data.hpp"

using namespace JustDraw;
using namespace godot;

void CanvasData::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_size"), &CanvasData::get_size);
    ClassDB::bind_method(D_METHOD("set_size", "p_size"), &CanvasData::set_size);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");

    ClassDB::bind_method(D_METHOD("get_color"), &CanvasData::get_color);
    ClassDB::bind_method(D_METHOD("set_color", "p_color"), &CanvasData::set_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");

    ClassDB::bind_method(D_METHOD("get_layers"), &CanvasData::get_layers);
    ClassDB::bind_method(D_METHOD("set_layers", "p_layers"), &CanvasData::set_layers);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "layers"), "set_layers", "get_layers");
}
