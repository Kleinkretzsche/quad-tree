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

void aabb_draw(AABB box, Color color) {
    DrawRectangle(
        WIDTH  * (box.center.x - box.half_dim), 
        HEIGHT * (box.center.y - box.half_dim),
        WIDTH  * (box.half_dim * 2),
        HEIGHT * (box.half_dim * 2),
        color
    );
}

Color aabb_avg(Image img, AABB aabb) {
    uint64_t avg_r = 0;
    uint64_t avg_g = 0;
    uint64_t avg_b = 0;
    uint64_t avg_a = 0;
    uint64_t avg_count = 0;
    for (int y = img.height * (aabb.center.y - aabb.half_dim); y < img.height * (aabb.center.y + aabb.half_dim); y++) {
        for (int x = img.width * (aabb.center.x - aabb.half_dim); x < img.width * (aabb.center.x + aabb.half_dim); x++) {
            avg_count++;
            Color c = GetImageColor(img, x, y);
            avg_r += c.r;
            avg_g += c.g;
            avg_b += c.b;
            avg_a += c.a;
        }
    }
    return (Color){avg_r / avg_count, avg_g / avg_count, avg_b / avg_count, avg_a / avg_count}; 
}

float aabb_err(Image img, AABB aabb) {
    uint64_t avg_r = 0;
    uint64_t avg_g = 0;
    uint64_t avg_b = 0;
    uint64_t avg_a = 0;
    uint64_t n = 0;
    for (int y = img.height * (aabb.center.y - aabb.half_dim); y < img.height * (aabb.center.y + aabb.half_dim); y++) {
        for (int x = img.width * (aabb.center.x - aabb.half_dim); x < img.width * (aabb.center.x + aabb.half_dim); x++) {
            n++;
            Color c = GetImageColor(img, x, y);
            avg_r += c.r;
            avg_g += c.g;
            avg_b += c.b;
            avg_a += c.a;
        }
    }

    if (n == 0) return 0;

    Color avg = (Color){avg_r / n, avg_g / n, avg_b / n, avg_a / n}; 

    uint64_t dev_r = 0;
    uint64_t dev_g = 0;
    uint64_t dev_b = 0;
    uint64_t dev_a = 0;
    for (int y = img.height * (aabb.center.y - aabb.half_dim); y < img.height * (aabb.center.y + aabb.half_dim); y++) {
        for (int x = img.width * (aabb.center.x - aabb.half_dim); x < img.width * (aabb.center.x + aabb.half_dim); x++) {
            Color c = GetImageColor(img, x, y);
            dev_r += fabs(c.r - avg.r);
            dev_g += fabs(c.g - avg.g);
            dev_b += fabs(c.b - avg.b);
            dev_a += fabs(c.a - avg.a);
        }
    }
    Color dev = (Color){dev_r / n, dev_g / n, dev_b / n, dev_a / n}; 
    return dev.r + dev.g + dev.b + dev.a;

}

void qt_calc_avg_err(qt *tree) {
    AABB aabb = tree->bounds;
    Image img = tree->img;

    double avg_r = 0;
    double avg_g = 0;
    double avg_b = 0;
    double avg_a = 0;
    uint64_t n = 0;

    for (int y = img.height * (aabb.center.y - aabb.half_dim); y < img.height * (aabb.center.y + aabb.half_dim); y++) {
        for (int x = img.width * (aabb.center.x - aabb.half_dim); x < img.width * (aabb.center.x + aabb.half_dim); x++) {
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
    for (int y = img.height * (aabb.center.y - aabb.half_dim); y < img.height * (aabb.center.y + aabb.half_dim); y++) {
        for (int x = img.width * (aabb.center.x - aabb.half_dim); x < img.width * (aabb.center.x + aabb.half_dim); x++) {
            n++;
            Color c = GetImageColor(img, x, y);
            dev_r += (c.r - tree->avg.r) * (c.r - tree->avg.r);
            dev_g += (c.g - tree->avg.g) * (c.g - tree->avg.g);
            dev_b += (c.b - tree->avg.b) * (c.b - tree->avg.b);
            dev_a += (c.a - tree->avg.a) * (c.a - tree->avg.a);
        }
    }
    tree->err = sqrt(dev_r / n * n + dev_g / n * n + dev_b / n * n + dev_a / n * n); 
}

qt *qt_create(Image img) {
    return qt_create_with_bounds(img, aabb_init(vec2(0.5, 0.5), 0.5));
}

qt *qt_create_with_bounds(Image img, AABB bounds) {
    qt *ret = malloc(sizeof(qt));

    ret->img = img;
    ret->bounds = bounds;
    ret->ne = NULL;
    ret->se = NULL;
    ret->sw = NULL;
    ret->nw = NULL;

    // init avg and err
    qt_calc_avg_err(ret);

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
    qt *ret = qt_create_with_bounds(parent->img, aabb_init(center, quad_dim));
    return ret;
}

Quad qt_get_max_err(qt *parent) {
    if (parent == NULL) return -1;
    if (parent->ne == NULL) {
        return parent->err;
    }
    float max_err = -1;
    Quad q = -1;
    float errs[4];
    errs[0] = qt_get_max_err(parent->ne);
    errs[1] = qt_get_max_err(parent->se);
    errs[2] = qt_get_max_err(parent->sw);
    errs[3] = qt_get_max_err(parent->nw);
    for (int i = 0; i < 4; i++) {
        if (errs[i] > max_err) {
            max_err = errs[i];
            q = i;
        }
    }
    return q;
}

void qt_subdivide(qt *tree) {
    if (tree == NULL) return;

    if (tree->ne == NULL) {
        tree->ne = qt_create_child(tree, NE);
        tree->se = qt_create_child(tree, SE);
        tree->nw = qt_create_child(tree, NW);
        tree->sw = qt_create_child(tree, SW);
        return;
    }
    switch (qt_get_max_err(tree)) {
        case NE:
            qt_subdivide(tree->ne);
        break;
        case SE:
            qt_subdivide(tree->se);
        break;
        case SW:
            qt_subdivide(tree->sw);
        break;
        case NW:
            qt_subdivide(tree->nw);
        break;
        default:
        printf("couldn't subdivide");
    }

}

float GetRandomFloat(float from, float to) {
    return from + (to-from)*(float)GetRandomValue(0, INT_MAX) / INT_MAX;
}

void qt_draw(qt *tree) {
    if (tree == NULL) return;
    aabb_draw(tree->bounds, tree->avg);
    qt_draw(tree->se);
    qt_draw(tree->ne);
    qt_draw(tree->sw);
    qt_draw(tree->nw);
}

