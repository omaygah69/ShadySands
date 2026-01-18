#include "raylib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define CELL_SIZE 5
#define MAX_GRID_CELLSX 256
#define MAX_GRID_CELLSY 144

typedef uint64_t u64;
typedef int32_t i32;
typedef i32 b32;

typedef enum
{
    EMPTY,
    SAND,
    WATER,
    BLOCK
} cell_type;

typedef struct
{
    cell_type type;
    Color color;
} particle_t;

void UpdateParticles();
void DrawGridLines(b32 y);
particle_t Grid[MAX_GRID_CELLSX][MAX_GRID_CELLSY] = {0};

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    /* particle_t cellurmom = { */
    /*     .x = 0, */
    /*     .y = 0, */
    /*     .color = ORANGE, */
    /* }; */
    
    InitWindow(screenWidth, screenHeight, "ncr");
    cell_type spawn_type = SAND;

    Vector2 mousePosition;
    i32 mouseX;
    i32 mouseY;
    i32 mouseCellX;
    i32 mouseCellY;

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        /* float dt = GetFrameTime(); */
        mousePosition = GetMousePosition();
        mouseX = (i32)mousePosition.x;
        mouseY = (i32)mousePosition.y;
        if(mouseX >= 0 && mouseY >= 0){
            mouseCellX = mouseX / CELL_SIZE;
            mouseCellY = mouseY / CELL_SIZE;
            if(mouseCellX>MAX_GRID_CELLSX-1 || mouseCellY>MAX_GRID_CELLSY-1){
                mouseCellX = -1;
                mouseCellY = -1;
            }
        }
        
        /* if(IsKeyPressed(KEY_W)) cellurmom.y--; */
        /* if(IsKeyPressed(KEY_A)) cellurmom.x--; */
        /* if(IsKeyPressed(KEY_S)) cellurmom.y++; */
        /* if(IsKeyPressed(KEY_D)) cellurmom.x++; */

        DrawGridLines(false);
        
        if (mouseCellX >= 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            if(Grid[mouseCellX][mouseCellY].type == EMPTY)
            {
                switch(spawn_type)
                {
                case SAND:
                    Grid[mouseCellX][mouseCellY].type = SAND;
                    Grid[mouseCellX][mouseCellY].color = ORANGE;
                    break;
                case BLOCK:
                    Grid[mouseCellX][mouseCellY].type = SAND;
                    Grid[mouseCellX][mouseCellY].color = GRAY;
                    break;
                dedfault:
                    break;
                }
            }
        }

        UpdateParticles();
        for (i32 y = 0; y < MAX_GRID_CELLSY; y++)
        {
            for (i32 x = 0; x < MAX_GRID_CELLSX; x++)
            {
                if (Grid[x][y].type != EMPTY)
                {
                    DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE,Grid[x][y].color);
                }
            }
        }

        DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void UpdateParticles()
{
    for(i32 y = MAX_GRID_CELLSY - 2;y >= 0; y--)
    {
        for(i32 x = 0; x < MAX_GRID_CELLSX; x++)
        {
            if(Grid[x][y].type != SAND)
                continue;

            if(Grid[x][y+1].type == EMPTY)
            {
                Grid[x][y+1] = Grid[x][y];
                Grid[x][y].type = EMPTY;
                continue;
            }

            bool leftFirst = GetRandomValue(0, 1);

            if (leftFirst)
            {
                if (x > 0 && Grid[x - 1][y + 1].type == EMPTY) {
                    Grid[x - 1][y + 1] = Grid[x][y];
                    Grid[x][y].type = EMPTY;
                    continue;
                }
                else if(x < MAX_GRID_CELLSX - 1 && Grid[x + 1][y + 1].type == EMPTY) {
                    Grid[x + 1][y + 1] = Grid[x][y];
                    Grid[x][y].type = EMPTY;
                    continue;
                }
            }
            else
            {
                if (x < MAX_GRID_CELLSX - 1 && Grid[x + 1][y + 1].type == EMPTY) {
                    Grid[x + 1][y + 1] = Grid[x][y];
                    Grid[x][y].type = EMPTY;
                    continue;
                }
                else if (x > 0 && Grid[x - 1][y + 1].type == EMPTY) {
                    Grid[x - 1][y + 1] = Grid[x][y];
                    Grid[x][y].type = EMPTY;
                    continue;
                }
            }
        }
    }
}

void DrawGridLines(b32 y)
{
    if(!y) return;
    for (int y = 0; y <= MAX_GRID_CELLSY; y++)
        DrawLine( 0, y*CELL_SIZE,MAX_GRID_CELLSX*CELL_SIZE, y*CELL_SIZE, LIGHTGRAY);
    for (int x = 0; x <= MAX_GRID_CELLSX; x++)
        DrawLine(x*CELL_SIZE, 0, x*CELL_SIZE, MAX_GRID_CELLSY*CELL_SIZE, LIGHTGRAY);
}
