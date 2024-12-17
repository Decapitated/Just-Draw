#ifndef RESOURCE_LINE_PEN_H
#define RESOURCE_LINE_PEN_H

#include "godot/resources/pens/pen/pen.hpp"

using namespace godot;

namespace JustDraw
{
    class LinePen : public Pen
    {
        GDCLASS(LinePen, Pen);

        protected:
            static void _bind_methods();

        public:
            Color color = Color();
            float width = 5.0f;
            float cap_scale = 1.0f;

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

            #pragma endregion

            virtual void _draw(const RID &p_canvas_item, const Line &p_line) override;
    };
}

#endif