#include "layer_data.hpp"

using namespace JustDraw;
using namespace godot;

void LayerData::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_lines"), &LayerData::get_lines);
    ClassDB::bind_method(D_METHOD("set_lines", "p_lines"), &LayerData::set_lines);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "lines"), "set_lines", "get_lines");

    ClassDB::bind_method(D_METHOD("get_pens"), &LayerData::get_pens);
    ClassDB::bind_method(D_METHOD("set_pens", "p_pens"), &LayerData::set_pens);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "pens"), "set_pens", "get_pens");
}
