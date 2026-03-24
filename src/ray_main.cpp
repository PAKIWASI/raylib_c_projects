#include <cstdint>
#include <raylib.h>
#include <termios.h>

using u32 = uint32_t;
using u64 = uint64_t;

constexpr u32         width  = 800;
constexpr u32         height = 600;
constexpr const char* title  = "game";

constexpr u32 cell = 10;


auto main() -> int
{
    termios term = {};

    cfgetospeed(&term);

    InitWindow(width, height, title);
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground({.r = 0, .g = 255, .b = 255, .a = 1});
        DrawFPS(cell, cell);

        EndDrawing();
    }

    CloseWindow();
}
