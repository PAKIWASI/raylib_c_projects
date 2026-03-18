#include <raylib.h>



#define WIDTH  800
#define HEIGHT 600
#define CELL 10


int main(void)
{ 
    InitWindow(WIDTH, HEIGHT, "g");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){10, 100, 10, 1});
        DrawFPS(CELL, CELL);
        EndDrawing();
    }

    CloseWindow();
}
