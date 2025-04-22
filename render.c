#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"

int main() {
    char* filename = "test.obj";
    OBJ* o = read_obj(filename);

    if (o == NULL) {
        printf("Failed to load OBJ file into OBJ struct!\n");
        return 0;
    }

    Group** gp = o->groups;

    // printf("%s", o->name);
    printvertex(o->vertices, o->vertices_size);
    printvertexnormals(o->normals, o->normals_size);
    printface(gp[0]->faces, gp[0]->faces_size);

    // Vec3 n = (Vec3) add(o->normals[0], o->vertices[0]);
    // printf("%f, %f, %f\n", n.x, n.y, n.z);

    free(o);

    return 0;
}