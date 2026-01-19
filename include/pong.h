#pragma once

#include "raylib.h"
#include <stdint.h>

typedef uint32_t u32 ;

#define WIDTH  1200.0f
#define HEIGHT 800.0f
#define TITLE  "pong"
#define FPS    60

#define CELL   10.0f        // base size unit

#define PADDLE_WIDTH  (CELL * 2)
#define PADDLE_HEIGHT (CELL * 25)
#define PADDLE_SPEED  (CELL * 25) // pixels per second 

#define BALL_RADIUS (CELL * 2)
#define BALL_SPEED  (CELL * 50) // pixels per second
                               

typedef struct {
    Vector2 pos; // pixel positins
    Vector2 dir;   // between [-1, 1]. direction only 
    //Vector2 v;      // this is the scalar speed in each dir
} Ball;

typedef struct {
    Vector2 pos; // pixel positins
} Paddle;

typedef struct {
    Ball    ball;
    Paddle  lpaddle;
    Paddle  rpaddle;
    u32     lscore;
    u32     rscore;
    bool    paused;
} GameState;



void pong_run(void);

