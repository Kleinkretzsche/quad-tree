#include "quadtree.h"

Vector2 vec2_init(float x, float y) {
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
    ret->bounds = aabb_init(vec2_init(0.5, 0.5), 0.5);
    ret->ne = NULL;
    ret->se = NULL;
    ret->sw = NULL;
    ret->nw = NULL;

    return ret;
}

qt *qt_create_child(qt *parent, Quad q) {
    Vector2 center = parent->bounds.center;
    float half_dim = parent->bounds.half_dim;
    float quad_dim = half_dim / 2;
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
    Vector2 nw_center = vec2_init(
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

    if (tree->ne == NULL) qt_subdivide(tree);

    if (qt_insert(tree->ne, point)) return true;
    if (qt_insert(tree->se, point)) return true;
    if (qt_insert(tree->sw, point)) return true;
    if (qt_insert(tree->nw, point)) return true;
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

int main(void) {
    qt *t = qt_create();
    qt_insert(t, vec2_init(0.4, 0.3));
    qt_insert(t, vec2_init(0.3, 0.3));
    qt_print(t);
    return 0;
}
