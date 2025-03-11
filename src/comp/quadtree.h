#ifndef QT_DEF
#define QT_DEF

#ifndef QT_MAX_DEPTH
    #define QT_MAX_DEPTH 10
    #define QT_ERR_THRESHHOLD 100
#endif

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

    size_t depth;

    struct qt *parent;
    struct qt *children[4];
} qt;

Vector2 vec2(float x, float y);

AABB  aabb_init(Vector2 center, float half_dim);

qt   *qt_create(Image img);
qt   *qt_create_with_bounds(Image img, AABB aabb);
qt   *qt_create_child(qt *parent, Quad q);
void  qt_free(qt *tree);
bool  qt_is_leaf(qt *tree);
void  qt_calc_avg_err(qt *tree);
qt   *qt_find_leaf_with_max_err(qt *tree);
bool  qt_subdivide(qt *tree);
void  qt_draw(qt *tree);

#endif
