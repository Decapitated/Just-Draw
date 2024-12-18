#include "pen.hpp"

using namespace JustDraw;

void Pen::_bind_methods()
{
    GDVIRTUAL_BIND(_draw, "parent_item", "canvas_item", "index", "line");
    ClassDB::bind_method(D_METHOD("draw", "parent_item", "canvas_item", "index", "line"), &Pen::Draw);
}
void Pen::Draw(const RID &p_parent_item, const RID &p_canvas_item, const int p_index, const Line &p_line)
{
    if(GDVIRTUAL_CALL(_draw, p_parent_item, p_canvas_item, p_index, p_line))
    {
        return;
    }
    _draw(p_parent_item, p_canvas_item, p_index, p_line);
}
