#ifndef QT_DEF
#define QT_DEF

#define WIDTH 1200
#define HEIGHT 1200 

#include "raylib.h"
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <stddef.h>

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
    Image img;
    Color avg;
    float err;

    AABB bounds;

    struct qt *ne;
    struct qt *se;
    struct qt *sw;
    struct qt *nw;
} qt;

Vector2 vec2(float x, float y);

AABB  aabb_init(Vector2 center, float half_dim);
bool  aabb_contains_point(AABB region, Vector2 point);
void  aabb_draw(AABB box, Color color);
Color aabb_avg(Image img, AABB aabb);
float aabb_err(Image img, AABB aabb);

qt   *qt_create(Image img);
qt   *qt_create_with_bounds(Image img, AABB aabb);
qt   *qt_create_child(qt *parent, Quad q);
void  qt_subdivide(qt *tree);
void  qt_draw(qt *tree);
void  qt_calc_avg_err(qt *tree);
qt   *qt_create_child(qt *parent, Quad q);
float qt_get_max_err(qt *parent);
void  qt_draw(qt *tree);

Vector2 coordToScreen(float x, float y);
float GetRandomFloat(float from, float to);

#endif
