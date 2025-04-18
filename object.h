#include "vec.h"

#ifndef OBJECT_H
#define OBJECT_H

typedef struct OBJ {
    char* name;
    int vertices_size;
    Vec3* vertices;
    int normals_size;
    Vec3* normals;
    int faces_size;
    Vec3I* faces;
    int* vertex_per_face;
} OBJ;

OBJ* read_obj(char*);

void printv(Vec3*, int);

void printvi(Vec3I*, int*, int);

#endif