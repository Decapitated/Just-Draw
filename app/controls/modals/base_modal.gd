class_name BaseModal extends Control

@export var modal_background: Control

@export var cancel_button: Button
@export var apply_button: Button

func _ready():
    cancel_button.connect("pressed", _cancel)
    apply_button.connect("pressed", _apply)

func close():
    if modal_background:
        modal_background.hide()
    hide()

func open():
    reset()
    if modal_background:
        modal_background.show()
    show()

func reset():
    pass

func _cancel():
    close()

func _apply():
    close()