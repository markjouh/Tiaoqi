#pragma once

struct Vector2 quadratic_bezier(float t, Vector2 p0, Vector2 p1, Vector2 p2) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    return (Vector2){
        uu * p0.x + 2.0f * u * t * p1.x + tt * p2.x,
        uu * p0.y + 2.0f * u * t * p1.y + tt * p2.y
    };
}

void draw_arrowhead(Vector2 end, Vector2 dir, float size, Color color) {
    float angle = atan2f(dir.y, dir.x);
    Vector2 p1 = end;
    Vector2 p2 = (Vector2){
        end.x - size * cosf(angle + DEG2RAD * 25),
        end.y - size * sinf(angle + DEG2RAD * 25)
    };
    Vector2 p3 = (Vector2){
        end.x - size * cosf(angle - DEG2RAD * 25),
        end.y - size * sinf(angle - DEG2RAD * 25)
    };
    DrawTriangle(p1, p2, p3, color);
}

void draw_arc(Vector2 start, Vector2 end, Color color) {
    int segments = 50;
    float line_width = 2.5f;
    float arrow_size = 12.0f;
    
    Vector2 direction = (Vector2){
        end.x - start.x,
        end.y - start.y
    };

    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    Vector2 unitDir = (Vector2){
        direction.x / length,
        direction.y / length
    };

    float dist = sqrtf(pow(start.x - end.x, 2) + pow(start.y - end.y, 2));
    Vector2 control = (Vector2){
        (start.x + end.x) / 2.0f,
        ((start.y + end.y) / 2.0f) - dist
    };

    Vector2 prev = quadratic_bezier(0.0f, start, control, end);
    for(int i = 1; i <= segments; i++) {
        float t = (float)i / segments;
        Vector2 current = quadratic_bezier(t, start, control, end);
        DrawLineEx(prev, current, line_width, color);
        prev = current;
    }

    Vector2 head_angle = (Vector2){
        end.x - control.x,
        end.y - control.y
    };
    draw_arrowhead(end, head_angle, arrow_size, color);
}