#ifndef RSLINE_H
#define RSLINE_H

#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>

#include <godot_cpp/classes/rendering_server.hpp>

#include <variant>

using namespace std;
using namespace godot;

namespace JustDraw
{
    using Line = PackedVector2Array;

    class PenLine : public Line
    {
        public:
            Color color = Color();
            float width = 5.0f;

            PenLine() {}
            PenLine(const Color &p_color, float p_width) : color(p_color), width(p_width) {}
            PenLine(const Line &p_line, const Color &p_color, float p_width) : Line(p_line), color(p_color), width(p_width) {}
            PenLine(const PenLine &p_line) : PenLine(p_line, p_line.color, p_line.width) {}
            virtual ~PenLine() {}
    };

    class CappedPenLine : public PenLine
    {
        public:
            float cap_radius = 0.0f;

            CappedPenLine() {};
            CappedPenLine(const Color &p_color, float p_width, float p_cap_radius) : PenLine(p_color, p_width), cap_radius(p_cap_radius) {}
            CappedPenLine(const Line &p_line, const Color &p_color, float p_width, float p_cap_radius) : PenLine(p_line, p_color, p_width), cap_radius(p_cap_radius) {}
            CappedPenLine(const PenLine &p_line, float p_cap_radius) : PenLine(p_line), cap_radius(p_cap_radius) {}
            CappedPenLine(const CappedPenLine &p_line) : CappedPenLine(p_line, p_line.color, p_line.width, p_line.cap_radius) {}
            virtual ~CappedPenLine() {}
    };

    class RSLine
    {
        protected:
            RID canvas_item = RID();

        public:
            CappedPenLine line;
            
            Rect2 rect;

            RSLine(const CappedPenLine &p_line, RID p_canvas_item) :
                line(p_line), canvas_item(p_canvas_item), rect(CalculateRect(line)) {};
            ~RSLine();

            void Clear(RenderingServer* rs) const;

            void Update(RenderingServer* rs) const;
            virtual void BeforeUpdate(RenderingServer* rs) const {};
            virtual void AfterUpdate(RenderingServer* rs) const {};

            void Redraw(RenderingServer* rs) const;

            static Rect2 CalculateRect(const Line &p_line);
    };

}

#endif