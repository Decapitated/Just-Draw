class_name PenSettings extends Control

@export var canvas_controls: CanvasControls

@export var line_color: ColorPickerButton
@export var line_width: SpinBox
@export var cap_scale: SpinBox
@export var eraser_size: SpinBox

func _ready():
    if canvas_controls:
        var draw_canvas = canvas_controls.draw_canvas
        if draw_canvas:
            line_color.color = draw_canvas.line_color
            line_color.color_changed.connect(_on_line_color_changed)

            line_width.value = draw_canvas.line_width
            line_width.value_changed.connect(_on_line_width_changed)

            cap_scale.value = draw_canvas.cap_scale
            cap_scale.value_changed.connect(_on_cap_scale_changed)

            eraser_size.value = draw_canvas.eraser_size
            eraser_size.value_changed.connect(_on_eraser_size_changed)

func _on_line_color_changed(color):
    if canvas_controls:
        var draw_canvas = canvas_controls.draw_canvas
        if draw_canvas:
            draw_canvas.line_color = color
            line_color.color = draw_canvas.line_color

func _on_line_width_changed(value):
    if canvas_controls:
        var draw_canvas = canvas_controls.draw_canvas
        if draw_canvas:
            draw_canvas.line_width = value
            line_width.value = draw_canvas.line_width

func _on_cap_scale_changed(value):
    if canvas_controls:
        var draw_canvas = canvas_controls.draw_canvas
        if draw_canvas:
            draw_canvas.cap_scale = value
            cap_scale.value = draw_canvas.cap_scale

func _on_eraser_size_changed(value):
    if canvas_controls:
        var draw_canvas = canvas_controls.draw_canvas
        if draw_canvas:
            draw_canvas.eraser_size = value
            eraser_size.value = draw_canvas.eraser_size
