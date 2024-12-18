#ifndef RESOURCE_PEN_H
#define RESOURCE_PEN_H

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

namespace JustDraw
{
    using Line = PackedVector2Array;

    class Pen : public Resource
    {
        GDCLASS(Pen, Resource);

        protected:
            static void _bind_methods();

        public:
            Pen() {}
            virtual ~Pen() {}

            GDVIRTUAL4(_draw, RID, RID, int, Line);
            virtual void _draw(const RID &p_parent_item, const RID &p_canvas_item, const int p_index, const Line &p_line) {}
            void Draw(const RID &p_parent_item, const RID &p_canvas_item, const int p_index, const Line &p_line);

            GDVIRTUAL1R(Line, _finish_draw, Line);
            virtual Line _finish_draw(const Line &p_line) { return p_line; }
            Line FinishDraw(const Line &p_line);
    };

}

#endif