#pragma once

#include "raylib.h"
#include <stdint.h>


#define WIDTH  1200.0f
#define HEIGHT 800.0f
#define TITLE  "pong"
#define FPS    60

#define CELL   10.0f        // base size unit

#define PADDLE_WIDTH  (CELL * 2)
#define PADDLE_HEIGHT (CELL * 25)
#define PADDLE_SPEED  (CELL * 25) // pixels per second 

#define BALL_RADIUS     (CELL * 2)
#define BALL_SPEED      (CELL * 50) // pixels per second
#define BALL_BOOST      (2.0f)
#define BALL_SLOWDOWN   (0.2f)      // TODO: make it less to progressively increase difficulty ?
                               

typedef struct {
    Vector2 pos; // pixel positins
    Vector2 dir;   // between [-1, 1]. direction only 
} Ball;

typedef struct {
    Vector2 pos; // pixel positins
} Paddle;

typedef uint32_t u32 ;

typedef struct {
    Ball    ball;
    Paddle  lpaddle;
    Paddle  rpaddle;
    u32     lscore;
    u32     rscore;
    float   precision_xply;
    bool    paused;
} GameState;



void pong_run(void);

