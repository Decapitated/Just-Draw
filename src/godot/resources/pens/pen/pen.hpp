#ifndef RESOURCE_PEN_H
#define RESOURCE_PEN_H

#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

#include "godot/objects/rs_pen/rs_pen.hpp"

using namespace godot;

namespace JustDraw
{
    class Pen : public Resource
    {
        GDCLASS(Pen, Resource);

        protected:
            static void _bind_methods();

        public:
            Pen() {}
            virtual ~Pen() {}

            virtual Rect2 CalculateRect(const Variant &p_data) const { return Rect2(); }

            GDVIRTUAL4(_draw, RID, RID, int, Variant);
            virtual void _draw(const RID &p_parent_item, const RID &p_canvas_item, const int p_index, const Variant &p_data) {}
            void Draw(const RID &p_parent_item, const RID &p_canvas_item, const int p_index, const Variant &p_data);

            // Return the data to be associated with this pen.
            GDVIRTUAL2R(Variant, _start_draw, Vector2, Ref<RSPen>);
            virtual Variant _start_draw(const Vector2 &p_pen_position, const Ref<RSPen> &p_rs_pen) { return Variant(); }
            Variant StartDraw(const Vector2 &p_pen_position, const Ref<RSPen> &p_rs_pen);

            // Return an array of new data/pens to add.
            GDVIRTUAL3R(Array, _update_draw, Vector2, float, Ref<RSPen>);
            virtual Array _update_draw(const Vector2 &p_pen_position, const float &p_cam_scale, const Ref<RSPen> &p_rs_pen) { return Array(); }
            Array UpdateDraw(const Vector2 &p_pen_position, const float &p_cam_scale, const Ref<RSPen> &p_rs_pen);

            // Returns whether to delete this pen/data.
            GDVIRTUAL2R(bool, _finish_draw, Vector2, Ref<RSPen>);
            virtual bool _finish_draw(const Vector2 &p_pen_position, const Ref<RSPen> &p_rs_pen) { return false; }
            bool FinishDraw(const Vector2 &p_pen_position, const Ref<RSPen> &p_rs_pen);

            // Returns either a bool or an array of new data.
            // True = Erase current pen/data. False = Do nothing. Array[Variant] = Erase current; Add new data/pens.
            GDVIRTUAL3R(Variant, _update_erase, Vector2, float, Ref<RSPen>);
            virtual Variant _update_erase(const Vector2 &p_pen_position, const float &p_eraser_size, const Ref<RSPen> &p_rs_pen) { return false; }
            Variant UpdateErase(const Vector2 &p_pen_position, const float &p_eraser_size, const Ref<RSPen> &p_rs_pen);

            GDVIRTUAL2(_scale_data, Vector2, Ref<RSPen>);
            virtual void _scale_data(const Vector2 &p_scale, const Ref<RSPen> &p_rs_pen) {}
            void ScaleData(const Vector2 &p_scale, const Ref<RSPen> &p_rs_pen);

            GDVIRTUAL2(_offset_data, Vector2, Ref<RSPen>);
            virtual void _offset_data(const Vector2 &p_offset, const Ref<RSPen> &p_rs_pen) {}
            void OffsetData(const Vector2 &p_offset, const Ref<RSPen> &p_rs_pen);

            GDVIRTUAL4(_draw_cursor, CanvasItem*, Vector2, float, bool);
            virtual void _draw_cursor(CanvasItem* p_canvas_item, const Vector2 &p_pen_position, const float &p_eraser_size, const bool &p_is_eraser) {}
            void DrawCursor(CanvasItem* p_canvas_item, const Vector2 &p_pen_position, const float &p_eraser_size, const bool &p_is_eraser);
    };

}

#endif