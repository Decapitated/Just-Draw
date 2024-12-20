#include "rs_pen.hpp"

using namespace JustDraw;

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
