#include <stdio.h>
#include <time.h>
#include <limits.h>

#define QT_MAX_DEPTH 3
#define QT_ERR_THRESHHOLD 10

#include "raylib.h"
#include "quadtree.h"

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("[USAGE]: ./a.out IMAGE_FILENAME");
        return 1;
    }
    SetTargetFPS(100);
    Image image = LoadImage(argv[1]);

    SetTargetFPS(100);

    int screen_width = image.width;
    int screen_height = image.height;

    InitWindow(screen_width, screen_height, "window");

    Texture2D texture = LoadTextureFromImage(image);

    qt *t = qt_create(image);
    while (qt_subdivide(t)) {}

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(WHITE);
            qt_draw(t);

        EndDrawing();
    }

    qt_free(t);
    UnloadImage(image);

    return 0;
}
