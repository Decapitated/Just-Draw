class_name ImageSize extends Control

@export var modal_background: Control
@export var export_viewport: SubViewport

@export var width_spinbox: SpinBox
@export var height_spinbox: SpinBox

func _cancel():
    close()

func _apply():
    var new_size = Vector2i(width_spinbox.value as int, height_spinbox.value as int)
    if new_size != export_viewport.size:
        export_viewport.size = new_size
    close()

func close():
    modal_background.hide()
    hide()

func open():
    reset()
    modal_background.show()
    show()

func reset():
    var current_size = export_viewport.size
    width_spinbox.value = current_size.x
    height_spinbox.value = current_size.y
