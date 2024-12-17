class_name CopyCanvas extends ColorRect

var current_canvas: CanvasData

signal finished_drawing

func _draw():
	if current_canvas != null:
		for layer_data in current_canvas.layers:
			var lines = layer_data.lines
			var pens = layer_data.pens
			for i in range(lines.size()):
				pens[i].draw(get_canvas_item(), lines[i])
		finished_drawing.emit()
