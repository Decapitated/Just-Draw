#include "pen.hpp"

using namespace JustDraw;

void Pen::_bind_methods()
{
    GDVIRTUAL_BIND(_draw, "parent_item", "canvas_item", "index", "line");
    ClassDB::bind_method(D_METHOD("draw", "parent_item", "canvas_item", "index", "line"), &Pen::Draw);

    GDVIRTUAL_BIND(_finish_draw, "line");
    ClassDB::bind_method(D_METHOD("finish_draw", "line"), &Pen::FinishDraw);
}

void Pen::Draw(const RID &p_parent_item, const RID &p_canvas_item, const int p_index, const Line &p_line)
{
    auto rs = RenderingServer::get_singleton();
    if(rs == nullptr) return;
    rs->canvas_item_set_parent(p_canvas_item, p_parent_item);
    rs->canvas_item_set_draw_index(p_canvas_item, p_index);
    if(GDVIRTUAL_CALL(_draw, p_parent_item, p_canvas_item, p_index, p_line))
    {
        return;
    }
    _draw(p_parent_item, p_canvas_item, p_index, p_line);
}

Line Pen::FinishDraw(const Line &p_line)
{
    Line r_line;
    if(GDVIRTUAL_CALL(_finish_draw, p_line, r_line))
    {
        return r_line;
    }
    return _finish_draw(p_line);
}
