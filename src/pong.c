#include "pong.h"
#include "raylib.h"



typedef struct {
    Rectangle rect;
    float     vx;
    float     vy;
} Ball;


void pong_init(Ball* b, Rectangle* lpaddle, Rectangle* rpaddle)
{
    InitWindow((int)WIDTH, (int)HEIGHT, TITLE);
    SetTargetFPS(60);

    // init the ball (centered)
    b->rect.x      = (WIDTH / 2.0f) - (BALL_SIZE / 2.0f);
    b->rect.y      = (HEIGHT / 2.0f) - (BALL_SIZE / 2.0f);
    b->rect.width  = BALL_SIZE;
    b->rect.height = BALL_SIZE;
    b->vx          = BALL_SPEED;
    b->vy          = BALL_SPEED;

    // init left paddle
    lpaddle->x      = PADDLE_OFFSET;
    lpaddle->y      = (HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f);
    lpaddle->width  = PADDLE_WIDTH;
    lpaddle->height = PADDLE_HEIGHT;

    // init right paddle
    rpaddle->x      = WIDTH - PADDLE_OFFSET - PADDLE_WIDTH;
    rpaddle->y      = (HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f);
    rpaddle->width  = PADDLE_WIDTH;
    rpaddle->height = PADDLE_HEIGHT;
}


void ball_update(Ball* b)
{
    float dt = GetFrameTime();

    b->rect.x += b->vx * dt;
    b->rect.y += b->vy * dt;

    // bounce off top and bottom walls
    if (b->rect.y <= 0.0f || b->rect.y + b->rect.height >= HEIGHT) { b->vy *= -1.0f; }

    // reset ball if it goes off left or right edge
    if (b->rect.x + b->rect.width < 0 || b->rect.x > WIDTH) {
        b->rect.x = (WIDTH / 2.0f) - (BALL_SIZE / 2.0f);
        b->rect.y = (HEIGHT / 2.0f) - (BALL_SIZE / 2.0f);
        b->vx     = BALL_SPEED * (b->vx > 0 ? -1.0f : 1.0f); // reverse direction
    }
}


void paddle_update(Ball* b, Rectangle* paddle)
{
    // clamp paddles to screen bounds
    if (paddle->y < 0.0f) { paddle->y = 0.0f; }
    if (paddle->y + paddle->height > HEIGHT) { paddle->y = HEIGHT - paddle->height; }

    // handle ball collision with paddles
    if (CheckCollisionRecs(b->rect, *paddle)) {
        if (b->vx < 0) {
            // hit left paddle
            b->vx     = BALL_SPEED;
            b->rect.x = paddle->x + paddle->width;
        } else {
            // hit right paddle
            b->vx     = -BALL_SPEED;
            b->rect.x = paddle->x - b->rect.width;
        }
    }
}


void pong_update(Ball* b, Rectangle* lpaddle, Rectangle* rpaddle)
{
    // left paddle controls (W/S)
    if (IsKeyDown(KEY_W)) { lpaddle->y -= PADDLE_SPEED; }
    if (IsKeyDown(KEY_S)) { lpaddle->y += PADDLE_SPEED; }

    // right paddle controls (I/K)
    if (IsKeyDown(KEY_I)) { rpaddle->y -= PADDLE_SPEED; }
    if (IsKeyDown(KEY_K)) { rpaddle->y += PADDLE_SPEED; }

    // update paddles
    paddle_update(b, lpaddle);
    paddle_update(b, rpaddle);

    // update ball position
    ball_update(b);
}


void pong_draw(Ball* b, Rectangle* lpaddle, Rectangle* rpaddle)
{
    BeginDrawing();
    ClearBackground(BLACK);

    // draw paddles
    DrawRectangleRec(*lpaddle, WHITE);
    DrawRectangleRec(*rpaddle, WHITE);

    // draw ball
    DrawRectangleRec(b->rect, WHITE);

    // draw centre line
    DrawLine((int)WIDTH / 2, 0, (int)WIDTH / 2, HEIGHT, WHITE);

    EndDrawing();
}


void pong_end(void)
{
    CloseWindow();
}


void pong_run(void)
{
    Ball      b;
    Rectangle lpaddle;
    Rectangle rpaddle;

    pong_init(&b, &lpaddle, &rpaddle);

    while (!WindowShouldClose()) {

        pong_update(&b, &lpaddle, &rpaddle);

        pong_draw(&b, &lpaddle, &rpaddle);
    }

    pong_end();
}
