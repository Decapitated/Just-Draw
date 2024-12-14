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
#include <vector>
#include <algorithm>

#include "godot/resources/layer_data/layer_data.hpp"

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

    using Lines = list<CappedPenLine>;
    using LineIterator = Lines::iterator;

    class DrawLayer : public Control
    {
        GDCLASS(DrawLayer, Control);
        public:
            enum PenMode { NONE, DRAW, ERASE };
        private:
            static const char* UPDATED_SIGNAL;

            Lines lines = Lines();

            PenMode mode = PenMode::NONE;

            bool active = false;
            bool dragging = false;

            void HandleMouseButton(const InputEventMouseButton &event);
            void HandleMouseMotion(const InputEventMouseMotion &event);
            void HandleKey(const InputEventKey &event);

            void StartDraw(Vector2 pen_position);
            void StartErase(Vector2 pen_position);

            void UpdateDraw(Vector2 pen_position);

            void UpdateErase(Vector2 pen_position);
            bool UpdateErase(Vector2 pen_position, LineIterator line_it);

            void FinishDraw();
            void FinishErase();

            TypedArray<PackedVector2Array> GetLines();
            TypedArray<Dictionary> GetPens();

        protected:
            static void _bind_methods();
	        void _notification(int p_what);

        public:
            DrawLayer();
            ~DrawLayer();

            #pragma region Getters and Setters

            bool get_active() { return active; }
            void set_active(bool p_active) { active = p_active; }

            Ref<LayerData> create_layer_data() { return memnew(LayerData(GetLines(), GetPens())); }
            void load_layer_data(Ref<LayerData> p_layer_data);

            PenMode get_pen_mode() { return mode; }

            #pragma endregion

            PackedStringArray _get_configuration_warnings() const override;
            void _process(double p_delta) override;
            void _unhandled_input(const Ref<InputEvent> &p_event) override;
            void _draw() override;
            
            static void SmoothLineStep(Line &line, float smooth_ratio, float smooth_min_distance, int smooth_start = 0);
            void SmoothLine(Line &line, int smooth_start = 0);

            void scale_lines(Vector2 scale);
            void offset_lines(Vector2 offset);
    };
}

#endif