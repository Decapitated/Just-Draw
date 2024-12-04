extends Control

@export var drawing: CanvasLayer

func _draw():
	duplicate_drawing()

func duplicate_drawing():
	var layers = drawing.find_children("*", "DrawLayer")
	for layer in layers:
		if layer is DrawLayer:
			var pens: Array[Dictionary] = layer.get_pens()
			var lines: Array[PackedVector2Array] = layer.get_lines()

			for i in range(lines.size()):
				var line: PackedVector2Array = lines[i]
				var pen = pens[i]
				if line.size() >= 2:
					draw_circle(line[0], pen.cap_radius, pen.color)
					draw_polyline(line, pen.color, pen.width)
					draw_circle(line[line.size() - 1], pen.cap_radius, pen.color)
				elif line.size() == 1:
					draw_circle(line[0], pen.width / 2.0, pen.color)
