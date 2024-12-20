class_name DuplicateLayer extends ColorRect

var layer: DrawLayer

var items: Array[RID] = []

func _process(_delta):
	if is_instance_valid(layer):
		var parent = layer.get_parent_control()
		if parent && parent is DrawCanvas:
			color = parent.color
			(get_parent() as SubViewport).size_2d_override = parent.size

func _draw():
	clear_items()
	if layer:
		var layer_data := layer.layer_data
		var data := layer_data.data
		var pens := layer_data.pens
		for i in range(data.size()):
			var item = RenderingServer.canvas_item_create()
			items.append(item)
			pens[i].draw(get_canvas_item(), item, i, data[i])

func clear_items():
	for item in items:
		RenderingServer.free_rid(item)
