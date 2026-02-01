#include "topdown_shooter.h"
#include "genVec.h"


void player_add(genVec* players)
{
    Player* p = malloc(sizeof(Player));
    CHECK_FATAL(!p, "player malloc failed");
    p->health
    genVec_push_move(players, );
}


void TDS_init(TDS* tds)
{
    tds->players = genVec_init(2, sizeof(Player), player_copy, player_move, player_del);

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

    TDS_end(&tds);
}

