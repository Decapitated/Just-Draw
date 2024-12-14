extends MenuButton

@export var draw_canvas: DrawCanvas
@export var subviewport: CanvasViewport

@onready var open_save_dialog: FileDialog = $OpenSaveFileDialog
@onready var export_dialog: FileDialog = $ExportFileDialog

var jsFileLoadCallback = JavaScriptBridge.create_callback(_on_js_file_load)

enum MenuOptions { OPEN, SAVE, EXPORT }

const TEMP_RES_SAVE_FILE = "temp_res_save_dhova5k2f7igh.res"
const TEMP_RES_SAVE_FILE_PATH = "user://" + TEMP_RES_SAVE_FILE

const TEMP_IMAGE_SAVE_FILE = "temp_image_save_dhova5k2f7igh.png"
const TEMP_IMAGE_SAVE_FILE_PATH = "user://" + TEMP_IMAGE_SAVE_FILE

const TEMP_LOAD_FILE = "temp_load_dhova5k2f7igh.res"
const TEMP_LOAD_FILE_PATH = "user://" + TEMP_LOAD_FILE

func _ready():
    get_popup().id_pressed.connect(_on_id_pressed)
    match OS.get_name():
        "Web":
            var js_window = JavaScriptBridge.get_interface("window")
            js_window.setupOpenFile(jsFileLoadCallback)
        _:
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
                match OS.get_name():
                    "Web":
                        var canvas_data := draw_canvas.create_canvas_data()
                        var result := ResourceSaver.save(canvas_data, TEMP_RES_SAVE_FILE_PATH, ResourceSaver.FLAG_COMPRESS)
                        if result == OK:
                            save_file_web(TEMP_RES_SAVE_FILE_PATH)
                    _:
                        open_save_dialog.file_mode = FileDialog.FILE_MODE_SAVE_FILE
                        open_save_dialog.visible = true
            MenuOptions.EXPORT:
                match OS.get_name():
                    "Web":
                        var result = await _on_export_file_selected(TEMP_IMAGE_SAVE_FILE_PATH)
                        if result == OK:
                            save_file_web(TEMP_IMAGE_SAVE_FILE_PATH, "image/png")
                    _:
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

func _on_export_file_selected(path: String) -> int:
    subviewport.update_viewport()
    return await subviewport.save_canvas(path)

func _on_files_dropped(files: PackedStringArray):
    _on_open_save_file_selected(files[0])

func save_file_web(path: String, type = "text/plain"):
        var buffer := FileAccess.get_file_as_bytes(path)
        if buffer.size() > 0:
            var uid := ResourceUID.id_to_text(ResourceUID.create_id()).trim_prefix("uid://")
            var file_name := uid + "." + path.get_extension()
            var js_window := JavaScriptBridge.get_interface("window")
            js_window.downloadFile(Marshalls.raw_to_base64(buffer), file_name, type)

func _on_js_file_load(_args):
    var buffer: PackedByteArray = JavaScriptBridge.eval("getFileBuffer();", true);
    if buffer:
        # Check if temp file exists. If it does, delete it.
        if FileAccess.file_exists(TEMP_LOAD_FILE_PATH):
            var dir = DirAccess.open("user://")
            dir.remove(TEMP_LOAD_FILE)
        # Write buffer to temp file.
        var file = FileAccess.open(TEMP_LOAD_FILE_PATH, FileAccess.WRITE)
        file.store_buffer(buffer)
        file.close()
        # Load temp file as CanvasData resource.
        var res: Resource = ResourceLoader.load(TEMP_LOAD_FILE_PATH, "CanvasData", ResourceLoader.CACHE_MODE_IGNORE_DEEP)
        if res and res is CanvasData:
            draw_canvas.load_canvas_data(res)
