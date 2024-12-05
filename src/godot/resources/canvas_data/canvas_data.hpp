#ifndef RESOURCE_CANVAS_H
#define RESOURCE_CANVAS_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

#include "godot/resources/layer_data/layer_data.hpp"

using namespace godot;

namespace JustDraw
{
    class CanvasData : public Resource
    {
        GDCLASS(CanvasData, Resource);
        private:
            Vector2i size;
            TypedArray<LayerData> layers;

        protected:
            static void _bind_methods();

        public:
            CanvasData() {}
            CanvasData(Vector2i p_size, TypedArray<LayerData> p_layers)
                : size(p_size), layers(p_layers) {}
            ~CanvasData() {}

            #pragma region Getters and Setters

            Vector2i get_size() { return size; }
            void set_size(Vector2i p_size) { size = p_size; }

            TypedArray<LayerData> get_layers() { return layers; }
            void set_layers(TypedArray<LayerData> p_layers) { layers = p_layers; }

            #pragma endregion
    };
}

#endif
