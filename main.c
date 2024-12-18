#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <raylib.h>

#include "game/board.h"
#include "game/reachable.h"

const int players = 2;
const int playerColors[players] = {0, 3};

int turn = 0;
bool over = false;

int myColor() {
    return playerColors[turn % players];
}

bool myPiece(int pos) {
    return board[pos] == myColor();
}

bool validMove(int a, int b) {
    if (a < 0 || a >= numSpaces || b < 0 || b >= numSpaces || a == b || !myPiece(a)) {
        return false;
    }

    clearReachable();
    findReachable(a);

    const bool ok = reachable[b];

    clearReachable();

    return ok;
}

bool makeMove(int a, int b) {
    if (!validMove(a, b)) {
        return false;
    }

    board[b] = board[a];
    board[a] = -1;

    turn++;

    return true;
}

#include "bots/randomBot.h"
#include "bots/basicBot.h"

void playerMove();

void gameLoop() {
    if (turn % 2) {
        basicMove();
    } else {
        playerMove();
    }

    int scores[6];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 10; j++) {
            scores[i] += board[corners[(i + 3) % 6][j]] == i;
        }
        if (scores[i] == 10) {
            over = true;
        }
    }
}

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

int cursor = -1;

void playerMove() {
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
            if (!myPiece(selected)) {
                return;
            }

            cursor = selected;
            clearReachable();
            findReachable(cursor);
        } else {
            makeMove(cursor, selected);

            cursor = -1;
            clearReachable();
        }
    }
}

void drawBoard() {
    gameLoop();

    ClearBackground(RAYWHITE);
    DrawCircleV(boardCenter, boardRadius, BEIGE);

    for (int i = 0; i < numSpaces; i++) {
        const struct Vector2 p = getPos(spaces[i].x, spaces[i].y);

        DrawCircleV(p, pieceRadius, colors[1 + board[i]]);

        if (i != cursor && reachable[i]) {
            DrawCircleV(p, pieceRadius / 2, darkTint);
        }
    }
}

int main(void) {
    InitWindow(screenWidth, screenHeight, "Tiaoqi Tools");
    SetTargetFPS(60);

    initBoard();

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        drawBoard();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}