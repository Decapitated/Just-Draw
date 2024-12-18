#include "rs_pen.hpp"

using namespace JustDraw;

RSPen::RSPen(const Line &p_line, const Ref<Pen> &p_pen) :
    line(p_line), pen(p_pen), rect(CalculateRect())
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
    pen->Draw(p_parent_item, canvas_item, p_index, line);
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
    rs->canvas_item_set_z_index(canvas_item, p_index);
}

Rect2 RSPen::CalculateRect() const
{
    auto rect = Rect2();
    for(int i = 0; i < line.size(); i++)
    {
        rect.expand_to(line[i]);
    }
    return rect;
}
