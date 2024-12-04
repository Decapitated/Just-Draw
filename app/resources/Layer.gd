class_name Layer extends Resource

@export var lines: Array[PackedVector2Array]
@export var pens: Array[Dictionary]

func _init(p_lines: Array[PackedVector2Array], p_pens: Array[Dictionary]):
    lines = p_lines
    pens = p_pens