class_name Canvas extends Resource

@export var size: Vector2i
@export var layers: Array[Layer]

func _init(p_size: Vector2i, p_layers: Array[Layer]):
    size = p_size
    layers = p_layers
