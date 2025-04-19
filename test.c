#include <stdio.h>
#include <stdlib.h>
#include "vec.h"

void assign(Group* o) {
    Vec3 b = {4.0, 5.0, 6.0};

    Vec3* x = (Vec3*) malloc(2 * sizeof(Vec3));
    x[0].x = 1.0;
    x[0].y = 2.0;
    x[0].z = 3.0;
    x[1] = b;

    o->vertices_size = 2;
    o->vertices = x;
}

void v3print(Vec3* v3, int v3_size) {
    if (v3_size > 0) {
        for (int i = 0; i < v3_size; i++) {
            printf("%f, %f, %f\n", v3[i].x, v3[i].y, v3[i].z);
        }
    }
}

int main() {
    OBJ* o;
    assign(o);
    v3print(o->vertices, o->vertices_size);

    return 0;
}