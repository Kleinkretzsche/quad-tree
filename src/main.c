#include <stdio.h>
#include "raylib.h"

int main(void) {
    InitWindow(800, 400, "window");

    while (!WindowShouldClose()) {
        BeginDrawing();

            DrawText("hello world", 200, 100, 40, BLACK);
            ClearBackground(WHITE);

        EndDrawing();
    }

    printf("hello world\n");
    return 0;
}
