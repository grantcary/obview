#ifndef VEC_H
#define VEC_H

typedef struct Vec3 {
    float x, y, z;
} Vec3;

typedef struct Vec3I {
    int* vec;
} Vec3I;

#define add(a, b) {a.x + b.x, a.y + b.y, a.z + b.z}

#endif