#include "pen.hpp"

using namespace JustDraw;

void Pen::_bind_methods()
{
    GDVIRTUAL_BIND(_draw, "canvas_item", "line");
    ClassDB::bind_method(D_METHOD("draw", "canvas_item", "line"), &Pen::Draw);
}
void Pen::Draw(const RID &p_canvas_item, const Line &p_line)
{
    if(GDVIRTUAL_CALL(_draw, p_canvas_item, p_line))
    {
        return;
    }
    _draw(p_canvas_item, p_line);
}
