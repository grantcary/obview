typedef struct Vec3 {
    float x, y, z;
} Vec3;

typedef struct Vec3I {
    int x, y, z;
} Vec3I;

typedef struct OBJ {
    char* name;
    int vertices_size;
    Vec3* vertices;
    int normals_size;
    Vec3* normals;
    int faces_size;
    Vec3I* faces;
} OBJ;
