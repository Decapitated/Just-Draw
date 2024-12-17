#ifndef RS_PEN_H
#define RS_PEN_H

#include <godot_cpp/classes/rendering_server.hpp>

#include "godot/resources/pens/pen/pen.hpp"

using namespace godot;

namespace JustDraw
{
    class RSPen
    {
        protected:
            RID canvas_item;

        public:
            Line line;
            Ref<Pen> pen;
            Rect2 rect;

            RSPen(const Line &p_line, const Ref<Pen> &p_pen, const RID &p_canvas_item) :
                line(p_line), pen(p_pen), canvas_item(p_canvas_item), rect(CalculateRect()) {};
            ~RSPen();

            void Clear() const;
            void Update() const;
            void Redraw() const;

            Rect2 CalculateRect() const;
    };
}

#endif