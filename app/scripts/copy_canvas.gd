class_name CopyCanvas extends ColorRect

var current_canvas: CanvasData

var items: Array[RID] = []

signal finished_drawing

func _exit_tree() -> void:
	clear_items()

func _draw():
	clear_items()
	if current_canvas != null:
		var global_index = 0
		for layer_data in current_canvas.layers:
			var data := layer_data.data
			var pens = layer_data.pens
			for i in range(data.size()):
				var item = RenderingServer.canvas_item_create()
				items.append(item)
				pens[i].draw(get_canvas_item(), item, global_index, data[i])
				global_index += 1
		finished_drawing.emit()

func clear_items():
	for item in items:
		RenderingServer.free_rid(item)
