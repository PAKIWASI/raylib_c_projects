#pragma once

#include "raylib.h"
#include <stdint.h>


#define WIDTH  1200.0f
#define HEIGHT 800.0f
#define TITLE  "pong"
#define FPS    60

#define CELL       20.0f
#define NUM_CELL_X ((int)(WIDTH / CELL))
#define NUM_CELL_Y ((int)(HEIGHT / CELL))

#define PADDLE_WIDTH  (CELL * 1)
#define PADDLE_HEIGHT (CELL * 10)
#define PADDLE_SPEED  (CELL * 15) // pixels per second 
#define L_POS_X       1             // in cell coords (padding of 1)
#define R_POS_X       NUM_CELL_X - 2 // -1 is last cell, -2 for padding

#define BALL_RADIUS (CELL / 2)
#define BALL_SPEED  (CELL * 25) // pixels per second
                               

typedef struct {
    int x, y;
} V2int;

typedef struct {
    V2int pos; // cell based positions
    Vector2 v;   // direction * speed
} Ball;


typedef struct {
    V2int pos; // cell based positins
    uint32_t score;
} Paddle;


void pong_run(void);


/* delta time for paddle/ball
float dt = GetFrameTime();
paddle.pos.y += PADDLE_SPEED * dt;
ball.pos = Vector2Add(ball.pos, Vector2Scale(ball.v, dt));
*/

/*
| Quantity          | Unit             |
| ----------------- | ---------------- |
| Position          | pixels           |
| Speed             | pixels / second  |
| Acceleration      | pixels / second² |
| Delta time (`dt`) | seconds          |
*/
