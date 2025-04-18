#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"
#include "vec.h"

// Vec3* float_parse(FILE* raw_obj, char* line) {
// }

void obj_init(OBJ* new_object) {
    new_object->vertices_size = 100;
    new_object->vertices = (Vec3*) calloc(100, sizeof(Vec3));
    new_object->normals_size = 100;
    new_object->normals = (Vec3*) calloc(100, sizeof(Vec3));
    new_object->faces_size = 100;
    new_object->faces = (Vec3I*) calloc(100, sizeof(Vec3I));
}

void consolidate(OBJ* new_object, int v, int vn, int f) {
    new_object->vertices_size = v;
    new_object->vertices = (Vec3*) realloc(new_object->vertices, v * sizeof(Vec3));
    new_object->normals_size = vn;
    new_object->normals = (Vec3*) realloc(new_object->normals, vn * sizeof(Vec3));
    new_object->faces_size = f;
    new_object->faces = (Vec3I*) realloc(new_object->faces, f * sizeof(Vec3I));
}

int string_to_vec3(Vec3* arr, int* arr_size, int p, char* line, int i) {
    // printf("%s", line);
    int j = 0, k = 0;
    char num[64];
    for (; line[i] != '\0' && j < 64; i++) {
        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\r') {
            num[j++] = line[i];
        }
        if ((line[i] == ' ' || line[i] == '\n' || line[i] == '\r' || line[i+1] == '\0') && j > 0) {
            if (p > *arr_size) {
                *arr_size *= 2;
                arr = (Vec3*) realloc(arr, *arr_size * sizeof(Vec3));
            }

            num[j] = '\0';
            switch (k) {
                case 0: arr[p].x = atof(num);
                case 1: arr[p].y = atof(num);
                case 2: arr[p].z = atof(num);
            }
            memset(num, 0, sizeof(num));
            j = 0;
            k++;
        }
    }
    return ++p;
}

OBJ* read_obj(char* filename) {
    FILE* raw_obj = fopen(filename, "r");

    if (raw_obj == NULL) {
        perror("Error opening file");
        return NULL;
    }

    OBJ* new_object = (OBJ*) malloc(sizeof(OBJ));
    obj_init(new_object);

    // if (new_object->vertices == NULL) {  
    //     printf("Name is empty\n");
    // }

    int v = 0, vn = 0, f = 0;

    char line[256];

    while (fgets(line, sizeof(line), raw_obj)) {
        // grabs keyword pattern from line
        int pattern_limit = 6;
        char* keyword = malloc(pattern_limit * sizeof(char));
        int i = 0;
        for (; line[i] != ' ' && i < pattern_limit; i++) {
            keyword[i] = line[i];
        }
        // skips white space
        i++;
        
        if (strcmp(keyword, "o") == 0) {
            if (new_object->name != NULL) {
                free(new_object->name);
            }
            int j = i;
            new_object->name = (char*) malloc((strlen(line) - i) * sizeof(char));
            for (; i < strlen(line); i++) {
                new_object->name[i - j] = line[i];
            }
            new_object->name[i - j] = '\0';
        }
        else if (strcmp(keyword, "v") == 0) {
            v = string_to_vec3(new_object->vertices, &new_object->vertices_size, v, line, i);
        }
        else if (strcmp(keyword, "vn") == 0) {
            vn = string_to_vec3(new_object->normals, &new_object->normals_size, vn, line, i);
        }
        else if (strcmp(keyword, "f") == 0) {
            // printf("%s", line);
        }
    }
    printf("\n");
    printf("%d\n", v);


    consolidate(new_object, v, vn, f);

    return new_object;
}

void printv(Vec3* arr, int arr_size) {
    for (int i = 0; i < arr_size; i++) {
        printf("%d [%f, %f, %f]\n", i, arr[i].x, arr[i].y, arr[i].z);
    }
}

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

    Vec3 n = (Vec3) add(o->normals[0], o->vertices[0]);
    printf("%f, %f, %f\n", n.x, n.y, n.z);

    // Vec3 a = {0.0, 0.1, 0.2};

    return 0;
}
