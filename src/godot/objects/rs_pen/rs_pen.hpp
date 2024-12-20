#ifndef RS_PEN_H
#define RS_PEN_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

#include "godot/resources/pens/pen/pen.hpp"

using namespace godot;

namespace JustDraw
{
    class RSPen : public RefCounted
    {
        GDCLASS(RSPen, RefCounted);
        protected:
            RID canvas_item;
        
        protected:
            static void _bind_methods();

        public:
            Variant data;
            Ref<Pen> pen;
            Rect2 rect;

            RSPen() {}
            RSPen(const Variant &p_data, const Ref<Pen> &p_pen);
            ~RSPen();

            #pragma region Getters and Setters

            Variant get_data() const { return data; }
            void set_data(const Variant &p_data) { data = p_data; }

            Ref<Pen> get_pen() const { return pen; }
            void set_pen(const Ref<Pen> &p_pen) { pen = p_pen; }

            Rect2 get_rect() const { return rect; }
            void set_rect(const Rect2 &p_rect) { rect = p_rect; }

            #pragma endregion

            void Clear() const;
            void Update(const RID &p_parent_item, const int p_index) const;
            void Redraw(const RID &p_parent_item, const int p_index) const;

            void UpdateIndex(const int p_index);
    };
}

#endif