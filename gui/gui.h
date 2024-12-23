#pragma once

#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include <array>

constexpr int SCREEN_W = 1920;
constexpr int SCREEN_H = 1080;

constexpr double UNIT = 50.0f;

constexpr double PIECE_RAD = 0.44f * UNIT;
constexpr double TRI_RATIO = 0.86602540378f;

constexpr double BOARD_RAD = 8.0f * UNIT;
constexpr double BOARD_PAD = (SCREEN_H - 2 * BOARD_RAD) / 2;
constexpr struct Vector2 BOARD_CENTER = {SCREEN_W / 2, SCREEN_H / 2};

constexpr Color colors[7] = { BROWN, MAROON, BLACK, DARKBLUE, DARKGREEN, WHITE, GOLD };
constexpr Color tint_black(float opacity) { return (struct Color) {0, 0, 0, opacity * 255}; }

constexpr Vector2 space_pos(int u) {
    return {
        BOARD_CENTER.x + UNIT * (spaces[u].y + 0.5f * spaces[u].x),
        BOARD_CENTER.y + UNIT * TRI_RATIO * spaces[u].x
    };
}

#include "arc.h"

#include <atomic>
#include <iostream>
#include <future>
#include <chrono>
#include <thread>

std::atomic<int> cursor, selected;
std::atomic<bool> lmb_down, lmb_used;

std::atomic<std::array<int, SPACES>> player_pred;

void gui_draw(Board &board) {
    ClearBackground(RAYWHITE);
    DrawCircleV(BOARD_CENTER, BOARD_RAD, BEIGE);
    DrawRing(BOARD_CENTER, BOARD_RAD - 2, BOARD_RAD + 2, 0, 360, 100, BEIGE);
    
    const int cur = cursor.load();
    const auto pred = player_pred.load();

    // Draw the board
    for (int i = 0; i < SPACES; i++) {
        DrawCircleV(space_pos(i), PIECE_RAD, colors[1 + board[i]]);
    }
    
    // For GUI moves: highlight reachable spaces
    for (int i = 0; i < SPACES; i++) {
        if (pred[i] != -1) {
            DrawCircleV(space_pos(i), PIECE_RAD / 2, tint_black(0.4));
        }
    }
    
    // For GUI moves: trace path when hovering over a reachable space
    if (pred[cur] != -1) {
        int buf[100];
        int idx = 0;

        int v = cur;
        while (v != -1) {
            buf[idx++] = v;
            v = pred[v];
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

#include "../game/game.h"

std::pair<int, int> gui_move(InputState s) {
    while (true) {
        const int cur = cursor.load(), sel = selected.load();
        
        if (lmb_down.load() && !lmb_used.load()) {
            lmb_used.store(true);
            
            if (cur == -1) {
                continue;
            }

            if (sel == -1) {
                if (s.board[cur] != s.color) {
                    continue;
                }

                selected.store(cur);
                player_pred.store(s.board.get_moves(cur));
            } else {
                if (cur == sel) {
                    continue;
                }
                
                const bool cond = player_pred.load()[cur] != -1;
                
                selected.store(-1);
                std::array<int, SPACES> buf;
                std::fill(begin(buf), end(buf), -1);
                player_pred.store(buf);
                
                if (cond) {
                    return {sel, cur};
                }
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

void gui_run(Game &game) {
    InitWindow(SCREEN_W, SCREEN_H, "Tiaoqi Tools");
    SetTargetFPS(60);
    
    selected.store(-1);
    std::array<int, SPACES> buf;
    std::fill(begin(buf), end(buf), -1);
    player_pred.store(buf);
    
    std::thread worker(game_run, std::ref(game));

    while (!WindowShouldClose()) {
        int tmp_cursor = -1;
        for (int i = 0; i < SPACES; i++) {
            if (CheckCollisionPointCircle(GetMousePosition(), space_pos(i), PIECE_RAD)) {
                tmp_cursor = i;
                break;
            }
        }
        cursor.store(tmp_cursor);
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            lmb_down.store(true);
        } else {
            lmb_down.store(false);
            lmb_used.store(false);
        }
        
        BeginDrawing();
        
        gui_draw(game.board);
        
        EndDrawing();
    }

    CloseWindow();
}