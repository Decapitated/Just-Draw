extends Camera2D

@export var zoom_speed := 0.05

var panning := false

func _unhandled_input(event):
	if event is InputEventMouseButton:
		# Zoom increment
		if event.button_index == MOUSE_BUTTON_MIDDLE:
			panning = event.pressed
		elif event.button_index == MOUSE_BUTTON_WHEEL_UP || event.button_index == MOUSE_BUTTON_WHEEL_DOWN:
			zoom += Vector2.ONE * zoom_speed * (1.0 if event.button_index == MOUSE_BUTTON_WHEEL_UP else -1.0)

	elif event is InputEventMouseMotion:
		if panning:
			position -= event.relative * (1.0 / zoom.x)
