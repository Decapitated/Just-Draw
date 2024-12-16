#include "rs_line.hpp"

using namespace JustDraw;

RSLine::~RSLine()
{
    auto rs = RenderingServer::get_singleton();
    if(rs == nullptr) return;
    rs->free_rid(canvas_item);
}

void RSLine::Clear(RenderingServer *rs) const
{
    if(rs == nullptr || !canvas_item.is_valid()) return;
    rs->canvas_item_clear(canvas_item);
}

void RSLine::Update(RenderingServer* rs) const
{
    if(rs == nullptr || !canvas_item.is_valid()) return;
    if(line.size() >= 2)
    {
        BeforeUpdate(rs);
        rs->canvas_item_add_circle(canvas_item, line[0], line.cap_radius, line.color);
        auto colors = PackedColorArray();
        colors.resize(line.size());
        colors.fill(line.color);
        rs->canvas_item_add_polyline(canvas_item, line, colors, line.width);
        rs->canvas_item_add_circle(canvas_item, line[line.size() - 1], line.cap_radius, line.color);
        AfterUpdate(rs);
    }
    else if(line.size() == 1)
    {
        rs->canvas_item_add_circle(canvas_item, line[0], line.width / 2.0f, line.color);
    }
}

void RSLine::Redraw(RenderingServer* rs) const
{
    Clear(rs);
    Update(rs);
}

Rect2 RSLine::CalculateRect(const Line &p_line)
{
    auto rect = Rect2();
    for(int i = 0; i < p_line.size(); i++)
    {
        rect.expand_to(p_line[i]);
    }
    return rect;
}
