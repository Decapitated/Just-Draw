extends Control

@export var canvas: DrawCanvas

var current_canvas: CanvasData

func _draw():
	if current_canvas != null:
		for layer_data in current_canvas.layers:
			var lines = layer_data.lines
			var pens = layer_data.pens
			for i in range(lines.size()):
				var line: PackedVector2Array = lines[i]
				var pen = pens[i]
				if line.size() >= 2:
					draw_circle(line[0], pen.cap_radius, pen.color)
					draw_polyline(line, pen.color, pen.width)
					draw_circle(line[line.size() - 1], pen.cap_radius, pen.color)
				elif line.size() == 1:
					draw_circle(line[0], pen.width / 2.0, pen.color)

func duplicate_drawing():
		current_canvas = canvas.create_canvas_data()
		queue_redraw()

func load_drawing():
	if current_canvas != null:
		canvas.load_canvas_data(current_canvas)
