#include "rs_pen.hpp"

#include "godot/resources/pens/pen/pen.hpp"

using namespace JustDraw;

void RSPen::_bind_methods()
{
    #pragma region Getters and Setters

    ClassDB::bind_method(D_METHOD("set_data", "p_data"), &RSPen::set_data);
    ClassDB::bind_method(D_METHOD("get_data"), &RSPen::get_data);
    ADD_PROPERTY(PropertyInfo(Variant::VARIANT_MAX, "data"), "set_data", "get_data");

    ClassDB::bind_method(D_METHOD("set_pen", "p_pen"), &RSPen::set_pen);
    ClassDB::bind_method(D_METHOD("get_pen"), &RSPen::get_pen);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "pen", PROPERTY_HINT_RESOURCE_TYPE, "Pen"), "set_pen", "get_pen");

    ClassDB::bind_method(D_METHOD("set_rect", "p_rect"), &RSPen::set_rect);
    ClassDB::bind_method(D_METHOD("get_rect"), &RSPen::get_rect);
    ADD_PROPERTY(PropertyInfo(Variant::RECT2, "rect"), "set_rect", "get_rect");

    ClassDB::bind_method(D_METHOD("get_is_finished"), &RSPen::get_is_finished);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_finished"), "", "get_is_finished");

    #pragma endregion
}

RSPen::RSPen(const Variant &p_data, const Ref<Pen> &p_pen) :
    data(p_data), pen(p_pen), rect(pen->CalculateRect(data))
{
    auto rs = RenderingServer::get_singleton();
    if(rs == nullptr) return;
    canvas_item = rs->canvas_item_create();
}

RSPen::~RSPen()
{
    auto rs = RenderingServer::get_singleton();
    if(rs == nullptr) return;
    rs->free_rid(canvas_item);
}

void RSPen::Clear() const
{
    auto rs = RenderingServer::get_singleton();
    if(rs == nullptr) return;
    if(!canvas_item.is_valid()) return;
    rs->canvas_item_clear(canvas_item);
}

void RSPen::Update(const RID &p_parent_item, const int p_index) const
{
    if(!canvas_item.is_valid()) return;
    pen->Draw(p_parent_item, canvas_item, p_index, data);
}

void RSPen::Redraw(const RID &p_parent_item, const int p_index) const
{
    Clear();
    Update(p_parent_item, p_index);
}

void RSPen::UpdateIndex(const int p_index)
{
    auto rs = RenderingServer::get_singleton();
    if(rs == nullptr) return;
    if(!canvas_item.is_valid()) return;
    rs->canvas_item_set_draw_index(canvas_item, p_index);
}
