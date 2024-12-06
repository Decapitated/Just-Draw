#ifndef RESOURCE_LAYER_H
#define RESOURCE_LAYER_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

using namespace godot;

namespace JustDraw
{
    class LayerData : public Resource
    {
        GDCLASS(LayerData, Resource);
        private:
            TypedArray<PackedVector2Array> lines;
            TypedArray<Dictionary> pens;

        protected:
            static void _bind_methods();

        public:
            LayerData() {}
            LayerData(TypedArray<PackedVector2Array> p_lines, TypedArray<Dictionary> p_pens)
                : lines(p_lines), pens(p_pens) {}
            ~LayerData() {}

            #pragma region Getters and Setters

            TypedArray<PackedVector2Array> get_lines() { return lines; };
            void set_lines(TypedArray<PackedVector2Array> p_lines) { lines = p_lines; };

            TypedArray<Dictionary> get_pens() { return pens; };
            void set_pens(TypedArray<Dictionary> p_pens) { pens = p_pens; };

            #pragma endregion
    };
}

#endif
