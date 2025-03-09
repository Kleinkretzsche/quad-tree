#ifndef QT_DEF
#define QT_DEF

#define QT_CAPACITY 1

typedef enum {
    NE,
    SE,
    SW,
    NW,
} Quad;

typedef struct {
    float x;
    float y;
}Vector2;

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

Vector2 vec2_init(float x, float y);

AABB aabb_init(Vector2 center, float half_dim);
bool aabb_contains_point(AABB region, Vector2 point);

qt  *qt_create(void);
qt  *qt_create_child(qt *parent, Quad q);
void qt_subdivide(qt *tree);
bool qt_insert(qt *tree, Vector2 point);
void qt_print(qt *tree);

#endif
