#ifndef RESOURCE_LAYER_H
#define RESOURCE_LAYER_H

#include "godot/resources/pens/pen/pen.hpp"

using namespace godot;

namespace JustDraw
{
    class LayerData : public Resource
    {
        GDCLASS(LayerData, Resource);
        private:
            using Lines = TypedArray<Line>;
            Lines lines = Lines();

            using Pens = TypedArray<Pen>;
            Pens pens = Pens();

        protected:
            static void _bind_methods();

        public:
            LayerData() {}
            LayerData(Lines p_lines, Pens p_pens)
                : lines(p_lines), pens(p_pens) {}
            ~LayerData() {}

            #pragma region Getters and Setters

            Lines get_lines() { return lines; };
            void set_lines(Lines p_lines) { lines = p_lines; };

            Pens get_pens() { return pens; };
            void set_pens(Pens p_pens) { pens = p_pens; };

            #pragma endregion
    };
}

#endif
