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
            using Data = Array;
            Data data = Data();

            using Pens = TypedArray<Pen>;
            Pens pens = Pens();

        protected:
            static void _bind_methods();

        public:
            LayerData() {}
            LayerData(Data p_data, Pens p_pens)
                : data(p_data), pens(p_pens) {}
            ~LayerData() {}

            #pragma region Getters and Setters

            Data get_data() { return data; };
            void set_data(Data p_data) { data = p_data; };

            Pens get_pens() { return pens; };
            void set_pens(Pens p_pens) { pens = p_pens; };

            #pragma endregion
    };
}

#endif
