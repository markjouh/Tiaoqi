#pragma once

#include <raylib.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

const int screenWidth = 1024;
const int screenHeight = 768;

const double displayUnit = 40.0f;
const double pieceRadius = 0.44f * displayUnit;
const double boardRadius = 8.0f * displayUnit;
const struct Vector2 boardCenter = {0.35f * screenWidth, 0.5f * screenHeight};

const double triangleHeight = 0.86602540378f;

const struct Color darkTint = {0, 0, 0, 90};

const struct Color colors[7] = {
    BROWN, MAROON, BLACK, DARKBLUE, DARKGREEN, WHITE, GOLD
};

struct Vector2 getPos(int u) {
    struct Vector2 p;
    p.x = boardCenter.x + displayUnit * (spaces[u].y + 0.5f * spaces[u].x);
    p.y = boardCenter.y + displayUnit * triangleHeight * spaces[u].x;
    return p;
}

int playerFrom[numSpaces];

int selected, cursor;

void playerMove() {
    cursor = -1;
    for (int i = 0; i < numSpaces; i++) {
        if (CheckCollisionPointCircle(GetMousePosition(), getPos(i), pieceRadius)) {
            cursor = i;
            break;
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (cursor == -1) {
            return;
        }

        if (selected == -1) {
            if (!chkMine(cursor)) {
                return;
            }

            selected = cursor;
            calcReachable(selected, playerFrom);
        } else {
            makeMove(selected, cursor);

            selected = -1;
            memset(playerFrom, -1, sizeof playerFrom);
        }
    }
}

#include "drawArc.h"

void drawBoard() {
    ClearBackground(RAYWHITE);
    DrawCircleV(boardCenter, boardRadius, BEIGE);

    // Board state
    for (int i = 0; i < numSpaces; i++) {
        DrawCircleV(getPos(i), pieceRadius, colors[1 + board[i]]);

        if (playerFrom[i] != -1) {
            DrawCircleV(getPos(i), pieceRadius / 2, darkTint);
        }
    }

    // If it's a player's turn and they've selected a piece, hovering will trace a path
    if (playerFrom[cursor] != -1) {
        int buf[100];
        int idx = 0;

        int v = cursor;
        while (v != -1) {
            buf[idx++] = v;
            v = playerFrom[v];
        }

        for (int i = idx - 2; i >= 0; i--) {
            drawDirectedArc(getPos(buf[i + 1]), getPos(buf[i]));

            const int num = idx - 1 - i;
            char s[50];
            sprintf(s, "%d", num);
            DrawText(s, getPos(buf[i]).x, getPos(buf[i]).y, 15, MAGENTA);
        }
    }
}

void gameLoop();

void runGame() {
    InitWindow(screenWidth, screenHeight, "Tiaoqi Tools");
    SetTargetFPS(60);

    memset(playerFrom, -1, sizeof playerFrom);
    selected = -1;

    while (!WindowShouldClose()) {
        gameLoop();

        BeginDrawing();
        
        drawBoard();

        EndDrawing();
    }

    CloseWindow();
}