#pragma once

#include "raylib.h"
#include <stdint.h>

typedef uint32_t u32 ;

#define WIDTH  1200.0f
#define HEIGHT 800.0f
#define TITLE  "pong"
#define FPS    60

#define CELL       20.0f        // base size

#define PADDLE_WIDTH  (CELL * 3)
#define PADDLE_HEIGHT (CELL * 10)
#define PADDLE_SPEED  (CELL * 15) // pixels per second 

#define BALL_RADIUS (CELL * 2)
#define BALL_SPEED  (CELL * 25) // pixels per second
                               

typedef struct {
    Vector2 pos; // pixel positins
    Vector2 v;   // direction * speed
} Ball;

typedef struct {
    Vector2 pos; // pixel positins
} Paddle;

typedef struct {
    Ball ball;
    Paddle left_paddle;
    Paddle right_paddle;
    u32 l_score;
    u32 r_score;
    bool paused;
} GameState;


void pong_run(void);


/* delta time for paddle/ball (making frame rate independent)
float dt = GetFrameTime();
paddle.pos.y += PADDLE_SPEED * dt;
ball.pos = Vector2Add(ball.pos, Vector2Scale(ball.v, dt));


| Quantity          | Unit             |
| ----------------- | ---------------- |
| Position          | pixels           |
| Speed             | pixels / second  |
| Acceleration      | pixels / second² |
| Delta time (`dt`) | seconds          |
*/
