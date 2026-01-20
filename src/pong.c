#include "pong.h"
#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <time.h>



// TODO:
// 3. handle paddle corner collision
// 4. improve paddle ball collision logic 
//      (center increases speed while side decreases it)
// 5. when ball/paddle collision, want the ball to "bounce" 
//      off the paddle (like top/bottom walls)




void random_init(void);
void ball_reset(Ball* ball);
void Vector2Normalize(Vector2* v);
void Vector2Scale(Vector2* v, float s);
void paddle_ball_collision(GameState* game, bool is_right);
float precision_update(Vector2* bpos, Vector2* ppos);


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
    game->lscore        = 0;
    game->rscore        = 0;
    game->precision_xply = 1.0f;
    game->paused        = false;
}


void pong_update(GameState* game)
{
    if (IsKeyPressed(KEY_P)) { game->paused = !game->paused; }
    if (game->paused) { return; }

    Vector2* lppos = &game->lpaddle.pos;
    Vector2* rppos = &game->rpaddle.pos;
    Ball* b = &game->ball;
    const float dt = GetFrameTime();


    // update paddle pos based on key input
    if (IsKeyDown(KEY_UP))   { lppos->y -= PADDLE_SPEED * dt;}
    if (IsKeyDown(KEY_DOWN)) { lppos->y += PADDLE_SPEED * dt;}
    if (IsKeyDown(KEY_LEFT)) { rppos->y -= PADDLE_SPEED * dt;}
    if (IsKeyDown(KEY_RIGHT)){ rppos->y += PADDLE_SPEED * dt;}
    

    // validate paddle pos (clamping)
    // no vertical offset
    if (lppos->y < 0) { lppos->y = 0; } 
    if (lppos->y + PADDLE_HEIGHT > HEIGHT) { lppos->y = HEIGHT - PADDLE_HEIGHT; } 
    if (rppos->y < 0) { rppos->y = 0; } 
    if (rppos->y + PADDLE_HEIGHT > HEIGHT) { rppos->y = HEIGHT - PADDLE_HEIGHT; } 


    // update ball position based on velocity
    b->pos.x += b->dir.x * BALL_SPEED * game->precision_xply * dt;
    b->pos.y += b->dir.y * BALL_SPEED * game->precision_xply * dt;

    //validate ball pos (set boundry up and down)
    if (b->pos.y < 0) { // i like the "bounce" effect
        b->dir.y *= -1; // half ball goes out of boundry
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
    paddle_ball_collision(game, false); // left
    paddle_ball_collision(game, true);  // right
}

void pong_draw(GameState* game)
{
    BeginDrawing();
    ClearBackground(BLACK);


    if (game->paused) {
        DrawText("PAUSED", (int)(WIDTH / 2) - 80, (int)(HEIGHT/2) - 20, 40, YELLOW);
    }

    // draw scores
    DrawText(TextFormat("%d", game->lscore), (int)(WIDTH / 4.f), 50, 40, GREEN);
    DrawText(TextFormat("%d", game->rscore), (int)(WIDTH - (WIDTH / 4.f)), 50, 40, GREEN);

    // draw central line
    DrawLine((int)(WIDTH / 2.f), 0, (int)(WIDTH / 2.f), HEIGHT, RAYWHITE);

    // draw ball
    DrawCircle((int)game->ball.pos.x, (int)game->ball.pos.y, BALL_RADIUS, RED);

    // draw paddles
    DrawRectangle((int)game->lpaddle.pos.x, (int)game->lpaddle.pos.y,
                  (int)PADDLE_WIDTH, (int)PADDLE_HEIGHT, RAYWHITE);

    DrawRectangle((int)game->rpaddle.pos.x, (int)game->rpaddle.pos.y,
                  (int)PADDLE_WIDTH, (int)PADDLE_HEIGHT, RAYWHITE);

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

    if (len == 0.f) {
        v->x = 0.f;
        v->y = 0.f;
        return;
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


// close to paddle center->speed increase and vice versa
// when this is called, b and p are colliding
float precision_update(Vector2* bpos, Vector2* ppos)
{
    // calulate distance b/w ball and paddle center
    // d = sqrt( (x2 - x1)2 + (y2 - y1)2 )
    float d = sqrtf(((ppos->x + (PADDLE_WIDTH/2.0f) - bpos->x) * (ppos->x + (PADDLE_WIDTH/2.0f) - bpos->x))
                    + ((ppos->y + (PADDLE_HEIGHT/2.0f) - bpos->y) * (ppos->y + (PADDLE_HEIGHT/2.0f) - bpos->y)));

    // d is between [0, PADDLE_HEIGHT/2]
    // convert to percentage [0, 0.5] by div by PADDLE_HEIGHT
    float d_per = d / PADDLE_HEIGHT;

    // from center, 20% up/down is boost region
    if (d_per >= 0 && d_per < 0.2) {
        return BALL_BOOST; 
    }
    // last 30% up/down is speed decrease
    if (d_per >= 0.3 && d_per <= 0.5) {
        return BALL_SLOWDOWN;
    }
    // 0.2-0.3 up/down is neutral zone (xplier same)
    return 1;   // for any anomoly
}

void paddle_ball_collision(GameState* game, bool is_right)
{
    Vector2* ppos = (is_right)? &game->rpaddle.pos : &game->lpaddle.pos;
    Ball* b = &game->ball;

    // converting paddle into rect                          
    Rectangle rect = (Rectangle){ppos->x, ppos->y, PADDLE_WIDTH, PADDLE_HEIGHT};
    if (CheckCollisionCircleRec(b->pos, BALL_RADIUS, rect)) {
        game->precision_xply = precision_update(&b->pos, ppos);
        b->dir.x *= -1;
    }
}

void random_init(void)
{
    SetRandomSeed((unsigned int)time(NULL));
}

void ball_reset(Ball* ball)
{
    float y = (float) GetRandomValue(0, HEIGHT);
    ball->pos = (Vector2){ WIDTH / 2.f, y};

        
    // TODO: not too horizontal ?
    
    // force horizontal bias
    float vx = (GetRandomValue(0, 1) == 0) ? -1.0f : 1.0f;
    float vy = (float)GetRandomValue(-70, 70) / 100.0f;

    ball->dir = (Vector2){ vx, vy };     // direction only 
    
    Vector2Normalize(&ball->dir);
}



