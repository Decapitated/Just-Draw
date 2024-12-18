class_name DashedLinePen extends LinePen

func _draw(_parent_item: RID, canvas_item: RID, _index: int, line: PackedVector2Array) -> void:
    if line.size() == 1:
        RenderingServer.canvas_item_add_circle(canvas_item, line[0], width / 2.0, color)
    elif line.size() % 2 == 0:
        var colors = PackedColorArray()
        colors.resize(floor(line.size() / 2.0))
        colors.fill(color)
        RenderingServer.canvas_item_add_multiline(canvas_item, line, colors, width)
        RenderingServer.canvas_item_add_circle(canvas_item, line[0], width / 2.0 * cap_scale, color)
        RenderingServer.canvas_item_add_circle(canvas_item, line[line.size() - 1], width / 2.0 * cap_scale, color)

func _finish_draw(line: PackedVector2Array) -> PackedVector2Array:
    return line