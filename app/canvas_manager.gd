class_name CanvasManager extends Node

@onready var draw_canvas: DrawCanvas = $DrawCanvas

func create_canvas_data() -> CanvasData:
    if draw_canvas:
        var layer_data_array: Array[LayerData] = []
        for layer in draw_canvas.find_children("*", "DrawLayer", false, false):
            layer_data_array.append(layer.get_layer_data())

        var canvas = CanvasData.new()
        canvas.size = draw_canvas.size
        canvas.layers = layer_data_array

        return canvas
    return null

func load_canvas_data(canvas_data: CanvasData):
    if draw_canvas:
        clear_canvas()
        var is_first = true
        for layer_data in canvas_data.layers:
            var draw_layer = DrawLayer.new()
            draw_canvas.add_child(draw_layer)
            if is_first:
                is_first = false
                draw_layer.active = true
            draw_layer.load_layer_data(layer_data)

func clear_canvas():
    if draw_canvas:
        var layers = draw_canvas.find_children("*", "DrawLayer", false, false)
        for layer in layers:
            layer.free()