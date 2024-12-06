#include "draw_canvas.hpp"

using namespace JustDraw;
using namespace godot;

void DrawCanvas::_bind_methods()
{
    #pragma region Getters and Setters

    ClassDB::bind_method(D_METHOD("set_color", "p_color"), &DrawCanvas::set_color);
    ClassDB::bind_method(D_METHOD("get_color"), &DrawCanvas::get_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "pen_color"), "set_color", "get_color");

	ClassDB::bind_method(D_METHOD("set_line_width", "p_width"), &DrawCanvas::set_line_width);
    ClassDB::bind_method(D_METHOD("get_line_width"), &DrawCanvas::get_line_width);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "line_width"), "set_line_width", "get_line_width");

	ClassDB::bind_method(D_METHOD("set_cap_scale", "p_width"), &DrawCanvas::set_cap_scale);
    ClassDB::bind_method(D_METHOD("get_cap_scale"), &DrawCanvas::get_cap_scale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cap_scale"), "set_cap_scale", "get_cap_scale");

    ClassDB::bind_method(D_METHOD("set_eraser_size", "p_width"), &DrawCanvas::set_eraser_size);
    ClassDB::bind_method(D_METHOD("get_eraser_size"), &DrawCanvas::get_eraser_size);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "eraser_size"), "set_eraser_size", "get_eraser_size");

    ClassDB::bind_method(D_METHOD("set_min_draw_distance", "p_distance"), &DrawCanvas::set_min_draw_distance);
    ClassDB::bind_method(D_METHOD("get_min_draw_distance"), &DrawCanvas::get_min_draw_distance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "minimum_draw_distance"), "set_min_draw_distance", "get_min_draw_distance");

    ClassDB::bind_method(D_METHOD("set_max_draw_angle", "p_angle"), &DrawCanvas::set_max_draw_angle);
    ClassDB::bind_method(D_METHOD("get_max_draw_angle"), &DrawCanvas::get_max_draw_angle);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "maximum_draw_angle"), "set_max_draw_angle", "get_max_draw_angle");

    ClassDB::bind_method(D_METHOD("set_smooth_steps", "p_steps"), &DrawCanvas::set_smooth_steps);
    ClassDB::bind_method(D_METHOD("get_smooth_steps"), &DrawCanvas::get_smooth_steps);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "smooth_steps"), "set_smooth_steps", "get_smooth_steps");

    ClassDB::bind_method(D_METHOD("set_smooth_ratio", "p_ratio"), &DrawCanvas::set_smooth_ratio);
    ClassDB::bind_method(D_METHOD("get_smooth_ratio"), &DrawCanvas::get_smooth_ratio);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "smooth_ratio"), "set_smooth_ratio", "get_smooth_ratio");

    ClassDB::bind_method(D_METHOD("set_smooth_min_distance", "p_distance"), &DrawCanvas::set_smooth_min_distance);
    ClassDB::bind_method(D_METHOD("get_smooth_min_distance"), &DrawCanvas::get_smooth_min_distance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "smooth_min_distance"), "set_smooth_min_distance", "get_smooth_min_distance");
    
    #pragma endregion
}

DrawCanvas::DrawCanvas()
{
    set_mouse_filter(MOUSE_FILTER_PASS);
    set_default_cursor_shape(CURSOR_CROSS);
}

DrawCanvas::~DrawCanvas() {}

Line DrawCanvas::SmoothLineStep(Line line, int smooth_start)
{
    // Clamp ratio, then minus one so it doesn't go past midpoint.
    float ratio = fmaxf(0.0, fminf(1.0, smooth_ratio));
    if (ratio > 0.5f) ratio = 1.0f - ratio;
    auto smoothed_line = Line();
    smoothed_line.push_back(line[0]); // Add first point to new line.
    for (int i = 1; i < line.size() - 1; i++)
    {
        if (i < smooth_start)
        {
            smoothed_line.push_back(line[i]);
            continue;
        }
        Vector2 prev = line[i - 1], curr = line[i], next = line[i + 1];
        float dist_squared = prev.distance_squared_to(curr);
        if(dist_squared < powf(smooth_min_distance, 2.0f))
        {
            smoothed_line.push_back(curr);
            continue;
        }
        smoothed_line.push_back(curr.lerp(prev, ratio));
        smoothed_line.push_back(curr.lerp(next, ratio));
    }
    smoothed_line.push_back(line[line.size() - 1]); // Add last point to new line.
    return smoothed_line;
}

Line DrawCanvas::SmoothLine(Line line, int smooth_start)
{
    for (int i = 0; i < smooth_steps; i++)
    {
        line = SmoothLineStep(line, smooth_start);
    }
    return line;
}
