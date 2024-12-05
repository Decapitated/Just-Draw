#include "layer_data.hpp"

using namespace JustDraw;
using namespace godot;

void LayerData::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_lines"), &LayerData::get_lines);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "lines"), "", "get_lines");

    ClassDB::bind_method(D_METHOD("get_pens"), &LayerData::get_pens);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "pens"), "", "get_pens");
}
