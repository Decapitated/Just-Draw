#include "register_types.hpp"

#include <godot_cpp/classes/engine.hpp>

#include "godot/resources/layer_data/layer_data.hpp"
#include "godot/resources/canvas_data/canvas_data.hpp"
#include "godot/nodes/draw_layer/draw_layer.hpp"
#include "godot/nodes/draw_canvas/draw_canvas.hpp"

using namespace godot;

void initialize_justdraw_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
    
    GDREGISTER_CLASS(JustDraw::LayerData);
    GDREGISTER_CLASS(JustDraw::CanvasData);
    GDREGISTER_CLASS(JustDraw::DrawLayer);
    GDREGISTER_CLASS(JustDraw::DrawCanvas);
}

void uninitialize_justdraw_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
    // Initialization.
    GDExtensionBool GDE_EXPORT justdraw_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_justdraw_module);
        init_obj.register_terminator(uninitialize_justdraw_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
        return init_obj.init();
    }
}