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
#include <algorithm>

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
        PenLine(const Color &p_color, float p_width) : color(p_color), width(p_width) {}
        PenLine(const Line &p_line, const Color &p_color, float p_width) : Line(p_line), color(p_color), width(p_width) {}
        PenLine(const PenLine &p_line) : PenLine(p_line, p_line.color, p_line.width) {}
        virtual ~PenLine() {}
};

class CappedPenLine : public PenLine {
    public:
        float cap_radius = 0.0f;

        CappedPenLine() {};
        CappedPenLine(const Color &p_color, float p_width, float p_cap_radius) : PenLine(p_color, p_width), cap_radius(p_cap_radius) {}
        CappedPenLine(const Line &p_line, const Color &p_color, float p_width, float p_cap_radius) : PenLine(p_line, p_color, p_width), cap_radius(p_cap_radius) {}
        CappedPenLine(const PenLine &p_line, float p_cap_radius) : PenLine(p_line), cap_radius(p_cap_radius) {}
        CappedPenLine(const CappedPenLine &p_line) : CappedPenLine(p_line, p_line.color, p_line.width, p_line.cap_radius) {}
        virtual ~CappedPenLine() {}
};

typedef list<CappedPenLine>::iterator LineIterator;

class DrawLayer : public Control {
    GDCLASS(DrawLayer, Control);
    private:
        list<CappedPenLine> lines = list<CappedPenLine>();

        enum PenMode { NONE, DRAW, ERASE };
        PenMode mode = NONE;

        bool active = false;
        
        float line_width = 5.0f;
        float cap_scale = 1.0f;

        float eraser_size = 10.0f;
        float min_draw_distance = 10.0f;
        float max_draw_angle = 135.0f;

        int smooth_steps = 1;
        float smooth_ratio = 0.333333f;
        float smooth_min_distance = 0.1f;

        void HandleMouseButton(const InputEventMouseButton &event);
        void HandleMouseMotion(const InputEventMouseMotion &event);
        void HandleKey(const InputEventKey &event);

        void StartDraw(Vector2 pen_position);
        void StartErase(Vector2 pen_position);

        void UpdateDraw(Vector2 pen_position);

        void UpdateErase(Vector2 pen_position);
        bool UpdateErase(Vector2 pen_position, LineIterator line_it);

    protected:
        static void _bind_methods();

    public:
        DrawLayer();
        ~DrawLayer();

        #pragma region Getters and Setters

        bool get_active() { return active; }
        void set_active(bool p_active) { active = p_active; }

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

        void _unhandled_input(const Ref<InputEvent> &p_event) override;
        void _draw() override;
};

}

#endif