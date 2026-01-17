#include "pong.h"
#include "raylib.h"


#define PADDLE_SIZE_W ((float)(1 * TILE_W))
#define PADDLE_SIZE_H ((float)(10 * TILE_H))


typedef struct {
    float x;
    float y;
    float vx;
    float vy;
} Ball;



void pong_init(Ball* b, Rectangle* lpaddle, Rectangle* rpaddle)
{
    InitWindow(WIDTH, HEIGHT, TITLE);

    SetTargetFPS(60);


    // init the ball
    b->x = WIDTH / 2;
    b->y = HEIGHT / 2;
    b->vx = 1;
    b->vy = 1;

    // init the paddles
    lpaddle->x = 0;
    lpaddle->y = (float) NUM_TILES_Y / 2;
    lpaddle->width = PADDLE_SIZE_W;
    lpaddle->height = PADDLE_SIZE_H;

    rpaddle->x = NUM_TILES_X;
    rpaddle->y = (float) NUM_TILES_Y / 2;
    rpaddle->width = PADDLE_SIZE_W;
    rpaddle->height = PADDLE_SIZE_H;
}



void ball_update(Ball* b, float x, float y)
{
    b->x += x;
    b->y += y;
}

// void paddle_update(Rectangle* p, float y)
// {
//     p->y += y;
// }

void pong_update(Ball* b, Rectangle* lpaddle, Rectangle* rpaddle)
{
    // if A is being pressed
    if (IsKeyDown(KEY_W)) { lpaddle->y++; }

}




