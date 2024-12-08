class_name CanvasSize extends Control

@export var modal_background: Control

@export var draw_canvas: DrawCanvas

@export var width_spinbox: SpinBox
@export var height_spinbox: SpinBox

@export var draw_options: OptionButton

enum DrawOptions { NONE, CENTER, SCALE }

func _cancel():
    close()

func _apply():
    var new_size = Vector2(width_spinbox.value, height_spinbox.value)
    if new_size != draw_canvas.size:
        match draw_options.selected:
            DrawOptions.CENTER:
                # Offset line data.
                var line_offset = (new_size - draw_canvas.size) / 2.0
                draw_canvas.offset_layers(line_offset)
            DrawOptions.SCALE:
                # Scale line data.
                var line_scale = new_size / draw_canvas.size
                draw_canvas.scale_layers(line_scale)
        (draw_canvas as Control).custom_minimum_size = new_size
    close()

func close():
    modal_background.hide()
    hide()

func open():
    reset()
    modal_background.show()
    show()

func reset():
    var current_size = draw_canvas.size
    width_spinbox.value = current_size.x
    height_spinbox.value = current_size.y
    draw_options.select(0)
