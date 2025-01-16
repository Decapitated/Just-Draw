@tool
extends Control

@export var draw_canvas: DrawCanvas

var is_eraser: bool = false
var pen_position: Vector2 = Vector2.ZERO

func _ready():
    draw_canvas.mouse_entered.connect(_mouse_entered_canvas)
    draw_canvas.mouse_exited.connect(_mouse_exited_canvas)

func _process(_delta):
    if draw_canvas:
        custom_minimum_size = draw_canvas.size
        queue_redraw()

func _unhandled_input(event):
    if event is InputEventMouseMotion:
        event = make_input_local(event)
        is_eraser = event.pen_inverted
        pen_position = event.position

func _draw():
    if draw_canvas:
        var active_layer = draw_canvas.get_active_layer()
        var p_is_eraser = is_eraser or (active_layer and active_layer.get_pen_mode() == DrawLayer.ERASE)
        draw_canvas.pen.draw_cursor(self, pen_position, draw_canvas.eraser_size, p_is_eraser)

func _mouse_entered_canvas():
    Input.mouse_mode = Input.MOUSE_MODE_HIDDEN

func _mouse_exited_canvas():
    Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
