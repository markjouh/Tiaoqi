#pragma once

#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const int screenWidth = 1024;
const int screenHeight = 768;

const double displayUnit = 40.0f;
const double pieceRadius = 0.44f * displayUnit;
const double boardRadius = 8.0f * displayUnit;
const struct Vector2 boardCenter = {0.35f * screenWidth, 0.5f * screenHeight};

const double triangleHeight = 0.86602540378f;

const struct Color darkTint = {0, 0, 0, 100};

const struct Color colors[7] = {
    BROWN, MAROON, BLACK, DARKBLUE, DARKGREEN, WHITE, GOLD
};

struct Vector2 getPos(int row, int col) {
    struct Vector2 p;
    p.x = boardCenter.x + displayUnit * (col + 0.5f * row);
    p.y = boardCenter.y + displayUnit * triangleHeight * row;
    return p;
}

bool playerMoves[numSpaces];

void playerMove() {
    static int cursor = -1;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int selected = -1;
        for (int i = 0; i < numSpaces; i++) {
            if (CheckCollisionPointCircle(GetMousePosition(), getPos(spaces[i].x, spaces[i].y), pieceRadius)) {
                selected = i;
                break;
            }
        }

        if (selected == -1) {
            return;
        }

        if (cursor == -1) {
            if (!chkMine(selected)) {
                return;
            }

            cursor = selected;
            calcReachable(cursor, playerMoves);
        } else {
            makeMove(cursor, selected);

            cursor = -1;
            memset(playerMoves, 0, sizeof playerMoves);
        }
    }
}

void drawBoard() {
    ClearBackground(RAYWHITE);
    DrawCircleV(boardCenter, boardRadius, BEIGE);

    for (int i = 0; i < numSpaces; i++) {
        const struct Vector2 p = getPos(spaces[i].x, spaces[i].y);

        DrawCircleV(p, pieceRadius, colors[1 + board[i]]);

        if (playerMoves[i]) {
            DrawCircleV(p, pieceRadius / 2, darkTint);
        }
    }
}

void gameLoop();

void runGame() {
    InitWindow(screenWidth, screenHeight, "Tiaoqi Tools");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        gameLoop();

        BeginDrawing();
        
        drawBoard();

        EndDrawing();
    }

    CloseWindow();
}