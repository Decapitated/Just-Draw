extends MenuButton

@export var draw_canvas: DrawCanvas

@onready var file_dialog: FileDialog = $FileDialog

enum MenuOptions { OPEN, SAVE, EXPORT }

func _ready():
    get_popup().id_pressed.connect(_on_id_pressed)
    file_dialog.file_selected.connect(_on_file_selected)

func _on_id_pressed(id: int):
    if draw_canvas:
        match id:
            MenuOptions.OPEN:
                file_dialog.file_mode = FileDialog.FILE_MODE_OPEN_FILE
                file_dialog.visible = true
            MenuOptions.SAVE:
                file_dialog.file_mode = FileDialog.FILE_MODE_SAVE_FILE
                file_dialog.visible = true
            MenuOptions.EXPORT:
                print("export")
        
func _on_file_selected(path: String):
    if draw_canvas:
        match file_dialog.file_mode:
            FileDialog.FILE_MODE_OPEN_FILE:
                var res: Resource = ResourceLoader.load(path, "CanvasData", ResourceLoader.CACHE_MODE_IGNORE_DEEP)
                if res and res is CanvasData:
                    draw_canvas.load_canvas_data(res)
            FileDialog.FILE_MODE_SAVE_FILE:
                var canvas_data: CanvasData = draw_canvas.create_canvas_data()
                ResourceSaver.save(canvas_data, path)