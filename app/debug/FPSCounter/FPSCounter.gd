extends Control

@onready var fps_label : Label = $MarginContainer/Label

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	fps_label.text = "FPS: %s" % Engine.get_frames_per_second()
