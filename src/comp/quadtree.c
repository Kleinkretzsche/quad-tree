#include "quadtree.h"

#define SWAP(T, x, y) do { T SWAP = x; x = y; y = SWAP; } while (0)

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

void qt_calc_avg_err(qt *tree) {
    AABB aabb = tree->bounds;
    Image img = tree->img;

    double avg_r = 0;
    double avg_g = 0;
    double avg_b = 0;
    double avg_a = 0;
    uint64_t n = 0;

    int min_x = fmax(0, img.width * (aabb.center.x - aabb.half_dim));
    int max_x = fmin(img.width, img.width * (aabb.center.x + aabb.half_dim));
    int min_y = fmax(0, img.height * (aabb.center.y - aabb.half_dim));
    int max_y = fmin(img.height, img.height * (aabb.center.y + aabb.half_dim));

    for (int y = min_y; y < max_y; y++) {
        for (int x = min_x; x < max_x; x++) {
            n++;
            Color c = GetImageColor(img, x, y);
            avg_r += c.r / 255.0; 
            avg_g += c.g / 255.0;
            avg_b += c.b / 255.0;
            avg_a += c.a / 255.0;
        }
    }

    if (n == 0) {
        tree->avg = (Color){0,0,0,0};
        tree->err = -1;
        return;
    }

    tree->avg = (Color){avg_r * 255 / n, avg_g * 255 / n, avg_b * 255 / n, avg_a * 255 / n}; 

    uint64_t dev_r = 0;
    uint64_t dev_g = 0;
    uint64_t dev_b = 0;
    uint64_t dev_a = 0;
    for (int y = min_y; y < max_y; y++) {
        for (int x = min_x; x < max_x; x++) {
            Color c = GetImageColor(img, x, y);
            dev_r += fabs(c.r - tree->avg.r);
            dev_g += fabs(c.g - tree->avg.g);
            dev_b += fabs(c.b - tree->avg.b);
            dev_a += fabs(c.a - tree->avg.a);
        }
    }
    tree->err = (dev_r + dev_g + dev_b + dev_a) / 4;
}

qt *qt_create(Image img) {
    return qt_create_with_bounds(img, aabb_init(vec2(0.5, 0.5), 0.5));
}

qt *qt_create_with_bounds(Image img, AABB bounds) {
    qt *ret = malloc(sizeof(qt));
    ret->img = img;
    ret->bounds = bounds;
    ret->depth = 1;
    ret->parent = NULL;
    ret->ne = NULL;
    ret->se = NULL;
    ret->sw = NULL;
    ret->nw = NULL;
    qt_calc_avg_err(ret);
    return ret;
}

qt *qt_create_child(qt *tree, Quad q) {
    Vector2 center = tree->bounds.center;
    float half_dim = tree->bounds.half_dim;
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
        return NULL;
    };
    qt *ret = qt_create_with_bounds(tree->img, aabb_init(center, quad_dim));
    ret->parent = tree;
    ret->depth = tree->depth + 1;
    return ret;
}

qt *qt_find_leaf_with_max_err(qt *tree) {
    if (tree == NULL) return NULL;
    if (tree->ne == NULL && tree->depth <= QT_MAX_DEPTH) return tree;
    if (tree->ne == NULL) return NULL;
    qt *se_max = qt_find_leaf_with_max_err(tree->se);
    qt *ne_max = qt_find_leaf_with_max_err(tree->ne);
    qt *sw_max = qt_find_leaf_with_max_err(tree->sw);
    qt *nw_max = qt_find_leaf_with_max_err(tree->nw);

    qt *ret = NULL;
    if (ne_max != NULL) ret = ne_max;
    if (se_max != NULL && ret->err < se_max->err) ret = se_max;
    if (sw_max != NULL && ret->err < sw_max->err) ret = sw_max;
    if (nw_max != NULL && ret->err < nw_max->err) ret = nw_max;
    return ret;
}

bool qt_subdivide(qt *tree) {
    if (tree == NULL) return false;
    if (tree->ne == NULL && tree->depth < QT_MAX_DEPTH) {
        tree->ne = qt_create_child(tree, NE);
        tree->se = qt_create_child(tree, SE);
        tree->sw = qt_create_child(tree, SW);
        tree->nw = qt_create_child(tree, NW);
        return true;
    }
    if (tree->depth < QT_MAX_DEPTH) {
        qt *max = qt_find_leaf_with_max_err(tree);
        if (max != NULL && max->err >= QT_ERR_THRESHHOLD)
            return qt_subdivide(max);
    }
    return false;
}

void qt_draw(qt *tree) {
    if (tree == NULL) return;
    AABB box = tree->bounds;
    DrawRectangle(
        tree->img.width  * (box.center.x - box.half_dim), 
        tree->img.height * (box.center.y - box.half_dim),
        tree->img.width  * (box.half_dim * 2),
        tree->img.height * (box.half_dim * 2),
        tree->avg
    );
    qt_draw(tree->se);
    qt_draw(tree->ne);
    qt_draw(tree->sw);
    qt_draw(tree->nw);
}

