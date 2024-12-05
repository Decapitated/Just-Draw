extends Control

@export var drawing: CanvasLayer

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
	var background = drawing.get_node("Background")
	if background:
		var layer_data_array: Array[LayerData] = []
		var layers = background.find_children("*", "DrawLayer", false, false)
		for layer in layers:
			layer_data_array.append(layer.get_layer_data())

		var canvas = CanvasData.new()
		canvas.size = background.size
		canvas.layers = layer_data_array

		current_canvas = canvas
		queue_redraw()

func load_drawing():
	if current_canvas != null:
		var background = drawing.get_node("Background")
		if background:
			var layers = background.find_children("*", "DrawLayer", false, false)
			for layer in layers:
				layer.free()	
			
			var is_first = true
			for layer_data in current_canvas.layers:
				var draw_layer = DrawLayer.new()
				background.add_child(draw_layer)
				draw_layer.set_anchors_preset(PRESET_FULL_RECT)
				if is_first:
					is_first = false
					draw_layer.active = true
				draw_layer.load_layer_data(layer_data)
