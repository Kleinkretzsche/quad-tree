#include "quadtree.h"

Vector2 vec2(float x, float y) {
    return (Vector2){ x, y };
}

AABB aabb_init(Vector2 center, float half_dim) {
    return (AABB){ center, half_dim };
}

bool aabb_contains_point(AABB region, Vector2 point) {
    return point.x < (region.center.x + region.half_dim)
        && point.x > (region.center.x - region.half_dim)
        && point.y < (region.center.y + region.half_dim)
        && point.y > (region.center.y - region.half_dim);
}

qt *qt_create(void) {
    qt *ret = malloc(sizeof(qt));

    ret->points = malloc(sizeof(Vector2) * QT_CAPACITY);
    ret->size = 0;
    ret->bounds = aabb_init(vec2(0.5, 0.5), 0.5);
    ret->ne = NULL;
    ret->se = NULL;
    ret->sw = NULL;
    ret->nw = NULL;

    return ret;
}

qt *qt_create_child(qt *parent, Quad q) {
    Vector2 center = parent->bounds.center;
    float half_dim = parent->bounds.half_dim;
    float quad_dim = half_dim / 2.0f;
    switch(q) {
        case NE:
            center.x += quad_dim; 
            center.y -= quad_dim;
        break;
        case SE:
            center.x += quad_dim; 
            center.y += quad_dim;
        break;
        case SW:
            center.x -= quad_dim; 
            center.y += quad_dim;
        break;
        case NW:
            center.x -= quad_dim; 
            center.y -= quad_dim;
        break;
        default:
        printf("failed to create quad_tree_child");
        return NULL;
    };
    qt *ret = qt_create();
    ret->bounds = aabb_init(center, quad_dim);
    return ret;
}

void qt_subdivide(qt *tree) {
    if (tree == NULL) return;
    Vector2 nw_center = vec2(
        tree->bounds.center.x * 0.5, 
        tree->bounds.center.y * 0.5
    );
    tree->ne = qt_create_child(tree, NE);
    tree->se = qt_create_child(tree, SE);
    tree->nw = qt_create_child(tree, NW);
    tree->sw = qt_create_child(tree, SW);
}

bool qt_insert(qt *tree, Vector2 point) {
    if (!aabb_contains_point(tree->bounds, point)) {
        return false;
    }

    if (tree->size < QT_CAPACITY && tree->nw == NULL) {
        tree->points[tree->size] = point;
        tree->size++;
        return true;
    }

    if (tree->ne == NULL) {
        qt_subdivide(tree);
    }

    if (qt_insert(tree->ne, point)) return true;
    if (qt_insert(tree->se, point)) return true;
    if (qt_insert(tree->sw, point)) return true;
    if (qt_insert(tree->nw, point)) return true;
    printf("insert failed");
    return false;
}

void qt_print(qt *tree) {
    if (tree == NULL) return;
    for (int i = 0; i < tree->size; i++) {
        printf("(%f %f)\n", tree->points[i].x, tree->points[i].y);
    }
    qt_print(tree->ne);
    qt_print(tree->se);
    qt_print(tree->sw);
    qt_print(tree->nw);
    printf("\n");
}

Vector2 coordToScreen(float x, float y) {
    return vec2(WIDTH * x, HEIGHT * y);
}

float GetRandomFloat(float from, float to) {
    return from + (to-from)*(float)GetRandomValue(0, INT_MAX) / INT_MAX;
}

void aabb_draw(AABB box) {
    DrawRectangleLines(
        WIDTH * (box.center.x - box.half_dim), 
        HEIGHT * (box.center.y - box.half_dim),
        WIDTH * (box.half_dim * 2),
        HEIGHT * (box.half_dim * 2),
        BLACK
    );
}

void qt_draw(qt *tree) {
    if (tree == NULL) return;
    for (int i = 0; i < tree->size; i++) {
        // TODO find a good way to draw all points
    }
    aabb_draw(tree->bounds);
    qt_draw(tree->se);
    qt_draw(tree->ne);
    qt_draw(tree->sw);
    qt_draw(tree->nw);
}

void qt_fill(qt *tree, int count) {
    for (int i = 0; i < count; i++) {
        float r1 = GetRandomFloat(0.0, 1.0);
        float r2 = GetRandomFloat(0.0, 1.0);
        qt_insert(tree, vec2(r1, r2));
    }
}

