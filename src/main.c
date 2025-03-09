#include <stdio.h>
#include <time.h>

#include "raylib.h"
#include "quadtree.h"

#define HEIGHT 400
#define WIDTH  800

Vector2 coordToScreen(float x, float y) {
    return vec2(WIDTH * x, HEIGHT * y);
}

void drawAABB(AABB box) {
    DrawRectangleLines(
        WIDTH * (box.center.x - box.half_dim), 
        HEIGHT * (box.center.y - box.half_dim),
        WIDTH * (box.half_dim * 2),
        HEIGHT * (box.half_dim * 2),
        BLACK
    );
}

void drawQuadTree(qt *tree) {
    if (tree == NULL) return;
    for (int i = 0; i < tree->size; i++) {
        // DrawCircleV(coordToScreen(tree->points[i].x, tree->points[i].y), 5, BLUE);
        DrawPixelV(coordToScreen(tree->points[i].x, tree->points[i].y), BLUE);
    }
    drawAABB(tree->bounds);
    drawQuadTree(tree->ne);
    drawQuadTree(tree->se);
    drawQuadTree(tree->sw);
    drawQuadTree(tree->nw);
}

void qt_fill(qt *tree, int count) {
    for (int i = 0; i < count; i++) {
        float r1 = ((float)rand())/RAND_MAX;
        float r2 = ((float)rand())/RAND_MAX;
        qt_insert(tree, vec2(r1, r2));
    }
}

int main(void) {
    srand(time(NULL));

    SetTargetFPS(10);
    InitWindow(800, 400, "window");

    qt *t = qt_create();

    while (!WindowShouldClose()) {
        //if (IsKeyPressed(KEY_SPACE)) {
        if (true) {
            qt_fill(t, 10);
        }
        BeginDrawing();

            drawQuadTree(t);
            ClearBackground(WHITE);

        EndDrawing();
    }

    return 0;
}
