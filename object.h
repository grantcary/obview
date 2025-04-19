#include "vec.h"

#ifndef OBJECT_H
#define OBJECT_H

typedef struct Group {
    char* name;
    int normals_size;
    Vec3* normals;
    int faces_size;
    Vec3I* faces;
    int* vertices_per_face;
} Group;

typedef struct OBJ {
    char* name;
    
    int vertices_size;
    Vec3* vertices;
    
    int groups_size;
    Group** groups;
} OBJ;

OBJ* read_obj(char*);

void printv(Vec3*, int);

void printvi(Vec3I*, int*, int);

#endif