extends MenuButton

@export var draw_canvas: DrawCanvas
@export var subviewport: CanvasViewport

@onready var open_save_dialog: FileDialog = $OpenSaveFileDialog
@onready var export_dialog: FileDialog = $ExportFileDialog

enum MenuOptions { OPEN, SAVE, EXPORT }

func _ready():
    get_popup().id_pressed.connect(_on_id_pressed)
    open_save_dialog.file_selected.connect(_on_open_save_file_selected)
    export_dialog.file_selected.connect(_on_export_file_selected)

func _on_id_pressed(id: int):
    if draw_canvas:
        match id:
            MenuOptions.OPEN:
                open_save_dialog.file_mode = FileDialog.FILE_MODE_OPEN_FILE
                open_save_dialog.visible = true
            MenuOptions.SAVE:
                open_save_dialog.file_mode = FileDialog.FILE_MODE_SAVE_FILE
                open_save_dialog.visible = true
            MenuOptions.EXPORT:
                export_dialog.visible = true
        
func _on_open_save_file_selected(path: String):
    if draw_canvas:
        match open_save_dialog.file_mode:
            FileDialog.FILE_MODE_OPEN_FILE:
                var res: Resource = ResourceLoader.load(path, "CanvasData", ResourceLoader.CACHE_MODE_IGNORE_DEEP)
                if res and res is CanvasData:
                    draw_canvas.load_canvas_data(res)
            FileDialog.FILE_MODE_SAVE_FILE:
                var canvas_data: CanvasData = draw_canvas.create_canvas_data()
                ResourceSaver.save(canvas_data, path, ResourceSaver.FLAG_COMPRESS)

func _on_export_file_selected(path: String):
    subviewport.update_viewport()
    subviewport.save_canvas(path)
    