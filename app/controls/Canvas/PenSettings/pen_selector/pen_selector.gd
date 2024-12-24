@tool
class_name PenSelector extends Control

@export var canvas_controls: CanvasControls

@onready var menu_toggle: Button = $MenuToggle
@onready var menu: PopupPanel = $MenuPanel
@onready var pen_container: Control = $MenuPanel/PenContainer

var current_pen_settings: PenSettings

func _ready():
	menu_toggle.toggled.connect(_menu_toggle_pressed)
	var index = 0
	for pen_button in pen_container.find_children("*", "PenButton", false):
		if index == 0:
			call_deferred("_on_pen_button_pressed", pen_button)
		menu_toggle.icon = pen_button.icon
		(pen_button as PenButton).pressed.connect(_on_pen_button_pressed.bind(pen_button))
		index += 1

func _process(_delta):
	custom_minimum_size = (get_child(0) as Control).size
	menu.position = global_position + Vector2(0, size.y)

func _menu_toggle_pressed(toggled_on: bool):
	menu.visible = toggled_on

func _on_pen_button_pressed(pen_button: PenButton):
	if canvas_controls and canvas_controls.draw_canvas and pen_button and pen_button.pen:
		if current_pen_settings:
			current_pen_settings.free()
		canvas_controls.draw_canvas.pen = pen_button.pen
		menu_toggle.icon = pen_button.icon
		var pen_settings_control: PenSettings = pen_button.pen_settings_control.instantiate()
		pen_settings_control.canvas_controls = canvas_controls
		add_sibling(pen_settings_control)
		current_pen_settings = pen_settings_control
		menu_toggle.button_pressed  = false
