#include <stdio.h>
#include <time.h>
#include <limits.h>

#define QT_MAX_DEPTH 5
#define QT_ERR_THRESHHOLD 5

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

    while (!WindowShouldClose()) {
        BeginDrawing();
            // if (IsKeyPressed(KEY_SPACE)) {
            if (true) {
                qt_subdivide(t);
            }
            ClearBackground(WHITE);
            // DrawTexture(texture, screen_width/2 - texture.width/2, screen_height/2 - texture.height/2, WHITE);
            qt_draw(t);

        EndDrawing();
    }

    UnloadImage(image);

    return 0;
}
