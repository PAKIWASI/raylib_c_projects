#pragma once
#include "raylib.h"
#include <stdint.h>
#include <stdbool.h>

// Configuration
#define MAX_ENTITIES 128

// Entity is just an ID
typedef uint32_t Entity;

// Component types (bitmask flags)
typedef enum {
    COMPONENT_NONE       = 0,
    COMPONENT_POSITION   = 1 << 0,
    COMPONENT_VELOCITY   = 1 << 1,
    COMPONENT_SPRITE     = 1 << 2,
    COMPONENT_PADDLE     = 1 << 3,
    COMPONENT_BALL       = 1 << 4,
    COMPONENT_COLLISION  = 1 << 5,
} ComponentType;

// Components (pure data, no logic)
typedef struct {
    Vector2 pos;
} Position;

typedef struct {
    Vector2 vel;
} Velocity;

typedef struct {
    Color color;
    float width;
    float height;
} Sprite;

typedef struct {
    int up_key;
    int down_key;
    float speed;
} Paddle;

typedef struct {
    float radius;
} Ball;

typedef struct {
    Rectangle bounds;
} Collision;


// ECS World - stores all entities and components
typedef struct {
    // Entity management
    uint32_t entity_count;
    uint32_t component_mask[MAX_ENTITIES];  // Which components each entity has
    
    // Component arrays (parallel arrays indexed by entity ID)
    Position   positions[MAX_ENTITIES];
    Velocity   velocities[MAX_ENTITIES];
    Sprite     sprites[MAX_ENTITIES];
    Paddle     paddles[MAX_ENTITIES];
    Ball       balls[MAX_ENTITIES];
    Collision  collisions[MAX_ENTITIES];
    
    // Game-specific data
    int left_score;
    int right_score;
    float width;
    float height;
} World;


// === Entity Management ===

Entity entity_create(World* world) {
    if (world->entity_count >= MAX_ENTITIES) {
        return MAX_ENTITIES; // Invalid entity
    }
    Entity e = world->entity_count++;
    world->component_mask[e] = COMPONENT_NONE;
    return e;
}

void entity_add_component(World* world, Entity e, ComponentType type) {
    world->component_mask[e] |= type;
}

bool entity_has_component(World* world, Entity e, ComponentType type) {
    return (world->component_mask[e] & type) != 0;
}

bool entity_has_components(World* world, Entity e, ComponentType types) {
    return (world->component_mask[e] & types) == types;
}

// === Component Setters ===

void set_position(World* world, Entity e, Vector2 pos) {
    entity_add_component(world, e, COMPONENT_POSITION);
    world->positions[e].pos = pos;
}

void set_velocity(World* world, Entity e, Vector2 vel) {
    entity_add_component(world, e, COMPONENT_VELOCITY);
    world->velocities[e].vel = vel;
}

void set_sprite(World* world, Entity e, Color color, float width, float height) {
    entity_add_component(world, e, COMPONENT_SPRITE);
    world->sprites[e].color = color;
    world->sprites[e].width = width;
    world->sprites[e].height = height;
}

void set_paddle(World* world, Entity e, int up_key, int down_key, float speed) {
    entity_add_component(world, e, COMPONENT_PADDLE);
    world->paddles[e].up_key = up_key;
    world->paddles[e].down_key = down_key;
    world->paddles[e].speed = speed;
}

void set_ball(World* world, Entity e, float radius) {
    entity_add_component(world, e, COMPONENT_BALL);
    world->balls[e].radius = radius;
}

void set_collision(World* world, Entity e, Rectangle bounds) {
    entity_add_component(world, e, COMPONENT_COLLISION);
    world->collisions[e].bounds = bounds;
}

// === Systems (logic that operates on components) ===

// Movement System: updates positions based on velocities
void system_movement(World* world) {
    float dt = GetFrameTime();
    
    for (Entity e = 0; e < world->entity_count; e++) {
        if (entity_has_components(world, e, COMPONENT_POSITION | COMPONENT_VELOCITY)) {
            world->positions[e].pos.x += world->velocities[e].vel.x * dt;
            world->positions[e].pos.y += world->velocities[e].vel.y * dt;
        }
    }
}

// Paddle Input System
void system_paddle_input(World* world) {
    float dt = GetFrameTime();
    
    for (Entity e = 0; e < world->entity_count; e++) {
        if (entity_has_components(world, e, COMPONENT_PADDLE | COMPONENT_POSITION)) {
            Paddle* paddle = &world->paddles[e];
            Position* pos = &world->positions[e];
            
            if (IsKeyDown(paddle->up_key)) {
                pos->pos.y -= paddle->speed * dt;
            }
            if (IsKeyDown(paddle->down_key)) {
                pos->pos.y += paddle->speed * dt;
            }
            
            // Clamp paddle to screen
            if (entity_has_component(world, e, COMPONENT_SPRITE)) {
                float paddle_height = world->sprites[e].height;
                if (pos->pos.y < 0) pos->pos.y = 0;
                if (pos->pos.y > world->height - paddle_height) {
                    pos->pos.y = world->height - paddle_height;
                }
            }
        }
    }
}

// Ball Boundary System
void system_ball_boundaries(World* world) {
    for (Entity e = 0; e < world->entity_count; e++) {
        if (entity_has_components(world, e, COMPONENT_BALL | COMPONENT_POSITION | COMPONENT_VELOCITY)) {
            Position* pos = &world->positions[e];
            Velocity* vel = &world->velocities[e];
            float radius = world->balls[e].radius;
            
            // Vertical boundaries
            if (pos->pos.y - radius < 0 || pos->pos.y + radius > world->height) {
                vel->vel.y *= -1;
                // Push ball back in bounds
                if (pos->pos.y - radius < 0) pos->pos.y = radius;
                if (pos->pos.y + radius > world->height) pos->pos.y = world->height - radius;
            }
            
            // Horizontal boundaries (scoring)
            if (pos->pos.x < 0) {
                world->right_score++;
                pos->pos = (Vector2){world->width / 2, world->height / 2};
                vel->vel = (Vector2){-300.0f, 200.0f};
            }
            if (pos->pos.x > world->width) {
                world->left_score++;
                pos->pos = (Vector2){world->width / 2, world->height / 2};
                vel->vel = (Vector2){300.0f, -200.0f};
            }
        }
    }
}

// Ball-Paddle Collision System
void system_ball_paddle_collision(World* world) {
    // Find ball
    Entity ball_entity = MAX_ENTITIES;
    for (Entity e = 0; e < world->entity_count; e++) {
        if (entity_has_component(world, e, COMPONENT_BALL)) {
            ball_entity = e;
            break;
        }
    }
    
    if (ball_entity == MAX_ENTITIES) return;
    
    Position* ball_pos = &world->positions[ball_entity];
    Velocity* ball_vel = &world->velocities[ball_entity];
    float ball_radius = world->balls[ball_entity].radius;
    
    // Check collision with all paddles
    for (Entity e = 0; e < world->entity_count; e++) {
        if (entity_has_components(world, e, COMPONENT_PADDLE | COMPONENT_POSITION | COMPONENT_SPRITE)) {
            Position* paddle_pos = &world->positions[e];
            Sprite* paddle_sprite = &world->sprites[e];
            
            Rectangle paddle_rect = {
                paddle_pos->pos.x,
                paddle_pos->pos.y,
                paddle_sprite->width,
                paddle_sprite->height
            };
            
            if (CheckCollisionCircleRec(ball_pos->pos, ball_radius, paddle_rect)) {
                ball_vel->vel.x *= -1;
                
                // Push ball out of paddle
                if (ball_pos->pos.x < paddle_pos->pos.x) {
                    ball_pos->pos.x = paddle_rect.x - ball_radius;
                } else {
                    ball_pos->pos.x = paddle_rect.x + paddle_rect.width + ball_radius;
                }
            }
        }
    }
}

// Render System
void system_render(World* world) {
    BeginDrawing();
    ClearBackground(BLACK);
    
    // Draw center line
    DrawLine(world->width / 2, 0, world->width / 2, world->height, GRAY);
    
    // Draw scores
    DrawText(TextFormat("%d", world->left_score), world->width / 4, 50, 40, RAYWHITE);
    DrawText(TextFormat("%d", world->right_score), 3 * world->width / 4, 50, 40, RAYWHITE);
    
    // Draw all entities with position and sprite
    for (Entity e = 0; e < world->entity_count; e++) {
        if (entity_has_components(world, e, COMPONENT_POSITION | COMPONENT_SPRITE)) {
            Position* pos = &world->positions[e];
            Sprite* sprite = &world->sprites[e];
            
            if (entity_has_component(world, e, COMPONENT_BALL)) {
                float radius = world->balls[e].radius;
                DrawCircleV(pos->pos, radius, sprite->color);
            } else {
                DrawRectangleV(pos->pos, (Vector2){sprite->width, sprite->height}, sprite->color);
            }
        }
    }
    
    EndDrawing();
}

// === Game Setup ===

void pong_ecs_init(World* world, float width, float height) {
    world->entity_count = 0;
    world->left_score = 0;
    world->right_score = 0;
    world->width = width;
    world->height = height;
    
    float CELL = 20.0f;
    
    // Create left paddle
    Entity left_paddle = entity_create(world);
    set_position(world, left_paddle, (Vector2){CELL, height / 2 - 100});
    set_sprite(world, left_paddle, RAYWHITE, CELL, CELL * 10);
    set_paddle(world, left_paddle, KEY_W, KEY_S, 500.0f);
    
    // Create right paddle
    Entity right_paddle = entity_create(world);
    set_position(world, right_paddle, (Vector2){width - CELL * 2, height / 2 - 100});
    set_sprite(world, right_paddle, RAYWHITE, CELL, CELL * 10);
    set_paddle(world, right_paddle, KEY_I, KEY_K, 500.0f);
    
    // Create ball
    Entity ball = entity_create(world);
    set_position(world, ball, (Vector2){width / 2, height / 2});
    set_velocity(world, ball, (Vector2){300.0f, 200.0f});
    set_sprite(world, ball, RED, 0, 0);  // width/height unused for ball
    set_ball(world, ball, CELL / 2);
}

void pong_ecs_run(void) {
    float WIDTH = 1200.0f;
    float HEIGHT = 800.0f;
    
    InitWindow(WIDTH, HEIGHT, "Pong ECS");
    SetTargetFPS(60);
    
    World world = {0};
    pong_ecs_init(&world, WIDTH, HEIGHT);
    
    while (!WindowShouldClose()) {
        // Update systems
        system_paddle_input(&world);
        system_movement(&world);
        system_ball_boundaries(&world);
        system_ball_paddle_collision(&world);
        
        // Render
        system_render(&world);
    }
    
    CloseWindow();
}

