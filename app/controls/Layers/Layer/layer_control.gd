class_name LayerControl extends Control

signal layer_selected(layer: DrawLayer)

var layer: DrawLayer

var active_flat_panel: StyleBoxFlat = StyleBoxFlat.new()

func _process(_delta):
    if is_instance_valid(layer) && layer.active:
        add_theme_stylebox_override("panel", active_flat_panel)
    else:
        remove_theme_stylebox_override("panel")

func _gui_input(event):
    if event is InputEventMouseButton && event.button_index == MOUSE_BUTTON_LEFT && event.pressed:
        if event.button_index == MOUSE_BUTTON_LEFT && event.pressed && layer:
            layer_selected.emit(layer)