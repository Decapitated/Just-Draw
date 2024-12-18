#include "line_pen.hpp"

using namespace JustDraw;

void LinePen::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_color"), &LinePen::get_color);
    ClassDB::bind_method(D_METHOD("set_color", "p_color"), &LinePen::set_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "color"), "set_color", "get_color");

    ClassDB::bind_method(D_METHOD("get_width"), &LinePen::get_width);
    ClassDB::bind_method(D_METHOD("set_width", "p_width"), &LinePen::set_width);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "width"), "set_width", "get_width");

    ClassDB::bind_method(D_METHOD("get_cap_scale"), &LinePen::get_cap_scale);
    ClassDB::bind_method(D_METHOD("set_cap_scale", "p_cap_scale"), &LinePen::set_cap_scale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cap_scale"), "set_cap_scale", "get_cap_scale");
}

void LinePen::_draw(const RID &p_parent_item, const RID &p_canvas_item, const int p_index, const Line &p_line)
{
    auto rs = RenderingServer::get_singleton();
    if(rs == nullptr) return;
    rs->canvas_item_set_parent(p_canvas_item, p_parent_item);
    rs->canvas_item_set_z_as_relative_to_parent(p_canvas_item, true);
    rs->canvas_item_set_z_index(p_canvas_item, p_index);
    if(p_line.size() == 1)
    {
        rs->canvas_item_add_circle(p_canvas_item, p_line[0], width / 2.0f, color);
    }
    else if(p_line.size() >= 2)
    {
        auto colors = PackedColorArray();
        colors.resize(p_line.size());
        colors.fill(color);
        rs->canvas_item_add_polyline(p_canvas_item, p_line, colors, width);
        rs->canvas_item_add_circle(p_canvas_item, p_line[0], width / 2.0f * cap_scale, color);
        rs->canvas_item_add_circle(p_canvas_item, p_line[p_line.size() - 1], width / 2.0f * cap_scale, color);
    }
}
