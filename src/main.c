#include <stdio.h>
#include <time.h>
#include "raylib.h"
#include "quadtree.h"

void drawQuadTree(qt *tree) {
    if (tree == NULL) return;

}

void qt_fill(qt *tree, int count) {
    for (int i = 0; i < count; i++) {
        float r1 = ((float)rand())/RAND_MAX;
        float r2 = ((float)rand())/RAND_MAX;
        qt_insert(tree, vec2_init(r1, r2));
    }
}

int main(void) {
    srand(time(NULL));

    InitWindow(800, 400, "window");

    qt *t = qt_create();
    qt_fill(t, 100);

    while (!WindowShouldClose()) {
        BeginDrawing();

            drawQuadTree(t);
            ClearBackground(WHITE);

        EndDrawing();
    }

    printf("hello world\n");
    return 0;
}
