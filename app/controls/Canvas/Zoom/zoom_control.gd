extends Control

@export var canvas_controls: CanvasControls

@onready var zoom_out: Button = $"Zoom Out"
@onready var zoom_in: Button = $"Zoom In"
@onready var zoom_slider: HSlider = $"Zoom Slider"

var zoom_button_amount: float = 0.1

func _ready():
	zoom_out.pressed.connect(_on_zoom_out_pressed)
	zoom_in.pressed.connect(_on_zoom_in_pressed)
	zoom_slider.value_changed.connect(_on_zoom_slider_changed)

func _process(_delta):
	if canvas_controls:
		var camera = canvas_controls.camera
		if camera && camera.zoom.x != zoom_slider.value:
			zoom_slider.value = camera.zoom.x

func _on_zoom_out_pressed():
	zoom_slider.value -= zoom_button_amount

func _on_zoom_in_pressed():
	zoom_slider.value += zoom_button_amount

func _on_zoom_slider_changed(new_value):
	if canvas_controls:
		var camera = canvas_controls.camera
		if camera:
			camera.zoom = Vector2.ONE * max(new_value, 0.01)
