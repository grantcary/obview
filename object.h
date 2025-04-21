#ifndef OBJECT_H
#define OBJECT_H

typedef struct Vertex {
    float x, y, z, w;
} Vertex;

typedef struct Vertex_Normal {
    float i, j, k;
} Vertex_Normal;

typedef struct Vertex_Texture {
    float u, v, w;
} Vertex_Texture;

typedef struct Face {
    int* vertices;
    int* vertex_texture;
    int* vertex_normals;
} Face;

typedef struct Group {
    char** name;

    int faces_size;
    Face* faces;
    int* vertices_per_face;
} Group;

typedef struct OBJ {
    char* name;
    
    int vertices_size;
    Vertex* vertices;
    
    int normals_size;
    Vertex_Normal* normals;

    int groups_size;
    Group** groups;
} OBJ;

OBJ* read_obj(char*);

void printvertex(Vertex*, int);

void printvertexnormals(Vertex_Normal*, int);

void printface(Face*, int*, int);

#endif