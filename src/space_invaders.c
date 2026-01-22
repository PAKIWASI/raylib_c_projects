#include "space_invaders.h"
#include "raylib.h"
#include <math.h>
#include <string.h>
#include <time.h>




void random_init(void);
//void Vector2Normalize(Vector2* v);
void calc_asteroid_dir(Asteroid* roid, Vector2 ship_pos);


void asteroids_init(Asteroid* roids)
{
    random_init();

    // atleast half of max asteroids must be alive
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        roids[i].size = ASTERIOD_MAX_SIZE;

        // start an asteroid to one of the sides, hidden
        // the dir is relative to player

        // chose random x , y dir
        float x;
        float y;
        if (GetRandomValue(0, 1)) {
            x = GetRandomValue(0, 1) == 0 ? 0 : WIDTH;
            y = (float)GetRandomValue(0, HEIGHT);
        } else {
            x = (float)GetRandomValue(0, WIDTH);
            y = GetRandomValue(0, 1) == 0 ? 0 : HEIGHT;
        }
        roids[i].pos = (Vector2){x, y};

        // set direction based on spaceship pos
        // this is init so player is in center
        calc_asteroid_dir(&roids[i], (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f});            
    }
}


void si_init(SI* si)
{
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    // init spaceship
    si->sp.pos = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
    si->sp.dir = (Vector2){0.0f, 1.0f};
    si->sp.lives = MAX_LIVES;
    
    // init asteriods
    asteroids_init(&si->roids[0]);

    // init bullets
    memset(si->bullets, 0, sizeof(Bullet) * MAX_BULLETS);
    si->num_bullets = 0;

    //init metadata
    si->paused = false;

}

void asteroids_update(Asteroid* roids, float dt)
{
    for (int i = 0; i < MAX_ASTEROIDS; i++) {

        if (roids[i].pos.x == 0 && roids[i].pos.y == 0) { continue; }

        roids[i].pos.x += ASTERIOD_SPEED * roids[i].dir.x * dt;
        roids[i].pos.y += ASTERIOD_SPEED * roids[i].dir.y * dt;
    }
}

void ship_shoot_bullet(SI* si)
{
    if (si->num_bullets == MAX_BULLETS) { return; }
    // shoot bullet in the dir of the ship,
    // staring at pos = ship head
    si->bullets[si->num_bullets].pos = si->sp.pos;
    si->bullets[si->num_bullets].dir = si->sp.dir;
    si->num_bullets++; // TODO: does this move by 1 or sizeof(Bullet) ?
}

void bullets_update(Bullet* bullets, u8 num_bullets, float dt)
{
    for (int i = 0; i < MAX_BULLETS; i++) {

        if (bullets[i].pos.x == 0 && bullets[i].pos.y == 0) { continue; }

        bullets[i].pos.x += BULLET_SPEED * bullets[i].dir.x * dt;
        bullets[i].pos.y += BULLET_SPEED * bullets[i].dir.y * dt;
    }
}

void bullet_asteroid_collision(Bullet* bullets, Asteroid* roids)
{
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        // not living
        if (roids[i].pos.x == 0 && roids[i].pos.y == 0) { continue; }

        for (int j = 0; j < MAX_BULLETS; j++) {

            if (bullets[j].pos.x == 0 && bullets[j].pos.y == 0) { continue; }

            if (CheckCollisionCircles((Vector2){bullets[j].pos.x, bullets[j].pos.y}, BULLET_RADIUS, 
                                      (Vector2){roids[i].pos.x, roids[i].pos.y}, roids[i].size/2.0f)) 
            {
                roids[i].size /= 2;
                if (roids[i].size == 0) {
                    memset(&roids[i], 0, sizeof(Asteroid));                     
                }

                memset()
            }
        }
    }
}

void si_update(SI* si)
{
    const float dt = GetFrameTime();

    Vector2* ship_pos = &si->sp.pos;
    Vector2* ship_dir = &si->sp.dir;

    // update spaceship pos based on user input
    if (IsKeyDown(KEY_UP))    { ship_pos->y -= SHIP_SPEED * ship_dir->y * dt; }
    if (IsKeyDown(KEY_DOWN))  { ship_pos->y += SHIP_SPEED * ship_dir->y * dt; }
    if (IsKeyDown(KEY_RIGHT)) { ship_pos->x += SHIP_SPEED * ship_dir->x * dt; }
    if (IsKeyDown(KEY_LEFT))  { ship_pos->x -= SHIP_SPEED * ship_dir->x * dt; }
    if (IsKeyDown(KEY_SPACE)) { ship_shoot_bullet(si); }

    // update asteriods based on dir
    asteroids_update(&si->roids[0], dt);

    // update bullets
    bullets_update(si->bullets, si->num_bullets, dt);

    // bullet asteroid collision
    bullet_asteroid_collision(si->bullets, si->roids);

    // more shi
}

void si_draw(SI* si)
{
    BeginDrawing();
    ClearBackground(BLACK);

    // draw asteroids
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        
        if (si->roids[i].pos.x == 0 && si->roids[i].pos.y == 0) { continue; }

        DrawCircle((int)si->roids[i].pos.x, (int)si->roids->pos.y, si->roids[i].size, RAYWHITE);
    }

    // draw spaceship
    DrawRectangle((int)si->sp.pos.x, (int)si->sp.pos.y, (int)SHIP_WIDTH, (int)SHIP_HEIGHT, BLUE);

    // draw bullets

    EndDrawing();
}


void si_run(void)
{
    SI si = {0};

    si_init(&si);

    while (!WindowShouldClose()) {

        si_update(&si);

        si_draw(&si);
    }

    si_end();
}



void random_init(void)
{
    SetRandomSeed((unsigned int)time(NULL));
}

void calc_asteroid_dir(Asteroid* roid, Vector2 ship_pos) 
{
    // Calculate direction vector from asteroid to ship
    float dx = ship_pos.x - roid->pos.x;
    float dy = ship_pos.y - roid->pos.y;
    
    // Calculate length of the vector
    float length = sqrtf((dx * dx) + (dy * dy));
    
    // Normalize the vector (set it to unit length)
    if (length > 0) {
        roid->dir.x = dx / length;
        roid->dir.y = dy / length;
    } else {
        // Handle case where asteroid is at same position as ship
        roid->dir.x = 0;
        roid->dir.y = 0;
    }
}


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
