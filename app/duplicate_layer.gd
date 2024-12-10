class_name DuplicateLayer extends ColorRect

var layer: DrawLayer

func _process(_delta):
	if is_instance_valid(layer):
		var parent = layer.get_parent_control()
		if parent && parent is DrawCanvas:
			color = parent.color
			(get_parent() as SubViewport).size_2d_override = parent.size

func _draw():
	if layer:
		var layer_data = layer.layer_data
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
