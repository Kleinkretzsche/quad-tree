#include <stdio.h>
#include <time.h>
#include <limits.h>

#include "raylib.h"
#include "quadtree.h"

int main(void) {
    SetTargetFPS(10);

    int screen_width = 1200;
    int screen_height = 1200;

    InitWindow(screen_width, screen_height, "window");

    Image image = LoadImage("resources/fish.png");
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
