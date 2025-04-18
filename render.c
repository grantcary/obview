#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"
#include "vec.h"

int main() {
    char* filename = "cube.obj";
    OBJ* o = read_obj(filename);

    if (o == NULL) {
        printf("Failed to load OBJ file into OBJ struct!\n");
        return 0;
    }

    printf("%s", o->name);
    printv(o->vertices, o->vertices_size);
    printv(o->normals, o->normals_size);
    printvi(o->faces, o->vertex_per_face, o->faces_size);

    // Vec3 n = (Vec3) add(o->normals[0], o->vertices[0]);
    // printf("%f, %f, %f\n", n.x, n.y, n.z);

    free(o);

    return 0;
}