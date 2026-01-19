#include "pong.h"
#include "raylib.h"
#include <math.h>
#include <time.h>



// TODO: 
// 1. randomise ball reset
// 2. increase speed on each score ? acceleration ?
// 3. handle paddle corner collision
// 4. improve paddle ball collision logic


/*
Turn a vector into a unit direction vector (length = 1), without changing its direction.

Vector length (magnitude):
|v| = sqrt(x² + y²)
Normalized vector:
v̂ = v / |v|
*/
void Vector2Normalize(Vector2* v)
{
    float len = sqrtf((v->x * v->x) + (v->y * v->y));

    if (len == 0.0f) {
        v->x = 0;
        v->y = 0;
    }

    v->x /= len;
    v->y /= len;
}


// Multiply a vector by a scalar (stretch or shrink it).
void Vector2Scale(Vector2* v, float s)
{
    v->x *= s;
    v->y *= s;
}


void random_init(void)
{
    SetRandomSeed((unsigned int)time(NULL));
}

void ball_reset(Ball* ball)
{
    float y = (float) GetRandomValue(0, HEIGHT);
    ball->pos = (Vector2){ WIDTH / 2.f, y};

        
    float vx = (float)GetRandomValue(-100, 100) / 100.0f;
    float vy = (float)GetRandomValue(-100, 100) / 100.0f;

    ball->dir = (Vector2){ vx, vy };     // direction only 
    
    Vector2Normalize(&ball->dir);

    //Vector2Scale(&ball->v, BALL_SPEED); // add speed
}

void pong_init(GameState* game)
{
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    // init ball (center)
    random_init();
    ball_reset(&game->ball);

    // init paddles (with offset = CELL)
    game->lpaddle.pos = (Vector2){ CELL, HEIGHT / 2.f };
    game->rpaddle.pos = (Vector2){ WIDTH - CELL - PADDLE_WIDTH, HEIGHT / 2.f };

    // init metadata
    game->lscore = 0;
    game->rscore = 0;
    game->paused = false;
}


void pong_update(GameState* game)
{
    if (IsKeyPressed(KEY_P)) { game->paused = !game->paused; }
    if (game->paused) { return; }

    Vector2* lp = &game->lpaddle.pos;
    Vector2* rp = &game->rpaddle.pos;
    Ball* b = &game->ball;
    const float dt = GetFrameTime();


    // update paddle pos based on key input
    if (IsKeyDown(KEY_UP))   { lp->y -= PADDLE_SPEED * dt;}
    if (IsKeyDown(KEY_DOWN)) { lp->y += PADDLE_SPEED * dt;}
    if (IsKeyDown(KEY_LEFT)) { rp->y -= PADDLE_SPEED * dt;}
    if (IsKeyDown(KEY_RIGHT)){ rp->y += PADDLE_SPEED * dt;}
    

    // validate paddle pos (clamping)
    // no vertical offset
    if (lp->y < 0) { lp->y = 0; } 
    if (lp->y + PADDLE_HEIGHT > HEIGHT) { lp->y = HEIGHT - PADDLE_HEIGHT; } 
    if (rp->y < 0) { rp->y = 0; } 
    if (rp->y + PADDLE_HEIGHT > HEIGHT) { rp->y = HEIGHT - PADDLE_HEIGHT; } 


    // update ball position based on velocity
    b->pos.x += b->dir.x * BALL_SPEED * dt;
    b->pos.y += b->dir.y * BALL_SPEED * dt;

    //validate ball pos (set boundry up and down)
    if (b->pos.y < 0) {
        b->dir.y *= -1;
    }
    if (b->pos.y > HEIGHT) {
        b->dir.y *= -1;
    }

    // left and right boundry (scoring)
    if (b->pos.x < 0) { // right scored
        ball_reset(b);
        game->rscore++;
    }
    if (b->pos.x > WIDTH) { // left scored
        ball_reset(b);
        game->lscore++;
    }

    // ball-paddle collition check and ball pos update
    // converting paddle into rect
    Rectangle lrect = (Rectangle){lp->x, lp->y, PADDLE_WIDTH, PADDLE_HEIGHT};
    if (CheckCollisionCircleRec(b->pos, BALL_RADIUS, lrect)) {
        b->dir.x *= -1;
    }
    
    Rectangle rrect = (Rectangle){rp->x, rp->y, PADDLE_WIDTH, PADDLE_HEIGHT};
    if (CheckCollisionCircleRec(b->pos, BALL_RADIUS, rrect)) {
        b->dir.x *= -1;
    }
    // When bouncing off a paddle, don’t normalize the reflected vector if you want angle control — normalize after adjusting angle.
}

void pong_draw(GameState* game)
{

    if (game->paused) {
        DrawText("PAUSED", (int)(WIDTH / 2) - 80, (int)(HEIGHT/2) - 20, 40, YELLOW);
    }

    BeginDrawing();
    ClearBackground(BLACK);

    // draw scores
    DrawText(TextFormat("%d", game->lscore), (int)(WIDTH / 4.f), 50, 40, GREEN);
    DrawText(TextFormat("%d", game->rscore), (int)(WIDTH - (WIDTH / 4.f)), 50, 40, GREEN);

    // draw central line
    DrawLine((int)(WIDTH / 2.f), 0, (int)(WIDTH / 2.f), HEIGHT, RAYWHITE);

    // draw paddles
    DrawRectangle((int)game->lpaddle.pos.x, (int)game->lpaddle.pos.y,
                  (int)PADDLE_WIDTH, (int)PADDLE_HEIGHT, RAYWHITE);

    DrawRectangle((int)game->rpaddle.pos.x, (int)game->rpaddle.pos.y,
                  (int)PADDLE_WIDTH, (int)PADDLE_HEIGHT, RAYWHITE);

    // draw ball
    DrawCircle((int)game->ball.pos.x, (int)game->ball.pos.y, BALL_RADIUS, RED);

    EndDrawing();
}

void pong_end(void)
{
    CloseWindow();
}


void pong_run(void)
{
    GameState game = {0};

    pong_init(&game);

    while (!WindowShouldClose()) {

        pong_update(&game);

        pong_draw(&game);
    }

    pong_end();
}


