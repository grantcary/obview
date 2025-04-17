typedef struct Vec3 {
    float x, y, z;
} Vec3;

typedef struct Vec3I {
    int x, y, z;
} Vec3I;

typedef struct OBJ {
    int vertices_size;
    Vec3* vertices;
    int faces_size;
    Vec3I* faces;
    int normals_size;
    Vec3* normals;
} OBJ;
