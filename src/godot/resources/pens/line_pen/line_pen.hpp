#ifndef RESOURCE_LINE_PEN_H
#define RESOURCE_LINE_PEN_H

#include <algorithm>

#include "godot/resources/pens/pen/pen.hpp"

using namespace std;
using namespace godot;

namespace JustDraw
{
    using Line = PackedVector2Array;

    class LinePen : public Pen
    {
        GDCLASS(LinePen, Pen);

        protected:
            static void _bind_methods();

        public:
            Color color = Color();
            float width = 5.0f;
            float cap_scale = 1.0f;

            int smooth_steps = 5;
            float smooth_ratio = 0.333333f;
            float smooth_min_distance = 0.001f;

            LinePen() {}
            LinePen(Color p_color, float p_width, float p_cap_scale) : color(p_color), width(p_width), cap_scale(p_cap_scale) {}
            LinePen(const LinePen &p_pen) : color(p_pen.color), width(p_pen.width), cap_scale(p_pen.cap_scale) {}

            #pragma region Getters and Setters

            Color get_color() { return color; }
            void set_color(Color p_color) { color = p_color; }
            
            float get_width() { return width; }
            void set_width(float p_width) { width = p_width; }

            float get_cap_scale() { return cap_scale; }
            void set_cap_scale(float p_cap_scale) { cap_scale = p_cap_scale; }

            int get_smooth_steps() { return smooth_steps; }
            void set_smooth_steps(int p_steps) { smooth_steps = max(p_steps, 0); }

            float get_smooth_ratio() { return smooth_ratio; }
            void set_smooth_ratio(float p_ratio) { smooth_ratio = min(max(p_ratio, 0.0f), 0.5f); }

            float get_smooth_min_distance() { return smooth_min_distance; }
            void set_smooth_min_distance(float p_distance) { smooth_min_distance = max(p_distance, 0.0f); }

            #pragma endregion

            Line SmoothLine(Line p_line);
            static Line SmoothLineStep(Line p_line, float p_smooth_ratio, float p_smooth_min_distance);

            virtual void _draw(const RID &p_parent_item, const RID &p_canvas_item, const int p_index, const Variant &p_data) override;
            
            virtual Variant _start_draw(const Vector2 &p_pen_position, const Ref<RSPen> &p_rs_pen) override;
            Variant start_draw(const Vector2 &p_pen_position, const Ref<RSPen> &p_rs_pen);

            virtual Array _update_draw(const Vector2 &p_pen_position, const float &p_cam_scale, const Ref<RSPen> &p_rs_pen);
            Array update_draw(const Vector2 &p_pen_position, const float &p_cam_scale, const Ref<RSPen> &p_rs_pen);

            virtual void _finish_draw(const Ref<RSPen> &p_rs_pen) override;
            void finish_draw(const Ref<RSPen> &p_rs_pen);

            virtual Variant _update_erase(const Vector2 &p_pen_position, const float &p_eraser_size, const Ref<RSPen> &p_rs_pen) override;
            Variant update_erase(const Vector2 &p_pen_position, const float &p_eraser_size, const Ref<RSPen> &p_rs_pen);

            virtual void _scale_data(const Vector2 &p_scale, const Ref<RSPen> &p_rs_pen) override;
            void scale_data(const Vector2 &p_scale, const Ref<RSPen> &p_rs_pen);

            virtual void _offset_data(const Vector2 &p_offset, const Ref<RSPen> &p_rs_pen) override;
            void offset_data(const Vector2 &p_offset, const Ref<RSPen> &p_rs_pen);
            
            virtual void _draw_cursor(CanvasItem* p_canvas_item, const Vector2 &p_pen_position, const float &p_eraser_size, const bool &p_is_eraser) override;
            void draw_cursor(CanvasItem* p_canvas_item, const Vector2 &p_pen_position, const float &p_eraser_size, const bool &p_is_eraser);

            virtual Ref<Pen> _duplicate_pen(bool p_subresources = false);
            Ref<Pen> duplicate_pen(bool p_subresources = false);
    };
}

#endif