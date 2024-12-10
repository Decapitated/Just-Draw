extends MenuButton

enum MenuOptions { CANVAS, IMAGE }

@export var canvas_size_modal: CanvasSize
@export var image_size_modal: ImageSize

func _ready():
	get_popup().id_pressed.connect(_on_id_pressed)

func _on_id_pressed(id: int):
		match id:
			MenuOptions.CANVAS:
				if canvas_size_modal:
					canvas_size_modal.open()
			MenuOptions.IMAGE:
				if image_size_modal:
					image_size_modal.open()
