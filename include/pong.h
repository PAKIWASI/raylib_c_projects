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
#define PADDLE_SPEED  (CELL * 35) // pixels per second

#define BALL_RADIUS   (CELL * 2)
#define BALL_SPEED    (CELL * 50) // pixels per second
#define BOOST    (1.5f)
#define SLOWDOWN (0.8f)

#define BOOST_XPLY(ralley, boost) (boost + ((float)ralley * 0.1f))



typedef struct {
    Vector2 pos; // pixel positins
    Vector2 dir; // between [-1, 1]. direction only
    float   boost;
} Ball;

typedef struct {
    Vector2 pos; // pixel positins
    float boost;
} Paddle;

typedef uint32_t u32;

typedef struct {
    Ball   ball;
    Paddle lpaddle;
    Paddle rpaddle;
    u32    lscore;
    u32    rscore;
    //float  precision_xply;
    u32    ralley_count;
    bool   paused;
} Pong;


void pong_run(void);


// TODO: incorporate:
/*
// Add to pong.h:
typedef struct {
    char text[32];
    Vector2 pos;
    float lifetime;  // seconds remaining
    Color color;
} FloatingText;

#define MAX_FLOATING_TEXTS 10

typedef struct {
    Ball   ball;
    Paddle lpaddle;
    Paddle rpaddle;
    u32    lscore;
    u32    rscore;
    u32    ralley_count;
    bool   paused;
    FloatingText floating_texts[MAX_FLOATING_TEXTS];
    int    floating_text_count;
} Pong;

// ============================================================
// VISUAL FEEDBACK FUNCTIONS
// ============================================================

// Get color based on ball speed (visual indicator)
Color get_ball_color(float boost, u32 ralley_count)
{
    float total_multiplier = BOOST_XPLY(ralley_count, boost);
    
    // Speed ranges with color coding:
    // 1.0 - 1.5: White (normal)
    // 1.5 - 2.0: Yellow (getting fast)
    // 2.0 - 2.5: Orange (fast)
    // 2.5+:      Red (very fast)
    
    if (total_multiplier < 1.5f) return WHITE;
    if (total_multiplier < 2.0f) return YELLOW;
    if (total_multiplier < 2.5f) return ORANGE;
    return RED;
}

// Get paddle color based on boost state
Color get_paddle_color(float boost)
{
    if (boost >= BOOST) return GREEN;      // Boosted
    if (boost <= SLOWDOWN) return BLUE;    // Slowed down
    return RAYWHITE;                       // Neutral
}

// Add floating text effect
void add_floating_text(Pong* pong, const char* text, Vector2 pos, Color color)
{
    if (pong->floating_text_count >= MAX_FLOATING_TEXTS) return;
    
    FloatingText* ft = &pong->floating_texts[pong->floating_text_count];
    snprintf(ft->text, sizeof(ft->text), "%s", text);
    ft->pos = pos;
    ft->lifetime = 1.5f; // Display for 1.5 seconds
    ft->color = color;
    
    pong->floating_text_count++;
}

// Update and draw floating texts
void update_floating_texts(Pong* pong, float dt)
{
    for (int i = 0; i < pong->floating_text_count; i++) {
        FloatingText* ft = &pong->floating_texts[i];
        ft->lifetime -= dt;
        ft->pos.y -= 50.0f * dt; // Float upward
        
        // Remove expired texts
        if (ft->lifetime <= 0) {
            // Shift remaining texts down
            for (int j = i; j < pong->floating_text_count - 1; j++) {
                pong->floating_texts[j] = pong->floating_texts[j + 1];
            }
            pong->floating_text_count--;
            i--; // Check this index again
        }
    }
}

void draw_floating_texts(Pong* pong)
{
    for (int i = 0; i < pong->floating_text_count; i++) {
        FloatingText* ft = &pong->floating_texts[i];
        
        // Fade out based on remaining lifetime
        Color color = ft->color;
        color.a = (unsigned char)(255 * (ft->lifetime / 1.5f));
        
        DrawText(ft->text, (int)ft->pos.x, (int)ft->pos.y, 20, color);
    }
}

*/
