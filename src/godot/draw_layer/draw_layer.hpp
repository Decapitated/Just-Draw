#ifndef DRAWLAYER_H
#define DRAWLAYER_H

#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

#include <list>

using namespace std;
using namespace godot;

namespace JustDraw
{

typedef PackedVector2Array Line;
class PenLine : public Line {
    public:
        Color color = Color();
        float width = 5.0f;

        PenLine() {}
        PenLine(const Line &line) : Line(line) {}
        virtual ~PenLine(){}
};

class CappedPenLine : public PenLine {
    public:
        float cap_radius = 0.0f;

        CappedPenLine() {};
        CappedPenLine(const Line &line) : PenLine(line) {}
};

typedef list<CappedPenLine>::iterator LineIterator;

class DrawLayer : public Control {
    GDCLASS(DrawLayer, Control);
    private:
        list<CappedPenLine> lines = list<CappedPenLine>();

        enum PenMode { NONE, DRAW, ERASE };
        PenMode mode = NONE;

        void HandleMouseButton(const InputEventMouseButton &event);
        void HandleMouseMotion(const InputEventMouseMotion &event);
        void HandleKey(const InputEventKey &event);

    protected:
        static void _bind_methods();

    public:
        float line_width = 5.0f;
        float cap_scale = 1.0f;

        float eraser_size = 10.0f;
        float min_draw_distance = 5.0f;
        float max_draw_angle = 135.0f;

        int smooth_steps = 5;
        float smooth_ratio = 0.333333f;
        float smooth_min_distance = 0.1f;

        DrawLayer();
        ~DrawLayer();

        #pragma region Getters and Setters

        float get_line_width() { return line_width; }
        void set_line_width(float width) { line_width = width; }

        float get_cap_scale() { return cap_scale; }
        void set_cap_scale(float scale) { cap_scale = scale; }

        float get_eraser_size() { return eraser_size; }
        void set_eraser_size(float size) { eraser_size = size; }

        float get_min_draw_distance() { return min_draw_distance; }
        void set_min_draw_distance(float distance) { min_draw_distance = distance; }

        float get_max_draw_angle() { return max_draw_angle; }
        void set_max_draw_angle(float angle) { max_draw_angle = angle; }

        int get_smooth_steps() { return smooth_steps; }
        void set_smooth_steps(int steps) { smooth_steps = steps; }

        float get_smooth_ratio() { return smooth_ratio; }
        void set_smooth_ratio(float ratio) { smooth_ratio = ratio; }

        float get_smooth_min_distance() { return smooth_min_distance; }
        void set_smooth_min_distance(float distance) { smooth_min_distance = distance; }

        #pragma endregion

        void _unhandled_input(const Ref<InputEvent> &p_event) override;
        void _draw() override;

        void Erase(Vector2 pos);
        bool Erase(Vector2 pos, LineIterator line_it);
};

}

#endif