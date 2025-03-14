#ifndef QT_DEF
#define QT_DEF

#include "raylib.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define QT_CAPACITY 8

#define WIDTH 800
#define HEIGHT 400 

typedef enum {
    NE,
    SE,
    SW,
    NW,
} Quad;

typedef struct {
    Vector2 center;
    float half_dim;
} AABB;

typedef struct qt{
    AABB bounds;
    size_t size;

    Vector2 *points;

    struct qt *ne;
    struct qt *se;
    struct qt *sw;
    struct qt *nw;
} qt;

Vector2 vec2(float x, float y);

AABB aabb_init(Vector2 center, float half_dim);
bool aabb_contains_point(AABB region, Vector2 point);
void aabb_draw(AABB box);

qt  *qt_create(void);
qt  *qt_create_child(qt *parent, Quad q);
void qt_subdivide(qt *tree);
bool qt_insert(qt *tree, Vector2 point);
void qt_print(qt *tree);
void qt_fill(qt *tree, int count);
void qt_draw(qt *tree);

Vector2 coordToScreen(float x, float y);
float GetRandomFloat(float from, float to);

#endif
