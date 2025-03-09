#include <stdio.h>
#include <time.h>
#include <limits.h>

#include "raylib.h"
#include "quadtree.h"

#define HEIGHT 400
#define WIDTH  800

Vector2 coordToScreen(float x, float y) {
    return vec2(WIDTH * x, HEIGHT * y);
}

float GetRandomFloat(float from, float to) {
    return from + (to-from)*(float)GetRandomValue(0, INT_MAX) / INT_MAX;
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
        // TODO find a good way to draw all points
    }
    drawAABB(tree->bounds);
    drawQuadTree(tree->ne);
    drawQuadTree(tree->se);
    drawQuadTree(tree->sw);
    drawQuadTree(tree->nw);
}

void qt_fill(qt *tree, int count) {
    for (int i = 0; i < count; i++) {
        float r1 = GetRandomFloat(0.0, 1.0);
        float r2 = GetRandomFloat(0.0, 1.0);
        qt_insert(tree, vec2(r1, r2));
    }
}

int main(void) {
    SetTargetFPS(10);
    InitWindow(800, 400, "window");

    qt *t = qt_create();

    while (!WindowShouldClose()) {
        qt_fill(t, 60);
        BeginDrawing();

            drawQuadTree(t);
            ClearBackground(WHITE);

        EndDrawing();
    }

    return 0;
}
