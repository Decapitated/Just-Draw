@tool
class_name AspectFix extends AspectRatioContainer

func _process(_delta):
    var child = get_child(0)
    if child is Control:
        custom_minimum_size.y = min(child.size.y, size.x)