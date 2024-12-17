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
		var pens: Array[Pen] = layer_data.pens
		for i in range(lines.size()):
			pens[i].draw(get_canvas_item(), lines[i])
