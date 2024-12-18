#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <raylib.h>

struct Coord {
    int x, y;
};

#include "board.h"

int board[width][width];

void initBoard() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            board[i][j] = -2;
        }
    }

    for (int i = 0; i < numCenter; i++) {
        board[center[i].x][center[i].y] = -1;
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 10; j++) {
            board[corners[i][j].x][corners[i][j].y] = i;
        }
    }
}

#include "reachable.h"

const int players = 2;
const int playerColors[players] = {0, 3};

int turn = 0;

bool myPiece(int pos) {
    return board[spaces[pos].x][spaces[pos].y] == playerColors[turn % players];
}

void cpuTurn() {
    int ord[numSpaces];
    for (int i = 0; i < numSpaces; i++) {
        ord[i] = i;
    }
    srand(time(0));
    for (int i = 1; i < numSpaces; i++) {
        int j = rand() % i;
        int temp = ord[i];
        ord[i] = ord[j];
        ord[j] = temp;
    }

    for (int i = 0; i < numSpaces; i++) {
        if (myPiece(ord[i])) {
            clearReachable();
            findReachable(ord[i]);

            for (int j = 0; j < numSpaces; j++) {
                if (ord[i] != ord[j] && reachable[spaces[ord[j]].x][spaces[ord[j]].y]) {
                    board[spaces[ord[j]].x][spaces[ord[j]].y] = board[spaces[ord[i]].x][spaces[ord[i]].y];
                    board[spaces[ord[i]].x][spaces[ord[i]].y] = -1;

                    clearReachable();
                    turn++;

                    return;
                }
            }
        }
    }
}

const int screenWidth = 1024;
const int screenHeight = 768;

const double displayUnit = 40.0f;
const double pieceRadius = 0.44f * displayUnit;
const double boardRadius = 8.0f * displayUnit;

const double triangleHeight = 0.86602540378f;

const struct Color darkTint = {0, 0, 0, 100};

const struct Color colors[7] = {
    BROWN, MAROON, BLACK, DARKBLUE, DARKGREEN, WHITE, GOLD
};

struct Vector2 getPos(int row, int col) {
    row -= offset;
    col -= offset;

    struct Vector2 p;
    p.x = boardRadius + displayUnit * (col + 0.5f * row);
    p.y = boardRadius + displayUnit * triangleHeight * row;
    return p;
}

int cursor = -1;

void playerTurn() {
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
            if (!checkFree(spaces[selected].x, spaces[selected].y)) {
                return;
            }
            
            board[spaces[selected].x][spaces[selected].y] = board[spaces[cursor].x][spaces[cursor].y];
            board[spaces[cursor].x][spaces[cursor].y] = -1;

            cursor = -1;
            clearReachable();

            turn++;
        }
    }
}

void drawBoard() {
    ClearBackground(RAYWHITE);

    if (turn % 2) {
        playerTurn();
    } else {
        cpuTurn();
    }

    const struct Vector2 boardCenter = {boardRadius, boardRadius};
    DrawCircleV(boardCenter, boardRadius, BEIGE);

    for (int i = 0; i < numSpaces; i++) {
        const struct Vector2 p = getPos(spaces[i].x, spaces[i].y);

        DrawCircleV(p, pieceRadius, colors[1 + board[spaces[i].x][spaces[i].y]]);

        if (i != cursor && reachable[spaces[i].x][spaces[i].y]) {
            DrawCircleV(p, 5, darkTint);
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