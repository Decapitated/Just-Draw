class_name CanvasViewport extends SubViewport

@export var canvas: DrawCanvas

@onready var background: CopyCanvas = $CopyCanvas

func update_viewport():
    var canvas_data = canvas.create_canvas_data()
    size_2d_override = canvas_data.size
    background.current_canvas = canvas_data
    background.queue_redraw()

func save_canvas(path: String):
    await background.finished_drawing
    await RenderingServer.frame_post_draw
    var texture = get_texture()
    var image = texture.get_image()
    var extension = path.split(".")[-1].to_lower()
    match extension:
        "png":
            image.save_png(path)
        "jpg", "jpeg":
            image.save_jpg(path)
        "webp":
            image.save_webp(path)