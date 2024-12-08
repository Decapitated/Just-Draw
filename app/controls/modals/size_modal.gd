class_name SizeModal extends BaseModal

@export var width_spinbox: SpinBox
@export var height_spinbox: SpinBox

func get_input_size() -> Vector2:
    if width_spinbox && height_spinbox:
        return Vector2(width_spinbox.value, height_spinbox.value)
    return Vector2.ZERO

func set_input_size(p_size: Vector2):
    if width_spinbox && height_spinbox:
        width_spinbox.value = p_size.x
        height_spinbox.value = p_size.y
