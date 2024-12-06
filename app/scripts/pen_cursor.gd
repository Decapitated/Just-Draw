extends Control

@export var draw_canvas: DrawCanvas

func _ready():
    mouse_entered.connect(_mouse_entered_canvas)
    mouse_exited.connect(_mouse_exited_canvas)

func _process(_delta):
    if draw_canvas:
        queue_redraw()

func _draw():
    if draw_canvas:
        var active_layer = draw_canvas.get_active_layer()
        # Only show eraser cursor if the active layer is erasing. Otherwise, show the draw cursor as default.
        if active_layer and active_layer.get_pen_mode() == DrawLayer.ERASE:
            draw_cursor(draw_canvas.eraser_size, 0.5)
        else:
            draw_cursor(draw_canvas.line_width / 2.0, 0.5)

func draw_cursor(radius: float, width: float = 1.0):
    draw_circle(get_local_mouse_position(), radius + width / 2.0, Color.DARK_GRAY, false, width)

func _mouse_entered_canvas():
    Input.mouse_mode = Input.MOUSE_MODE_HIDDEN

func _mouse_exited_canvas():
    Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
