#include "pong.h"
#include "raylib.h"





void ball_reset(Ball* b)
{
    b->pos = (V2int){NUM_CELL_X / 2, NUM_CELL_Y / 2};
    b->v   = (Vector2){BALL_SPEED, BALL_SPEED};
}


void pong_init(Ball* b, Paddle* lp, Paddle* rp)
{
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    // init ball (centre)
    ball_reset(b);

    // init paddles
    lp->pos = (V2int){L_POS_X, NUM_CELL_Y / 2};              // 0 is first cell
    rp->pos = (V2int){R_POS_X, NUM_CELL_Y / 2}; // NUM_CELL_X - 1 is final cell
    lp->score = 0;
    rp->score = 0;
}


void ball_update(Ball* b)
{
    // handle vertical boundry
    if (b->pos.y < 0) {
        b->pos.y *= -1;
    }

    if (b->pos.y >= NUM_CELL_Y) {
        b->pos.y *= -1;
    }
}

void score_check(Ball* b, Paddle* lp, Paddle* rp)
{
    // handle horizontal boundry (scoring)
    if (b->pos.x < 0) { // rp scored
        rp->score++;
        ball_reset(b);
    }
    if (b->pos.x >= NUM_CELL_X) { // lp scored
        lp->score++;
        ball_reset(b);
    }
}



void paddle_update(Paddle* paddle)
{
    // handle clamping
    if (paddle->pos.y < 0) { paddle->pos.y = 0; }
    if (paddle->pos.y >= NUM_CELL_Y) { paddle->pos.y = NUM_CELL_Y - 1; }
}

void paddle_ball_collision(Ball* b, Paddle* p)
{
    // handle collision 
    // make a paddle rect for collition (convert to screen cords)
    Rectangle paddle = (Rectangle){(float)p->pos.x * CELL, (float)p->pos.y * CELL, PADDLE_WIDTH, PADDLE_HEIGHT};
    Vector2 ball = (Vector2){(float)b->pos.x * CELL, (float)b->pos.y * CELL};

    if (CheckCollisionCircleRec(ball, BALL_RADIUS, paddle)) {
        if (p->pos.x == L_POS_X) { // left paddle
            b->v.x *= BALL_SPEED; 
        }
        if (p->pos.x == R_POS_X) { // right paddle
            b->v.x *= -BALL_SPEED;
        }
    }
}

// TODO: where to use dt?

void pong_update(Ball* b, Paddle* lp, Paddle* rp)
{
    // get keys, update paddle positions
    if (IsKeyDown(KEY_W)) { lp->pos.y++; }
    if (IsKeyDown(KEY_S)) { lp->pos.y--; }
    if (IsKeyDown(KEY_I)) { lp->pos.y++; }
    if (IsKeyDown(KEY_K)) { lp->pos.y--; }

    paddle_update(lp);
    paddle_update(rp);

    ball_update(b);

    paddle_ball_collision(b, lp);
    paddle_ball_collision(b, rp);

    score_check(b, lp, rp);
}



void pong_draw(Ball* b, Paddle* lp, Paddle* rp) 
{
    BeginDrawing();
    ClearBackground(BLACK); 
    
    // draw ball
    DrawCircle(b->pos.x * (int)CELL, b->pos.y * (int)CELL, BALL_RADIUS, RED);

    // draw paddles
    DrawRectangle(lp->pos.x * CELL, lp->pos.y * CELL, PADDLE_WIDTH, PADDLE_HEIGHT, RAYWHITE);
    DrawRectangle(rp->pos.x * CELL, rp->pos.y * CELL, PADDLE_WIDTH, PADDLE_HEIGHT, RAYWHITE);

    EndDrawing();
}


void pong_end(void)
{
    CloseWindow();
}


void pong_run(void)
{
    Ball   b;
    Paddle lp;
    Paddle rp;

    pong_init(&b, &lp, &rp);

    while (!WindowShouldClose()) {

        pong_update(&b, &lp, &rp);

        pong_draw(&b, &lp, &rp);
    }

    pong_end();
}


