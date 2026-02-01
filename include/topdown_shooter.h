#ifndef TOPDOWN_SHOOTER_H
#define TOPDOWN_SHOOTER_H

#include "genVec_entities.h"
#include <raylib.h>


// Game Config
//=============================
#define WIDTH  1200.0f
#define HEIGHT 800.0f
#define TITLE  "shoot"
#define FPS    60
#define CELL   10.0f // base size unit

// Game Settings
//=============================
#define P_WIDTH      (CELL)
#define P_HEIGHT     (CELL * 2)
#define P_MAX_HEALTH 10
#define P_SPEED      20.0f

#define B_SPEED 50.0f


// map matrix ?


// Entities
//=============================

// player
typedef struct {
    Vector2 pos;
    Vector2 dir;
    u32     health;
} Player;

// bullet : TDS keeps track of fired bullets, dynamic entities (malloced) use genVec?
// typedef struct {
//     Vector2 pos;
//     Vector2 dir;
// } Bullet;


// Game State
//=============================
typedef struct {
    genVec players;
    // genVec bullets; // Bullet type
} TDS;


void TDS_run(void);

#endif // TOPDOWN_SHOOTER_H
