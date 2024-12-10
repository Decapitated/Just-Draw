class_name CanvasSize extends SizeModal

@export var draw_canvas: DrawCanvas
@export var draw_options: OptionButton

enum DrawOptions { NONE, CENTER, SCALE }

func _apply():
    var new_size = get_input_size()
    if new_size != draw_canvas.size:
        match draw_options.selected:
            DrawOptions.CENTER:
                # Offset line data.
                var line_offset = (new_size - draw_canvas.size) / 2.0
                draw_canvas.offset_layers(line_offset)
            DrawOptions.SCALE:
                # Scale line data.
                var line_scale = new_size / draw_canvas.size
                draw_canvas.scale_layers(line_scale)
        (draw_canvas as Control).custom_minimum_size = new_size
    close()

func reset():
    set_input_size(draw_canvas.size)
    draw_options.select(0)
