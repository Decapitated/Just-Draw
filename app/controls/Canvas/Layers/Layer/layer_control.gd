class_name LayerControl extends Control

signal layer_selected(layer: DrawLayer)

@export var layer_view: DuplicateLayer

var layer: DrawLayer

var active_flat_panel: StyleBoxFlat = StyleBoxFlat.new()

func _ready():
    layer_view.layer = layer
    layer.updated.connect(layer_view.queue_redraw)
        
func _process(_delta):
    if is_instance_valid(layer):
        if layer.active:
            add_theme_stylebox_override("panel", active_flat_panel)
        else:
            remove_theme_stylebox_override("panel")
        var parent = get_parent_control()
        if parent && parent is DrawCanvas:
            (layer_view.get_parent() as SubViewport).size = parent.size

func _gui_input(event):
    if event is InputEventMouseButton && event.button_index == MOUSE_BUTTON_LEFT && event.pressed:
        if event.button_index == MOUSE_BUTTON_LEFT && event.pressed && layer:
            layer_selected.emit(layer)