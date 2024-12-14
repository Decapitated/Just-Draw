class_name LayerControl extends Control

signal layer_selected(layer: DrawLayer)

@export var layer_view: DuplicateLayer
@export var visibility_button: Button

var layer: DrawLayer

var active_flat_panel: StyleBoxFlat = StyleBoxFlat.new()

func _ready():
    layer_view.layer = layer
    layer.updated.connect(layer_view.queue_redraw)
        
func _process(_delta):
    if is_instance_valid(layer):
        layer.visible = visibility_button.button_pressed
        visibility_button.text = "visibility" if visibility_button.button_pressed else "visibility_off"
        if layer.active:
            add_theme_stylebox_override("panel", active_flat_panel)
        else:
            remove_theme_stylebox_override("panel")

func _gui_input(event):
    if event is InputEventMouseButton && event.button_index == MOUSE_BUTTON_LEFT && event.pressed:
        if event.button_index == MOUSE_BUTTON_LEFT && event.pressed && layer:
            layer_selected.emit(layer)

func _get_drag_data(_at_position) -> Variant:
    var self_dupe = self.duplicate()
    self_dupe.layer = layer
    set_drag_preview(self_dupe)
    return self

func _can_drop_data(_at_position: Vector2, data: Variant) -> bool:
    return typeof(data) == TYPE_OBJECT && \
           data is LayerControl && \
           (data as LayerControl).get_parent() == get_parent() && \
           (data as LayerControl).layer != layer

func _drop_data(_at_position: Vector2, data: Variant) -> void:
    var new_index = get_index()
    var layer_control = data as LayerControl
    layer_control.layer.get_parent().move_child(layer_control.layer, new_index)
    layer_control.get_parent().move_child(layer_control, new_index)
