extends Camera2D

@export var zoom_speed := 0.05

var panning := false

func _ready():
	position = get_viewport().size / 2.0


func _unhandled_input(event):
	if event is InputEventMouseButton:
		# Zoom increment
		if event.button_index == MOUSE_BUTTON_MIDDLE:
			panning = event.pressed
		elif event.button_index == MOUSE_BUTTON_WHEEL_UP || event.button_index == MOUSE_BUTTON_WHEEL_DOWN:
			var zoom_dir = 1.0 if event.button_index == MOUSE_BUTTON_WHEEL_UP else -1.0
			zoom = Vector2.ONE * max(zoom.x + zoom_speed * zoom_dir, 0.01)

	elif event is InputEventMouseMotion:
		if panning:
			position -= event.relative * (1.0 / zoom.x)
