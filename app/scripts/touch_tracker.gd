extends Node

var touches: Dictionary

var prev_position: Vector2
var prev_distance: float
	
func _unhandled_input(event: InputEvent) -> void:
	# InputEventScreenTouch ~= InputEventMouseButton
	# Only triggers on press down/up events.
	if event is InputEventScreenTouch:
		if event.pressed:
			# Only allow 2 touch gestures.
			if touches.size() < 2:
				touches[event.index] = event
				if touches.size() == 2:
					prev_distance = get_gesture_distance()
					prev_position = get_gesture_position()
		# Remove touches as they are released.
		elif touches.has(event.index):
			touches.erase(event.index)
	# InputEventScreenDrag ~= InputEventMouseMotion
	# Only triggers on movement events.
	elif event is InputEventScreenDrag:
		# Update touch event data if it exists already.
		if touches.has(event.index):
			touches[event.index] = event
		# Check if only two touches is available.
		if touches.size() == 2.0:
			# Create pan gesture if changed.
			var current_position := get_gesture_position()
			if current_position != prev_position:
				create_pan_event(current_position)
			# Create zoom gesture if changed.
			var current_distance := get_gesture_distance()
			if current_distance != prev_distance:
				create_magnify_event(current_position, current_distance)

# Used for pan.
func get_gesture_position() -> Vector2:
	var values = touches.values()
	return ((values[0].position + values[1].position) as Vector2) / 2.0

# Used for zoom.
func get_gesture_distance() -> float:
	var values = touches.values()
	return ((values[0].position - values[1].position) as Vector2).length()

func create_pan_event(current_position: Vector2):
	var pan_delta := current_position - prev_position
	prev_position = current_position
	var pan_event = InputEventPanGesture.new()
	pan_event.position = current_position
	pan_event.delta = pan_delta
	Input.parse_input_event(pan_event)

func create_magnify_event(current_position: Vector2, current_distance: float):
	var zoom_delta := current_distance - prev_distance
	prev_distance = current_distance
	var magnify_event = InputEventMagnifyGesture.new()
	magnify_event.position = current_position
	magnify_event.factor = zoom_delta
	Input.parse_input_event(magnify_event)
