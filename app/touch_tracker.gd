extends Node2D

var touches: Dictionary

var prev_distance := 0.0
var prev_position := Vector2()

var zoom_delta := 0.0
var pan_delta := Vector2()

var pos = Vector2()
var radius = 100.0

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
	
func _unhandled_input(event: InputEvent) -> void:
	if event is InputEventScreenTouch:
		if event.pressed:
			if touches.size() < 2:
				touches[event.index] = event
				if touches.size() == 2:
					prev_distance = get_gesture_distance()
					prev_position = get_gesture_position()
		elif touches.has(event.index):
			touches.erase(event.index)
	elif event is InputEventScreenDrag:
		if touches.has(event.index):
			touches[event.index] = event
		if touches.size() == 2.0:
			var current_distance = get_gesture_distance()
			var current_position = get_gesture_position()
			zoom_delta = current_distance - prev_distance
			prev_distance = current_distance
			pan_delta = current_position - prev_position
			prev_position = current_position
			
			pos += pan_delta * 10.0
			radius += zoom_delta
	queue_redraw()

func _draw() -> void:
	draw_circle(pos, radius, Color.RED)

# Used for zoom.
func get_gesture_distance() -> float:
	var values = touches.values()
	return ((values[0].position - values[1].position) as Vector2).length()

# Used for pan.
func get_gesture_position() -> Vector2:
	var values = touches.values()
	return ((values[0].position - values[1].position) as Vector2) / 2.0
