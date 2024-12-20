#pragma once

static Vector2 getQuadraticBezierPoint(float t, Vector2 p0, Vector2 p1, Vector2 p2) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    return (Vector2){
        uu * p0.x + 2.0f * u * t * p1.x + tt * p2.x,
        uu * p0.y + 2.0f * u * t * p1.y + tt * p2.y
    };
}

static void drawFilledArrowhead(Vector2 end, Vector2 dir, float size, Color color) {
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

void drawDirectedArc(Vector2 start, Vector2 end, Color color) {
    int segments = 50;
    float thickness = 2.5f;
    float arrowSize = 12.0f;
    
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

    Vector2 prev = getQuadraticBezierPoint(0.0f, start, control, end);
    for(int i = 1; i <= segments; i++) {
        float t = (float)i / segments;
        Vector2 current = getQuadraticBezierPoint(t, start, control, end);
        DrawLineEx(prev, current, thickness, color);
        prev = current;
    }

    Vector2 directionToEnd = (Vector2){
        end.x - control.x,
        end.y - control.y
    };
    drawFilledArrowhead(end, directionToEnd, arrowSize, color);
}