#include "draw_layer.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/camera2d.hpp>

#include "godot/nodes/draw_canvas/draw_canvas.hpp"
#include "godot/resources/pens/line_pen/line_pen.hpp"

using namespace JustDraw;
using namespace godot;

VARIANT_ENUM_CAST(DrawLayer::PenMode);

const char* DrawLayer::UPDATED_SIGNAL = "updated";

void DrawLayer::_bind_methods()
{
    #pragma region Getters and Setters

    ClassDB::bind_method(D_METHOD("set_active", "p_active"), &DrawLayer::set_active);
    ClassDB::bind_method(D_METHOD("get_active"), &DrawLayer::get_active);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");

    ClassDB::bind_method(D_METHOD("create_layer_data"), &DrawLayer::create_layer_data);
    ClassDB::bind_method(D_METHOD("load_layer_data", "p_layer_data"), &DrawLayer::load_layer_data);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "layer_data", PROPERTY_HINT_RESOURCE_TYPE, "LayerData",
                              PROPERTY_USAGE_NO_EDITOR),
                 "load_layer_data", "create_layer_data");

    #pragma endregion
    
    BIND_ENUM_CONSTANT(NONE);
    BIND_ENUM_CONSTANT(DRAW);
    BIND_ENUM_CONSTANT(ERASE);

    ClassDB::bind_method(D_METHOD("get_pen_mode"), &DrawLayer::get_pen_mode);
    ClassDB::bind_method(D_METHOD("scale_data", "p_scale"), &DrawLayer::scale_data);
    ClassDB::bind_method(D_METHOD("offset_data", "p_offset"), &DrawLayer::offset_data);

    ADD_SIGNAL(MethodInfo(UPDATED_SIGNAL));
}

DrawLayer::DrawLayer()
{
    set_mouse_filter(MOUSE_FILTER_PASS);
}

DrawLayer::~DrawLayer() {}

void DrawLayer::_notification(int p_what)
{
    if(p_what == NOTIFICATION_DRAG_BEGIN)
    {
        dragging = true;
    }
    else if(p_what == NOTIFICATION_DRAG_END)
    {
        dragging = false;
    }
}

PackedStringArray DrawLayer::_get_configuration_warnings() const
{
    auto warnings = PackedStringArray();
    auto parent = dynamic_cast<DrawCanvas*>(get_parent_control());
    if(parent == nullptr)
    {
        warnings.push_back("DrawLayer must be a child of a DrawCanvas.");
    }
    return warnings;
}

void DrawLayer::_unhandled_input(const Ref<InputEvent> &p_event)
{
    if(active && !dragging && p_event->get_device() != InputEvent::DEVICE_ID_EMULATION)
    {
        const Ref<InputEvent> event = make_input_local(*p_event);
        if(auto e = dynamic_cast<InputEventMouseButton*>(*event))
        {
            HandleMouseButton(*e);
        }
        else if(auto e = dynamic_cast<InputEventMouseMotion*>(*event))
        {
            HandleMouseMotion(*e);
        }
        else if(auto e = dynamic_cast<InputEventKey*>(*event))
        {
            HandleKey(*e);
        }
    }
}

void DrawLayer::_process(double p_delta)
{
    set_position(Vector2());
    set_size(get_parent_control()->get_size());
}

void DrawLayer::HandleMouseButton(const InputEventMouseButton &event)
{
    if(!event.is_pressed())
    {
        if(mode == DRAW)
        {
            mode = NONE;
            FinishDraw();
        }
        else if(mode == ERASE)
        {
            mode = NONE;
            FinishErase();
        }
    }
}

void DrawLayer::HandleMouseMotion(const InputEventMouseMotion &event)
{
    const bool is_left_button_pressed = (event.get_button_mask() & MOUSE_BUTTON_MASK_LEFT) == MOUSE_BUTTON_MASK_LEFT;
    const bool is_right_button_pressed = (event.get_button_mask() & MOUSE_BUTTON_MASK_RIGHT) == MOUSE_BUTTON_MASK_RIGHT;
    const float pen_pressure = event.get_pressure() || is_left_button_pressed || is_right_button_pressed;
    const bool pen_inverted = event.get_pen_inverted();
    const Vector2 pen_position = event.get_position();
    
    if(pen_pressure > 0.0f)
    {
        if(mode == ERASE)
        {
            UpdateErase(pen_position);
        }
        else if(mode == DRAW)
        {
            UpdateDraw(pen_position);
        }
        else if(mode == NONE)
        {
            if(is_left_button_pressed && !pen_inverted)
            {
                StartDraw(pen_position);
            }
            else if(is_right_button_pressed || pen_inverted)
            {
                StartErase(pen_position);
            }
        }
    }
    else
    {
        if(mode == ERASE)
        {
            mode = NONE;
            FinishErase();
        }
        else if(mode == DRAW)
        {
            mode = NONE;
            FinishDraw();
        }
    }
}

void DrawLayer::HandleKey(const InputEventKey &event) {}

void DrawLayer::StartDraw(Vector2 pen_position)
{
    auto canvas = dynamic_cast<DrawCanvas*>(get_parent_control());
    if(canvas != nullptr)
    {
        mode = DRAW;
        Ref<Pen> new_pen = canvas->pen->duplicate(true);
        auto new_data = new_pen->StartDraw(pen_position);
        if(new_data.get_type() == Variant::NIL) return;
        Ref<RSPen> new_rs_pen = memnew(RSPen(new_data, new_pen));
        new_rs_pen->Update(get_canvas_item(), pens.size());
        // Add the line to the list.
        pens.push_back(new_rs_pen);
    }
}

void DrawLayer::StartErase(Vector2 pen_position)
{
    mode = ERASE;
    UpdateErase(pen_position);
}

void DrawLayer::UpdateDraw(Vector2 pen_position)
{
    auto canvas = dynamic_cast<DrawCanvas*>(get_parent_control());
    if(canvas != nullptr)
    {
        auto cam = get_viewport()->get_camera_2d();
        Ref<RSPen> rs_pen = pens.back();
        Array new_data = rs_pen->pen->UpdateDraw(pen_position, 1.0f / cam->get_zoom().x, rs_pen->data);
        if(new_data.size() > 0)
        {
            rs_pen->data = new_data[0];
            int current_index = pens.size() - 1;
            if(new_data.size() > 1)
            {
                rs_pen->data = rs_pen->pen->FinishDraw(rs_pen->data);
                rs_pen->rect = rs_pen->pen->CalculateRect(rs_pen->data);
                for(int i = 1; i < new_data.size(); i++)
                {
                    auto new_pen = rs_pen->pen->duplicate(true);
                    Ref<RSPen> new_rs_pen = memnew(RSPen(new_data[i], new_pen));
                    pens.push_back(new_rs_pen);
                    if(i != new_data.size() - 1)
                    {
                        new_rs_pen->data = new_rs_pen->pen->FinishDraw(new_rs_pen->data);
                        new_rs_pen->rect = new_rs_pen->pen->CalculateRect(new_rs_pen->data);
                        new_rs_pen->Redraw(get_canvas_item(), pens.size() - 1);
                    }
                }
            }
            rs_pen->Redraw(get_canvas_item(), current_index);
        }
    }
}

// Loop through each line and check if it contains our "eraser".
void DrawLayer::UpdateErase(Vector2 pen_position)
{
    auto canvas = dynamic_cast<DrawCanvas*>(get_parent_control());
    if(canvas == nullptr) return;
    DataPens::iterator pen_it = pens.begin();
    while(pen_it != pens.end())
    {
        // Check if the pen position is inside the line bounding rect.
        // Rect2 line_rect = (*pen_it)->rect;
        // bool contains = line_rect.has_point(pen_position);
        // if(!contains)
        // {
        //     Vector2 center = line_rect.get_center();
        //     Vector2 closest = Vector2(
        //         max(center.x - line_rect.size.x / 2.0f, min(pen_position.x, center.x + line_rect.size.x / 2.0f)),
        //         max(center.y - line_rect.size.y / 2.0f, min(pen_position.y, center.y + line_rect.size.y / 2.0f))
        //     );
        //     auto line_pen = dynamic_cast<LinePen*>((*pen_it)->pen.ptr());
        //     float pen_dist = (line_pen == nullptr) ? 0.0f : line_pen->get_width() / 2.0f;
        //     contains = closest.distance_squared_to(pen_position) < powf(canvas->eraser_size + pen_dist, 2.0f);
        // }
        // if(contains)
        // {
        //     bool should_erase = UpdateErase(pen_position, pen_it);
        //     if(should_erase)
        //     {
        //         pen_it = pens.erase(pen_it);
        //         UpdateIndexes(pen_it);
        //         continue;
        //     }
        // }
        bool should_erase = UpdateErase(pen_position, pen_it);
        if(should_erase)
        {
            pen_it = pens.erase(pen_it);
            UpdateIndexes(pen_it);
            continue;
        }
        pen_it++;
    }
}

// Loop through line and split the line where necessary.
bool DrawLayer::UpdateErase(Vector2 pen_position, DataPens::iterator data_it)
{
    auto canvas = dynamic_cast<DrawCanvas*>(get_parent_control());
    if(canvas != nullptr)
    {
        auto new_data = (*data_it)->pen->UpdateErase(pen_position, canvas->eraser_size, (*data_it)->data);
        if(new_data.get_type() == Variant::BOOL)
        {
            return new_data;
        }
        else if(new_data.get_type() == Variant::ARRAY)
        {
            Array data_array = new_data;
            for(int i = 0; i < data_array.size(); i++)
            {
                auto new_pen = (*data_it)->pen->duplicate(true);
                Ref<RSPen> new_rs_line = memnew(RSPen(data_array[i], new_pen));
                new_rs_line->Update(get_canvas_item(), distance(pens.begin(), data_it));
                data_it = pens.insert(data_it, new_rs_line);
            }
            return true;
        }
    }
    return false;
}

void DrawLayer::FinishDraw()
{
    auto rs_pen = pens.back();
    rs_pen->data = rs_pen->pen->FinishDraw(rs_pen->data);
    rs_pen->Redraw(get_canvas_item(), pens.size() - 1);
    emit_signal(UPDATED_SIGNAL);
}

void DrawLayer::FinishErase()
{
    emit_signal(UPDATED_SIGNAL);
}

void DrawLayer::UpdateIndexes(DataPens::iterator data_it)
{
    int index = distance(pens.begin(), data_it);
    while(data_it != pens.end())
    {
        (*data_it)->UpdateIndex(index);
        data_it = next(data_it);
        index++;
    }
}

Array DrawLayer::GetData()
{
    Array data_array;
    for(const auto rs_pen : pens)
    {
        data_array.append(rs_pen->data);
    }
    return data_array;
}

TypedArray<Pen> DrawLayer::GetPens()
{
    TypedArray<Pen> pens_array;
    for(const auto rs_pen : pens)
    {
        pens_array.append(rs_pen->pen);
    }
    return pens_array;
}

void DrawLayer::load_layer_data(Ref<LayerData> p_layer_data)
{
    if(p_layer_data.is_null() || !p_layer_data.is_valid()) return;
    DataPens new_pens = DataPens();
    auto p_data = p_layer_data->get_data();
    auto p_pens = p_layer_data->get_pens();
    for(int i = 0; i < p_data.size(); i++)
    {
        Ref<RSPen> new_rs_line = memnew(RSPen(p_data[i], p_pens[i]));
        new_rs_line->Update(get_canvas_item(), i);
        new_pens.push_back(new_rs_line);
    }
    pens = new_pens;
}

void DrawLayer::scale_data(Vector2 scale)
{
    DataPens::iterator pen_it = pens.begin();
    int index = 0;
    while(pen_it != pens.end())
    {
        (*pen_it)->data = (*pen_it)->pen->ScaleData(scale, (*pen_it)->data);
        (*pen_it)->Redraw(get_canvas_item(), index);
        pen_it++;
        index++;
    }
}

void DrawLayer::offset_data(Vector2 offset)
{
    DataPens::iterator pen_it = pens.begin();
    int index = 0;
    while(pen_it != pens.end())
    {
        (*pen_it)->data = (*pen_it)->pen->OffsetData(offset, (*pen_it)->data);
        (*pen_it)->Redraw(get_canvas_item(), index);
        pen_it++;
        index++;
    }
}
