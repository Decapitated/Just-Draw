#ifndef DRAWLAYER_H
#define DRAWLAYER_H

#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

#include <vector>

using namespace std;
using namespace godot;

namespace JustDraw
{

typedef PackedVector2Array Line;

struct PenLine {
    Color color = Color();
    float width = 10.0f;
    Line line = Line();
};

class DrawLayer : public Control {
    GDCLASS(DrawLayer, Control);
    private:
        typedef vector<shared_ptr<PenLine>> PenLines;
        shared_ptr<PenLines> lines = make_shared<PenLines>();

        bool is_drawing = false;

        void HandleMouseButton(const InputEventMouseButton &event);
        void HandleMouseMotion(const InputEventMouseMotion &event);
        void HandleKey(const InputEventKey &event);

    protected:
        static void _bind_methods();

    public:
        DrawLayer();
        ~DrawLayer();

        void _unhandled_input(const Ref<InputEvent> &p_event) override;
        void _draw() override;
};

}

#endif