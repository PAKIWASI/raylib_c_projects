#include <raylib.h>

#define WIDTH  600
#define HEIGHT 480

int main(void)
{ 
    InitWindow(WIDTH, HEIGHT, "g");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){10, 100, 10, 1});
        EndDrawing();
    }

    CloseWindow();
}
