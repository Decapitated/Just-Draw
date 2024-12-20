#ifndef RESOURCE_PEN_H
#define RESOURCE_PEN_H

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

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
            GDVIRTUAL1R(Variant, _start_draw, Vector2);
            virtual Variant _start_draw(const Vector2 &p_pen_position) { return Variant(); }
            Variant StartDraw(const Vector2 &p_pen_position);

            // Return an array of new data/pens to add.
            // Empty = do nothing. 1 = Update data. 2 = Update and add new data/pen.
            GDVIRTUAL3R(Array, _update_draw, Vector2, float, Variant);
            virtual Array _update_draw(const Vector2 &p_pen_position, const float &p_cam_scale, const Variant &p_current_data) { return Array(); }
            Array UpdateDraw(const Vector2 &p_pen_position, const float &p_cam_scale, const Variant &p_current_data);

            // Returns the new data to replace the current data.
            GDVIRTUAL1R(Variant, _finish_draw, Variant);
            virtual Variant _finish_draw(const Variant &p_current_data) { return p_current_data; }
            Variant FinishDraw(const Variant &p_current_data);

            // Returns either a bool or an array of new data.
            // True = Erase current pen/data. False = Do nothing. Array[Variant] = Erase current; Add new data/pens.
            GDVIRTUAL3R(Variant, _update_erase, Vector2, float, Variant);
            virtual Variant _update_erase(const Vector2 &p_pen_position, const float &p_eraser_size, const Variant &p_current_data) { return false; }
            Variant UpdateErase(const Vector2 &p_pen_position, const float &p_eraser_size, const Variant &p_current_data);

            GDVIRTUAL2R(Variant, _scale_data, Vector2, Variant);
            virtual Variant _scale_data(const Vector2 &p_scale, const Variant &p_current_data) { return p_current_data; }
            Variant ScaleData(const Vector2 &p_scale, const Variant &p_current_data);

            GDVIRTUAL2R(Variant, _offset_data, Vector2, Variant);
            virtual Variant _offset_data(const Vector2 &p_offset, const Variant &p_current_data) { return p_current_data; }
            Variant OffsetData(const Vector2 &p_offset, const Variant &p_current_data);
    };

}

#endif