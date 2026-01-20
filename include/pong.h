#pragma once

#include "raylib.h"
#include <stdint.h>


#define WIDTH  1200.0f
#define HEIGHT 800.0f
#define TITLE  "pong"
#define FPS    60

#define CELL 10.0f // base size unit

#define PADDLE_WIDTH  (CELL * 2)
#define PADDLE_HEIGHT (CELL * 20)
#define PADDLE_SPEED  (CELL * 25) // pixels per second

#define BALL_RADIUS   (CELL * 2)
#define BALL_SPEED    (CELL * 50) // pixels per second
#define BALL_BOOST    (2.0f)
#define BALL_SLOWDOWN (0.5f)


typedef struct {
    Vector2 pos; // pixel positins
    Vector2 dir; // between [-1, 1]. direction only
} Ball;

typedef struct {
    Vector2 pos; // pixel positins
} Paddle;

typedef uint32_t u32;

typedef struct {
    Ball   ball;
    Paddle lpaddle;
    Paddle rpaddle;
    u32    lscore;
    u32    rscore;
    float  precision_xply;
    bool   paused;
} Pong;


void pong_run(void);
