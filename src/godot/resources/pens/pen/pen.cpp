#include "pen.hpp"

using namespace JustDraw;

void Pen::_bind_methods()
{
    GDVIRTUAL_BIND(_draw, "parent_item", "canvas_item", "index", "data");
    ClassDB::bind_method(D_METHOD("draw", "parent_item", "canvas_item", "index", "data"), &Pen::Draw);

    GDVIRTUAL_BIND(_start_draw, "pen_position", "rs_pen");
    GDVIRTUAL_BIND(_update_draw, "pen_position", "cam_scale", "rs_pen");
    GDVIRTUAL_BIND(_finish_draw, "pen_position", "rs_pen");
    GDVIRTUAL_BIND(_update_erase, "pen_position", "eraser_size", "rs_pen");
    GDVIRTUAL_BIND(_scale_data, "scale", "rs_pen");
    GDVIRTUAL_BIND(_offset_data, "offset", "rs_pen");
}

void Pen::Draw(const RID &p_parent_item, const RID &p_canvas_item, const int p_index, const Variant &p_data)
{
    auto rs = RenderingServer::get_singleton();
    if(rs == nullptr) return;
    rs->canvas_item_set_parent(p_canvas_item, p_parent_item);
    rs->canvas_item_set_draw_index(p_canvas_item, p_index);
    if(GDVIRTUAL_CALL(_draw, p_parent_item, p_canvas_item, p_index, p_data))
    {
        return;
    }
    _draw(p_parent_item, p_canvas_item, p_index, p_data);
}

Variant Pen::StartDraw(const Vector2 &p_pen_position, const Ref<RSPen> &p_rs_pen)
{
    Variant r_data;
    if(GDVIRTUAL_CALL(_start_draw, p_pen_position, p_rs_pen, r_data))
    {
        return r_data;
    }
    return _start_draw(p_pen_position, p_rs_pen);
}

Array Pen::UpdateDraw(const Vector2 &p_pen_position, const float &p_cam_scale, const Ref<RSPen> &p_rs_pen)
{
    Array r_new_data;
    if(GDVIRTUAL_CALL(_update_draw, p_pen_position, p_cam_scale, p_rs_pen, r_new_data))
    {
        return r_new_data;
    }
    return _update_draw(p_pen_position, p_cam_scale, p_rs_pen);
}

bool Pen::FinishDraw(const Vector2 &p_pen_position, const Ref<RSPen> &p_rs_pen)
{
    bool r_delete;
    if(GDVIRTUAL_CALL(_finish_draw, p_pen_position, p_rs_pen, r_delete))
    {
        return r_delete;
    }
    return _finish_draw(p_pen_position, p_rs_pen);
}

Variant Pen::UpdateErase(const Vector2 &p_pen_position, const float &p_eraser_size, const Ref<RSPen> &p_rs_pen)
{
    Variant r_return;
    if(GDVIRTUAL_CALL(_update_erase, p_pen_position, p_eraser_size, p_rs_pen, r_return))
    {
        return r_return;
    }
    return _update_erase(p_pen_position, p_eraser_size, p_rs_pen);
}

void Pen::ScaleData(const Vector2 &p_scale, const Ref<RSPen> &p_rs_pen)
{
    if(!GDVIRTUAL_CALL(_scale_data, p_scale, p_rs_pen))
    {
        _scale_data(p_scale, p_rs_pen);
    }
}

void Pen::OffsetData(const Vector2 &p_offset, const Ref<RSPen> &p_rs_pen)
{
    if(!GDVIRTUAL_CALL(_offset_data, p_offset, p_rs_pen))
    {
        _offset_data(p_offset, p_rs_pen);
    }
}
