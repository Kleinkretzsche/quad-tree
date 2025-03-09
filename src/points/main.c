#include <stdio.h>
#include <time.h>
#include <limits.h>

#include "raylib.h"
#include "quadtree.h"

int main(void) {
    SetTargetFPS(10);
    InitWindow(WIDTH, HEIGHT, "window");

    qt *t = qt_create();

    while (!WindowShouldClose()) {
        qt_fill(t, 60);
        BeginDrawing();

            qt_draw(t);
            ClearBackground(WHITE);

        EndDrawing();
    }

    return 0;
}
