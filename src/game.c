#include "game.h"

#include <raylib.h>
#include <stdlib.h>


typedef struct {
    int x;
    int y;
} Player;

Player* players[10];


Player* player_init(int x, int y)
{
    Player* p = malloc(sizeof(Player));
    p->x = x;
    p->y = y;
    return p;
}

// void player_update(Player* p, int x, int y)
// {
// }

void player_destroy(Player* p)
{
    free(p);
}

void game_init(void) 
{
    InitWindow(WIDTH, HEIGHT, TITLE); 

    SetTargetFPS(60);
    
    players[0] = player_init(10, 10);
}

void game_update(void)
{
    Player* p = players[0];
    if (IsKeyPressed(KEY_A)) { p->x--; }
    if (IsKeyPressed(KEY_D)) { p->x++; }
    if (IsKeyPressed(KEY_W)) { p->y++; }
    if (IsKeyPressed(KEY_S)) { p->y--; }
}

void game_draw(void)
{
    Player* p = players[0];
    for (int i = 0; i < NUM_TILE_H; i++) {

        for (int j = 0; j < NUM_TILE_W; j++) {

            if (j / p->x == 0 && i / p->y == 0  ) {
                DrawRectangle(i * TILE_W, j * TILE_H, TILE_W, TILE_H, RED);
            }
        }
    }
}

void game_close(void)
{
    player_destroy(players[0]);

    CloseWindow();
}


void game_run(void)
{
    game_init();
    
    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(BLACK);

        game_update();

        game_draw();

        EndDrawing();
    }

    game_close();
}

