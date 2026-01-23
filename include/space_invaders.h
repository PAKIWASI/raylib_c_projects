#pragma once

#include <raylib.h>
#include <stdbool.h>
#include <stdint.h>


typedef uint8_t u8;
typedef uint32_t u32;
typedef u8 b8;


#define WIDTH  1200.0f
#define HEIGHT 800.0f
#define TITLE  "space invaders"
#define FPS    60

#define CELL 10.0f // base size unit

#define MAX_LIVES   5
#define SHIP_WIDTH  (CELL * 4)
#define SHIP_HEIGHT (CELL * 8)
#define SHIP_SPEED  (CELL * 20)

#define MAX_BULLETS   100
#define BULLET_SPEED  (CELL * 40)
#define BULLET_RADIUS (CELL)

#define MAX_ASTEROIDS     20
#define ASTERIOD_MAX_SIZE (CELL * 16) // we divide until min size
#define ASTERIOD_MIN_SIZE (CELL)
#define ASTERIOD_SPEED    (CELL * 15)



typedef struct {
    Vector2 pos;
    Vector2 dir; // [-1, 1]
    u8 lives;
} Spaceship;

typedef struct {
    Vector2 pos; 
    Vector2 dir;
    b8      alive;    
} Bullet;

typedef struct {
    Vector2 pos;
    Vector2 dir; // [-1, 1]
    float   size;
    b8      alive;    
} Asteroid;

typedef struct {
    Spaceship sp;
    Asteroid  roids[MAX_ASTEROIDS];
    Bullet    bullets[MAX_BULLETS];
    u8        num_bullets;
    b8      paused;
} SI;


void si_run(void);


