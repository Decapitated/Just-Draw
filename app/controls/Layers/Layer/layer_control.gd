class_name LayerControl extends Control

signal layer_selected(layer: DrawLayer)

var layer: DrawLayer

func _gui_input(event):
    if event is InputEventMouseButton && event.button_index == MOUSE_BUTTON_LEFT && event.pressed:
        if event.button_index == MOUSE_BUTTON_LEFT && event.pressed && layer:
            layer_selected.emit(layer)