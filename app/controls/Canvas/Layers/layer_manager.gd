class_name LayerManager extends Control

@export var canvas_controls: CanvasControls

@export var layer_list: VBoxContainer
@export var layer_control: PackedScene

@export var create_button: Button
@export var delete_button: Button
@export var move_up_button: Button
@export var move_down_button: Button

func _ready():
    setup_layers()
    setup_buttons()
    if is_valid():
        var draw_canvas = canvas_controls.draw_canvas
        draw_canvas.data_loaded.connect(setup_layers)

func is_valid():
    return canvas_controls && canvas_controls.draw_canvas && layer_list && layer_control

func setup_layers():
    if is_valid():
        _clear()
        var draw_canvas = canvas_controls.draw_canvas
        for layer in draw_canvas.get_children():
            var layer_control_instance = layer_control.instantiate() as LayerControl
            layer_control_instance.layer = layer
            layer_control_instance.layer_selected.connect(_set_active_layer)
            layer_list.add_child(layer_control_instance)

func setup_buttons():
    if create_button:
        create_button.pressed.connect(create_layer)
    if delete_button:
        delete_button.pressed.connect(delete_layer)
    if move_up_button:
        move_up_button.pressed.connect(move_layer_up)
    if move_down_button:	
        move_down_button.pressed.connect(move_layer_down)

func create_layer():
    print("create layer")
    if is_valid():
        var draw_canvas = canvas_controls.draw_canvas
        var new_draw_layer = DrawLayer.new()
        draw_canvas.add_child(new_draw_layer)
        draw_canvas.active_layer = new_draw_layer

        var layer_control_instance = layer_control.instantiate()
        layer_control_instance.layer = new_draw_layer
        layer_control_instance.layer_selected.connect(_set_active_layer)
        layer_list.add_child(layer_control_instance)

func delete_layer():
    if is_valid():
        var draw_canvas = canvas_controls.draw_canvas
        var active_layer = draw_canvas.active_layer
        if active_layer:
            for layer in layer_list.get_children():
                if layer.layer != active_layer:
                    continue
                var new_active_layer = _get_next_active_layer(layer)
                if new_active_layer:
                    draw_canvas.active_layer = new_active_layer.layer
                layer.queue_free()
                break;
            active_layer.queue_free()

func move_layer_up():
    if is_valid():
        var draw_canvas = canvas_controls.draw_canvas
        var active_layer = draw_canvas.active_layer
        if active_layer:
            for layer in layer_list.get_children():
                if layer.layer != active_layer:
                    continue
                var child_index = layer.get_index()
                if child_index > 0:
                    layer_list.move_child(layer, child_index - 1)
                    draw_canvas.move_child(layer.layer, child_index - 1)
                break

func move_layer_down():
    if is_valid():
        var draw_canvas = canvas_controls.draw_canvas
        var active_layer = draw_canvas.active_layer
        if active_layer:
            for layer in layer_list.get_children():
                if layer.layer != active_layer:
                    continue
                var child_index = layer.get_index()
                if child_index < layer_list.get_child_count() - 1:
                    layer_list.move_child(layer, child_index + 1)
                    draw_canvas.move_child(layer.layer, child_index + 1)
                break

func _clear():
    for layer in layer_list.get_children():
        layer.queue_free()

func _get_next_active_layer(layer: LayerControl) -> LayerControl:
    var new_active_layer = layer_list.get_child(max(layer.get_index() - 1, 0))
    if new_active_layer == layer:
        new_active_layer = null
    if !new_active_layer:
        new_active_layer = layer_list.get_child(layer.get_index() + 1)
    return new_active_layer

func _set_active_layer(layer: DrawLayer):
    if is_valid():
        var draw_canvas = canvas_controls.draw_canvas
        draw_canvas.active_layer = layer
