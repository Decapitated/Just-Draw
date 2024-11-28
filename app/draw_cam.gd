extends Camera2D

var panning := false

func _unhandled_input(event):
    if event is InputEventMouseButton:
        if event.button_index == MOUSE_BUTTON_MIDDLE:
            panning = event.pressed
    elif event is InputEventMouseMotion:
        if panning:
            position -= event.relative