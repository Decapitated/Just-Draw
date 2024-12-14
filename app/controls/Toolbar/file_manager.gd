extends MenuButton

@export var draw_canvas: DrawCanvas
@export var subviewport: CanvasViewport

@onready var open_save_dialog: FileDialog = $OpenSaveFileDialog
@onready var export_dialog: FileDialog = $ExportFileDialog

var jsFileLoadCallback = JavaScriptBridge.create_callback(_on_js_file_load)

enum MenuOptions { OPEN, SAVE, EXPORT }

func _ready():
    get_popup().id_pressed.connect(_on_id_pressed)
    if OS.get_name() == "Web":
        var js_window = JavaScriptBridge.get_interface("window")
        js_window.setupOpenFile(jsFileLoadCallback)
    else:
        open_save_dialog.file_selected.connect(_on_open_save_file_selected)
        export_dialog.file_selected.connect(_on_export_file_selected)
        get_viewport().files_dropped.connect(_on_files_dropped)

func _on_id_pressed(id: int):
    if draw_canvas:
        match id:
            MenuOptions.OPEN:
                match OS.get_name():
                    "Web":
                        var js_window = JavaScriptBridge.get_interface("window")
                        js_window.openFileInput.click()
                    _:
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

func _on_files_dropped(files: PackedStringArray):
    _on_open_save_file_selected(files[0])

const TEMP_FILE = "temp_load_dhova5k2f7igh.res"
const TEMP_FILE_PATH = "user://" + TEMP_FILE

func _on_js_file_load(_args):
    var buffer: PackedByteArray = JavaScriptBridge.eval("getFileBuffer();", true);
    if buffer:
        # Check if temp file exists. If it does, delete it.
        if FileAccess.file_exists(TEMP_FILE_PATH):
            var dir = DirAccess.open("user://")
            dir.remove(TEMP_FILE)
        # Write buffer to temp file.
        var file = FileAccess.open(TEMP_FILE_PATH, FileAccess.WRITE)
        file.store_buffer(buffer)
        file.close()
        # Load temp file as CanvasData resource.
        var res: Resource = ResourceLoader.load(TEMP_FILE_PATH, "CanvasData", ResourceLoader.CACHE_MODE_IGNORE_DEEP)
        if res and res is CanvasData:
            draw_canvas.load_canvas_data(res)
