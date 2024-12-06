#ifndef DRAWCANVAS_H
#define DRAWCANVAS_H

#include <godot_cpp/classes/color_rect.hpp>

#include "godot/nodes/draw_layer/draw_layer.hpp"

using namespace std;
using namespace godot;

namespace JustDraw
{
    class DrawCanvas : public ColorRect
    {
        GDCLASS(DrawCanvas, ColorRect);
        private:
            friend class DrawLayer;

            Color pen_color = Color();
            
            float line_width = 5.0f;
            float cap_scale = 1.0f;

            float eraser_size = 10.0f;
            float min_draw_distance = 10.0f;
            float max_draw_angle = 135.0f;

            int smooth_steps = 1;
            float smooth_ratio = 0.333333f;
            float smooth_min_distance = 0.1f;

        protected:
            static void _bind_methods();
        public:
            DrawCanvas();
            ~DrawCanvas();

            #pragma region Getters and Setters

            Color get_color() { return pen_color; }
            void set_color(Color p_color) { pen_color = p_color; }

            float get_line_width() { return line_width; }
            void set_line_width(float p_width) { line_width = p_width; }

            float get_cap_scale() { return cap_scale; }
            void set_cap_scale(float p_scale) { cap_scale = max(p_scale, 0.0f); }

            float get_eraser_size() { return eraser_size; }
            void set_eraser_size(float p_size) { eraser_size = max(p_size, 0.0f); }

            float get_min_draw_distance() { return min_draw_distance; }
            void set_min_draw_distance(float p_distance) { min_draw_distance = max(p_distance, 0.0f); }

            float get_max_draw_angle() { return max_draw_angle; }
            void set_max_draw_angle(float p_angle) { max_draw_angle = min(max(p_angle, 0.0f), 180.0f); }

            int get_smooth_steps() { return smooth_steps; }
            void set_smooth_steps(int p_steps) { smooth_steps = max(p_steps, 0); }

            float get_smooth_ratio() { return smooth_ratio; }
            void set_smooth_ratio(float p_ratio) { smooth_ratio = min(max(p_ratio, 0.0f), 1.0f); }

            float get_smooth_min_distance() { return smooth_min_distance; }
            void set_smooth_min_distance(float p_distance) { smooth_min_distance = max(p_distance, 0.0f); }

            #pragma endregion

            Line SmoothLineStep(Line line, int smooth_start = 0);
            Line SmoothLine(Line line, int smooth_start = 0);
    };
}

#endif