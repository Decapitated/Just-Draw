class_name ImageSize extends SizeModal

@export var export_viewport: SubViewport

func _apply():
    var new_size = get_input_size() as Vector2i
    if new_size != export_viewport.size:
        export_viewport.size = new_size
    close()

func reset():
    set_input_size(export_viewport.size)
