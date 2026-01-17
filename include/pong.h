#pragma once



#define WIDTH  600.f
#define HEIGHT 400.f
#define TITLE  "pong"


#define NUM_TILES_X 120
#define NUM_TILES_Y 80


#define TILE_W (WIDTH / NUM_TILES_X)
#define TILE_H (HEIGHT / NUM_TILES_Y)



void pong_run(void);
