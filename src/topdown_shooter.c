#include "topdown_shooter.h"


void TDS_init(TDS* tds)
{

}



void TDS_run(void)
{
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

    TDS tds = {0};

    TDS_init(&tds);

    while (!WindowShouldClose()) {

        TDS_update(&tds);

        TDS_draw(&tds);
    }

    TDS_end();
}

