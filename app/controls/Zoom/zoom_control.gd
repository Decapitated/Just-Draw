extends Control

@export var camera: Camera2D

@onready var zoom_out: Button = $"HBoxContainer/Zoom Out"
@onready var zoom_in: Button = $"HBoxContainer/Zoom In"
@onready var zoom_slider: HSlider = $"HBoxContainer/Zoom Slider"

var zoom_button_amount: float = 0.1

func _ready():
	zoom_out.pressed.connect(_on_zoom_out_pressed)
	zoom_in.pressed.connect(_on_zoom_in_pressed)
	zoom_slider.value_changed.connect(_on_zoom_slider_changed)

func _process(_delta):
	if camera.zoom.x != zoom_slider.value:
		zoom_slider.value = camera.zoom.x

func _on_zoom_out_pressed():
	zoom_slider.value -= zoom_button_amount

func _on_zoom_in_pressed():
	zoom_slider.value += zoom_button_amount

func _on_zoom_slider_changed(new_value):
	camera.zoom = Vector2.ONE * max(new_value, 0.01)
