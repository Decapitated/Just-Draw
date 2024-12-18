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
    ClassDB::bind_method(D_METHOD("scale_lines", "p_scale"), &DrawLayer::scale_lines);
    ClassDB::bind_method(D_METHOD("offset_lines", "p_offset"), &DrawLayer::offset_lines);

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
        auto new_line = Line();
        new_line.append(pen_position);
        auto new_pen = canvas->pen->duplicate(true);
        auto new_rs_line = shared_ptr<RSPen>(new RSPen(new_line, new_pen));
        new_rs_line->Update(get_canvas_item(), lines.size());
        // Add the line to the list.
        lines.push_back(new_rs_line);
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
        const float min_dist = canvas->min_draw_distance * (1.0f / cam->get_zoom().x);
        shared_ptr<RSPen> rs_line = lines.back();
        auto prev_pos = rs_line->line[rs_line->line.size() - 1];
        auto dist = prev_pos.distance_squared_to(pen_position);
        if(dist >= powf(min_dist, 2.0))
        {
            // Clamp the angle between the line and the pen.
            if(rs_line->line.size() >= 2)
            {
                Vector2 prev = rs_line->line[rs_line->line.size() - 2], curr = prev_pos, next = pen_position;
                float curr_dot = (curr - prev).normalized().dot((next - curr).normalized());
                curr_dot = (1.0f - (curr_dot * 0.5f + 0.5f)) * 180.0f;
                if(curr_dot > canvas->max_draw_angle)
                {
                    FinishDraw();
                    auto new_line = Line();
                    new_line.append(rs_line->line[rs_line->line.size() - 1]);
                    new_line.append(next);
                    auto new_pen = canvas->pen->duplicate(true);
                    auto new_rs_line = shared_ptr<RSPen>(new RSPen(new_line, new_pen));
                    new_rs_line->Update(get_canvas_item(), lines.size());
                    // Add the line to the list.
                    lines.push_back(new_rs_line);
                    return;
                }
            }
            rs_line->line.append(pen_position);
            rs_line->rect.expand_to(pen_position);
            rs_line->Redraw(get_canvas_item(), lines.size() - 1);
        }
    }
}

void DrawLayer::UpdateErase(Vector2 pen_position)
{
    auto canvas = dynamic_cast<DrawCanvas*>(get_parent_control());
    if(canvas == nullptr) return;
    LineIterator line_it = lines.begin();
    while(line_it != lines.end())
    {
        // Check if the pen position is inside the line bounding rect.
        Rect2 line_rect = (*line_it)->rect;
        bool contains = line_rect.has_point(pen_position);
        if(!contains)
        {
            Vector2 center = line_rect.get_center();
            Vector2 closest = Vector2(
                max(center.x - line_rect.size.x / 2.0f, min(pen_position.x, center.x + line_rect.size.x / 2.0f)),
                max(center.y - line_rect.size.y / 2.0f, min(pen_position.y, center.y + line_rect.size.y / 2.0f))
            );
            auto line_pen = dynamic_cast<LinePen*>((*line_it)->pen.ptr());
            float pen_dist = (line_pen == nullptr) ? 0.0f : line_pen->get_width() / 2.0f;
            contains = closest.distance_squared_to(pen_position) < powf(canvas->eraser_size + pen_dist, 2.0f);
        }
        if(contains)
        {
            bool should_erase = UpdateErase(pen_position, line_it);
            if(should_erase)
            {
                line_it = lines.erase(line_it);
                UpdateIndexes(line_it);
                continue;
            }
        }
        line_it++;
    }
}

bool DrawLayer::UpdateErase(Vector2 pen_position, LineIterator line_it)
{
    bool sliced = false;
    auto canvas = dynamic_cast<DrawCanvas*>(get_parent_control());
    if(canvas != nullptr)
    {
        int slice_start = 0;
        bool slicing = false;
        for(int i = 0; i < (*line_it)->line.size(); i++)
        {
            auto curr = (*line_it)->line[i];
            bool is_last_slice = i == (*line_it)->line.size() - 1 && slice_start > 0;
            auto line_pen = dynamic_cast<LinePen*>((*line_it)->pen.ptr());
            float pen_dist = (line_pen == nullptr) ? 0.0f : line_pen->get_width() / 2.0f;
            if(is_last_slice || (curr.distance_squared_to(pen_position) < powf(canvas->eraser_size + pen_dist, 2.0f)))
            {
                if(slicing)
                {
                    int end = is_last_slice ? i + 1 : i;
                    auto new_line = (*line_it)->line.slice(slice_start, end);
                    auto new_pen = (*line_it)->pen->duplicate(true);
                    auto new_rs_line = shared_ptr<RSPen>(new RSPen(new_line, new_pen));
                    new_rs_line->Update(get_canvas_item(), distance(lines.begin(), line_it));
                    auto new_line_it = lines.insert(line_it, new_rs_line);

                    slicing = false;
                }
                sliced = true;
            }
            else if(!slicing)
            {
                slice_start = i;
                slicing = true;
            }
        }
    }
    return sliced || (*line_it)->line.size() == 0;
}

void DrawLayer::FinishDraw()
{
    auto rs_line = lines.back();
    rs_line->line = rs_line->pen->FinishDraw(rs_line->line);
    rs_line->Redraw(get_canvas_item(), lines.size() - 1);
    emit_signal(UPDATED_SIGNAL);
}

void DrawLayer::FinishErase()
{
    emit_signal(UPDATED_SIGNAL);
}

void DrawLayer::UpdateIndexes(LineIterator line_it)
{
    int index = distance(lines.begin(), line_it);
    while(line_it != lines.end())
    {
        (*line_it)->UpdateIndex(index);
        line_it = next(line_it);
        index++;
    }
}

TypedArray<Line> DrawLayer::GetLines()
{
    TypedArray<Line> lines_array;
    for(const auto rs_line : lines)
    {
        lines_array.append(rs_line->line);
    }
    return lines_array;
}

TypedArray<Pen> DrawLayer::GetPens()
{
    TypedArray<Pen> pens_array;
    for(const auto rs_line : lines)
    {
        pens_array.append(rs_line->pen);
    }
    return pens_array;
}

void DrawLayer::load_layer_data(Ref<LayerData> p_layer_data)
{
    if(p_layer_data.is_null() || !p_layer_data.is_valid()) return;
    Lines new_lines = Lines();
    auto p_lines = p_layer_data->get_lines();
    auto p_pens = p_layer_data->get_pens();
    for(int i = 0; i < p_lines.size(); i++)
    {
        auto new_rs_line = shared_ptr<RSPen>(new RSPen(p_lines[i], p_pens[i]));
        new_rs_line->Update(get_canvas_item(), i);
        new_lines.push_back(new_rs_line);
    }
    lines = new_lines;
}

void DrawLayer::scale_lines(Vector2 scale)
{
    LineIterator line_it = lines.begin();
    int index = 0;
    while(line_it != lines.end())
    {
        for(int i = 0; i < (*line_it)->line.size(); i++)
        {
            (*line_it)->line[i] *= scale;
        }
        (*line_it)->Redraw(get_canvas_item(), index);
        line_it++;
        index++;
    }
}

void DrawLayer::offset_lines(Vector2 offset)
{
    LineIterator line_it = lines.begin();
    int index = 0;
    while(line_it != lines.end())
    {
        for(int i = 0; i < (*line_it)->line.size(); i++)
        {
            (*line_it)->line[i] += offset;
        }
        (*line_it)->Redraw(get_canvas_item(), index);
        line_it++;
        index++;
    }
}
