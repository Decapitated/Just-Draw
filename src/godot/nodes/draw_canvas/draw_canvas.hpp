#ifndef DRAWCANVAS_H
#define DRAWCANVAS_H

#include <godot_cpp/classes/color_rect.hpp>

#include "godot/nodes/draw_layer/draw_layer.hpp"
#include "godot/resources/canvas_data/canvas_data.hpp"
#include "godot/resources/pens/line_pen/line_pen.hpp"

#include <functional>

using namespace std;
using namespace godot;

namespace JustDraw
{
    class DrawCanvas : public ColorRect
    {
        GDCLASS(DrawCanvas, ColorRect);
        private:
            friend class DrawLayer;

            static const char* DATA_LOADED_SIGNAL;

            Ref<Pen> pen = memnew(LinePen()); // The pen to draw with.

            float eraser_size = 10.0f;
            float min_draw_distance = 5.0f;
            float max_draw_angle = 135.0f;

        protected:
            static void _bind_methods();
        public:
            DrawCanvas();
            ~DrawCanvas();

            #pragma region Getters and Setters

            Ref<Pen> get_pen() { return pen; }
            void set_pen(Ref<Pen> p_pen)
            {
                if(p_pen.is_valid())
                {
                    pen = p_pen;
                }
            }

            float get_eraser_size() { return eraser_size; }
            void set_eraser_size(float p_size) { eraser_size = max(p_size, 0.0f); }

            float get_min_draw_distance() { return min_draw_distance; }
            void set_min_draw_distance(float p_distance) { min_draw_distance = max(p_distance, 0.0f); }

            float get_max_draw_angle() { return max_draw_angle; }
            void set_max_draw_angle(float p_angle) { max_draw_angle = min(max(p_angle, 0.0f), 180.0f); }

            Ref<CanvasData> create_canvas_data();
            void load_canvas_data(Ref<CanvasData> canvas_data);

            DrawLayer* get_active_layer();
            void set_active_layer(DrawLayer* draw_layer);

            #pragma endregion

            using ShouldBreak = bool;
            using LayerCallback = function<ShouldBreak(DrawLayer*)>;
            void CallOnLayers(LayerCallback callback);

            void clear_canvas();
            void scale_layers(Vector2 p_scale);
            void offset_layers(Vector2 p_offset);
    };
}

#endif