#include "pen.hpp"

using namespace JustDraw;

void Pen::_bind_methods()
{
    GDVIRTUAL_BIND(_draw, "parent_item", "canvas_item", "index", "data");
    ClassDB::bind_method(D_METHOD("draw", "parent_item", "canvas_item", "index", "data"), &Pen::Draw);

    GDVIRTUAL_BIND(_start_draw, "pen_position");
    GDVIRTUAL_BIND(_update_draw, "pen_position", "current_data");
    GDVIRTUAL_BIND(_finish_draw, "current_data");
    GDVIRTUAL_BIND(_update_erase, "pen_position", "current_data");
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

Variant Pen::StartDraw(const Vector2 &p_pen_position)
{
    Variant r_data;
    if(GDVIRTUAL_CALL(_start_draw, p_pen_position, r_data))
    {
        return r_data;
    }
    return _start_draw(p_pen_position);
}

Array Pen::UpdateDraw(const Vector2 &p_pen_position, const float &p_cam_scale, const Variant &p_current_data)
{
    Array r_new_data;
    if(GDVIRTUAL_CALL(_update_draw, p_pen_position, p_cam_scale, p_current_data, r_new_data))
    {
        return r_new_data;
    }
    return _update_draw(p_pen_position, p_cam_scale, p_current_data);
}

Variant Pen::FinishDraw(const Variant &p_current_data)
{
    Variant r_data;
    if(GDVIRTUAL_CALL(_finish_draw, p_current_data, r_data))
    {
        return r_data;
    }
    return _finish_draw(p_current_data);
}

Variant Pen::UpdateErase(const Vector2 &p_pen_position, const float &p_eraser_size, const Variant &p_current_data)
{
    Variant r_return;
    if(GDVIRTUAL_CALL(_update_erase, p_pen_position, p_eraser_size, p_current_data, r_return))
    {
        return r_return;
    }
    return _update_erase(p_pen_position, p_eraser_size, p_current_data);
}

Variant Pen::ScaleData(const Vector2 &p_scale, const Variant &p_current_data)
{
    Variant r_data;
    if(GDVIRTUAL_CALL(_scale_data, p_scale, p_current_data, r_data))
    {
        return r_data;
    }
    return _scale_data(p_scale, p_current_data);
}

Variant Pen::OffsetData(const Vector2 &p_offset, const Variant &p_current_data)
{
    Variant r_data;
    if(GDVIRTUAL_CALL(_offset_data, p_offset, p_current_data, r_data))
    {
        return r_data;
    }
    return _offset_data(p_offset, p_current_data);
}
