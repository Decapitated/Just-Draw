#ifndef DRAWLAYER_H
#define DRAWLAYER_H

#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/rendering_server.hpp>

#include <list>
#include <vector>
#include <algorithm>

#include "godot/resources/layer_data/layer_data.hpp"
#include "godot/objects/rs_pen/rs_pen.hpp"

using namespace std;
using namespace godot;

namespace JustDraw
{
    using DataPens = list<Ref<RSPen>>;

    class DrawLayer : public Control
    {
        GDCLASS(DrawLayer, Control);
        public:
            enum PenMode { NONE, DRAW, ERASE };
        private:
            static const char* UPDATED_SIGNAL;

            DataPens pens = DataPens();

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
            bool UpdateErase(Vector2 pen_position, DataPens::iterator data_it);

            void FinishDraw();
            void FinishErase();

            void UpdateIndexes(DataPens::iterator data_it);

            Array GetData();
            TypedArray<Pen> GetPens();

        protected:
            static void _bind_methods();
	        void _notification(int p_what);

        public:
            DrawLayer();
            ~DrawLayer();

            #pragma region Getters and Setters

            bool get_active() { return active; }
            void set_active(bool p_active) { active = p_active; }

            Ref<LayerData> create_layer_data() { return memnew(LayerData(GetData(), GetPens())); }
            void load_layer_data(Ref<LayerData> p_layer_data);

            PenMode get_pen_mode() { return mode; }

            #pragma endregion

            PackedStringArray _get_configuration_warnings() const override;
            void _process(double p_delta) override;
            void _unhandled_input(const Ref<InputEvent> &p_event) override;

            void scale_data(Vector2 scale);
            void offset_data(Vector2 offset);
    };
}

#endif