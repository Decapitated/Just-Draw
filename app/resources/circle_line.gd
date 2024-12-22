class_name CircleLine extends LinePen

@export var stencil_position := Vector2.ZERO
@export var stencil_radius := 100.0

var is_moving_stencil := false

func _start_draw(pen_position: Vector2, rs_pen: RSPen) -> Variant:
    var mouse_dir := pen_position - stencil_position
    var mouse_dist_sqr := mouse_dir.length_squared()
    var is_near_center := mouse_dist_sqr <= pow(10.0, 2.0)
    if is_near_center:
        is_moving_stencil = true
    else:
        var is_near_edge := absf(mouse_dist_sqr - pow(stencil_radius, 2.0)) <= pow(20.0, 2.0)
        if is_near_edge:
            return super.start_draw(pen_position, rs_pen)
    return null

func _update_draw(pen_position: Vector2, cam_scale: float, rs_pen: RSPen) -> Array:
    if is_moving_stencil:
        stencil_position = pen_position
    elif rs_pen:
        var new_data := super.update_draw(pen_position, cam_scale, rs_pen)
        if new_data.size() > 0:
            for i in new_data.size():
                var line := new_data[i] as PackedVector2Array
                for j in line.size():
                    var point := line[j]
                    var mouse_dir := (point - stencil_position).normalized()
                    line[j] = stencil_position + mouse_dir * stencil_radius
                new_data[i] = line
        else:
            var line := rs_pen.data as PackedVector2Array
            var latest_point := line[line.size() - 1]
            var mouse_dir := (latest_point - stencil_position).normalized()
            line[line.size() - 1] = stencil_position + mouse_dir * stencil_radius
            rs_pen.data = line
        return new_data
    return []

func _finish_draw(pen_position: Vector2, rs_pen: RSPen) -> bool:
    if is_moving_stencil:
        is_moving_stencil = false
    elif rs_pen:
        return super.finish_draw(pen_position, rs_pen)
    return false

func _draw_cursor(canvas_item: CanvasItem, pen_position: Vector2, eraser_size: float, is_eraser: bool) -> void:
    var cam := canvas_item.get_viewport().get_camera_2d()
    canvas_item.draw_circle(stencil_position, stencil_radius, Color(1.0, 1.0, 1.0, 0.5), true)
    canvas_item.draw_circle(stencil_position, stencil_radius, Color.DARK_GRAY, false, 2.0 * (1.0 / cam.zoom.x))
    super.draw_cursor(canvas_item, pen_position, eraser_size, is_eraser)
