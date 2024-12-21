#pragma once

#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

const int screen_width = 1920;
const int screen_height = 1080;

const double unit = 50.0f;

const double piece_rad = 0.44f * unit;
const double tri_ratio = 0.86602540378f;

const double board_rad = 8.0f * unit;
const double board_pad = (screen_height - 2 * board_rad) / 2;
const struct Vector2 board_center = {screen_width / 2, screen_height / 2};

struct Color tint_black(float opacity) {
    return (struct Color) {0, 0, 0, opacity * 255};
}

const struct Color colors[7] = {
    BROWN, MAROON, BLACK, DARKBLUE, DARKGREEN, WHITE, GOLD
};

struct Vector2 space_pos(int u) {
    struct Vector2 p;
    p.x = board_center.x + unit * (spaces[u].y + 0.5f * spaces[u].x);
    p.y = board_center.y + unit * tri_ratio * spaces[u].x;
    return p;
}

int player_pred[SPACES];

int selected, cursor;

void player_move() {
    cursor = -1;
    for (int i = 0; i < SPACES; i++) {
        if (CheckCollisionPointCircle(GetMousePosition(), space_pos(i), piece_rad)) {
            cursor = i;
            break;
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (cursor == -1) {
            return;
        }

        if (selected == -1) {
            if (!space_mine(cursor)) {
                return;
            }

            selected = cursor;
            reachable_copy(selected, player_pred);
        } else {
            move(selected, cursor);

            selected = -1;
            memset(player_pred, -1, sizeof player_pred);
        }
    }
}

#include "arc.h"

void board_draw() {
    ClearBackground(RAYWHITE);
    DrawCircleV(board_center, board_rad, BEIGE);
    DrawRing(board_center, board_rad - 2, board_rad + 2, 0, 360, 100, BEIGE);

    // Board state
    for (int i = 0; i < SPACES; i++) {
        
        DrawCircleV(space_pos(i), piece_rad, colors[1 + board[i]]);

        if (player_pred[i] != -1) {
            DrawCircleV(space_pos(i), piece_rad / 2, tint_black(0.4));
        }
    }

    // If it's a player's turn and they've selected a piece, hovering will trace a path
    if (player_pred[cursor] != -1) {
        int buf[100];
        int idx = 0;

        int v = cursor;
        while (v != -1) {
            buf[idx++] = v;
            v = player_pred[v];
        }

        for (int i = idx - 2; i >= 0; i--) {
            draw_arc(space_pos(buf[i + 1]), space_pos(buf[i]), tint_black(0.5));

            const int num = idx - 1 - i;
            char s[50];
            sprintf(s, "%d", num);
            DrawText(s, space_pos(buf[i]).x, space_pos(buf[i]).y, 15, MAGENTA);
        }
    }
}

void game_iter();

void game_run() {
    InitWindow(screen_width, screen_height, "Tiaoqi Tools");
    SetTargetFPS(60);

    memset(player_pred, -1, sizeof player_pred);
    selected = -1;

    while (!WindowShouldClose()) {
        game_iter();

        BeginDrawing();
        
        board_draw();

        EndDrawing();
    }

    CloseWindow();
}