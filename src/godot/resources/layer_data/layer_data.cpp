#include "layer_data.hpp"

using namespace JustDraw;
using namespace godot;

void LayerData::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_data"), &LayerData::get_data);
    ClassDB::bind_method(D_METHOD("set_data", "p_data"), &LayerData::set_data);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "data"), "set_data", "get_data");

    ClassDB::bind_method(D_METHOD("get_pens"), &LayerData::get_pens);
    ClassDB::bind_method(D_METHOD("set_pens", "p_pens"), &LayerData::set_pens);
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "pens"), "set_pens", "get_pens");
}
