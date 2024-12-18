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

            RSPen(const Line &p_line, const Ref<Pen> &p_pen);
            ~RSPen();

            void Clear() const;
            void Update(const RID &p_parent_item, const int p_index) const;
            void Redraw(const RID &p_parent_item, const int p_index) const;

            void UpdateIndex(const int p_index);

            Rect2 CalculateRect() const;
    };
}

#endif