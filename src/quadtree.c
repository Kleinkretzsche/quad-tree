#include <stdlib.h>
#include "raylib.h"

#define QT_CAPACITY 4

typedef struct {
    float x;
    float y;
}Vector2;

typedef struct {
    Vector2 center;
    float half_dim;
} AABB;

typedef struct qt{
    Vector2 points[QT_CAPACITY];
    AABB bounds;

    struct qt *NE;
    struct qt *SE;
    struct qt *SW;
    struct qt *NW;
} qt;

Vector2 vec2_init(float x, float y);

AABB aabb_init(vec2f_t center, float half_dim);

qt *qt_create(void);

Vector2 vec2_init(float x, float y);
    return (Vector2){ x, y };
}

AABB aabb_init(vec2f_t center, float half_dim) {
    return (AABB){ center, half_dim };
}

qt *qt_create(void) {
    qt *ret = malloc(sizeof(qt));

    qt->points = NULL;

    qt->AABB = aabb_init(vec2_init(0.5, 0.5), 0.5);

    qt->NE = NULL;
    qt->SE = NULL;
    qt->SW = NULL;
    qt->NW = NULL;

    return qt;
}

int main(void) {
    return 0;
}
