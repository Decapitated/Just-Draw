#include "rs_pen.hpp"

using namespace JustDraw;

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

void RSPen::Update() const
{
    if(!canvas_item.is_valid()) return;
    pen->Draw(canvas_item, line);
}

void RSPen::Redraw() const
{
    Clear();
    Update();
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
