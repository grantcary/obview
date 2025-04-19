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

    Group** gp = o->groups;

    printf("%s", o->name);
    printv(o->vertices, o->vertices_size);
    printv(gp[0]->normals, gp[0]->normals_size);
    printvi(gp[0]->faces, gp[0]->vertices_per_face, gp[0]->faces_size);

    // Vec3 n = (Vec3) add(o->normals[0], o->vertices[0]);
    // printf("%f, %f, %f\n", n.x, n.y, n.z);

    free(o);

    return 0;
}