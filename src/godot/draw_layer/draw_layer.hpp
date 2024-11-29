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
        float width = 10.0f;

        PenLine() {};
        virtual ~PenLine(){};
};

class CappedPenLine : public PenLine {
    public:
        float cap_radius = 0.0f;
};

class DrawLayer : public Control {
    GDCLASS(DrawLayer, Control);
    private:
        typedef list<shared_ptr<CappedPenLine>> PenLines;
        shared_ptr<PenLines> lines = make_shared<PenLines>();

        bool is_drawing = false;

        void HandleMouseButton(const InputEventMouseButton &event);
        void HandleMouseMotion(const InputEventMouseMotion &event);
        void HandleKey(const InputEventKey &event);

    protected:
        static void _bind_methods();

    public:
        float line_width = 10.0f;
        float cap_scale = 1.0f;

        DrawLayer();
        ~DrawLayer();

        float get_line_width() { return line_width; }
        void set_line_width(float width) { line_width = width; }

        float get_cap_scale() { return cap_scale; }
        void set_cap_scale(float scale) { cap_scale = scale; }

        void _unhandled_input(const Ref<InputEvent> &p_event) override;
        void _draw() override;
};

}

#endif