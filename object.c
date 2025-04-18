#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"
#include "vec.h"

void obj_init(OBJ* new_object, int size) {
    new_object->vertices_size = size;
    new_object->vertices = (Vec3*) calloc(size, sizeof(Vec3));
    new_object->normals_size = size;
    new_object->normals = (Vec3*) calloc(size, sizeof(Vec3));
    new_object->faces_size = size;
    new_object->faces = (Vec3I*) calloc(size, sizeof(Vec3I));
    new_object->vertex_per_face = (int*) calloc(size, sizeof(int));
}

void consolidate(OBJ* new_object, int v, int vn, int f) {
    new_object->vertices_size = v;
    new_object->vertices = (Vec3*) realloc(new_object->vertices, v * sizeof(Vec3));
    new_object->normals_size = vn;
    new_object->normals = (Vec3*) realloc(new_object->normals, vn * sizeof(Vec3));
    new_object->faces_size = f;
    new_object->vertex_per_face = (int*) realloc(new_object->vertex_per_face, f * sizeof(int));
    new_object->faces = (Vec3I*) realloc(new_object->faces, f * sizeof(Vec3I));
    for (int i = 0; i < new_object->faces_size; i++) {
        new_object->faces[i].vec = (int*) realloc(new_object->faces[i].vec, new_object->vertex_per_face[i] * sizeof(int));
    }   
}

int string_to_Vec3(Vec3* arr, int* arr_size, int p, char* line, int i) {
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

int string_to_Vec3I(Vec3I* arr, int* arr2, int* arr_size, int p, char* line, int i) {
    arr[p].vec = (int*) calloc(3, sizeof(int));
    int j = 0, k = 0;
    char num[64];
    int slash_encountered = 0;
    for (; line[i] != '\0' && j < 64; i++) {
        // accumulates the first number, detects if a slash has been encountered, the rest is skipped until next space
        if (line[i] == ' ' || line[i + 1] == '\0') {
            slash_encountered = 0;
        } else if (line[i] == '/') {
            slash_encountered = 1;
        }
        
        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\r' && slash_encountered == 0) {
            num[j++] = line[i];
        }
        // get only first integer from group
        if (slash_encountered == 1 && j > 0) {
            if (p > *arr_size) {
                *arr_size *= 2;
                arr = (Vec3I*) realloc(arr, *arr_size * sizeof(Vec3I));
                arr2 = (int*) realloc(arr2, *arr_size * sizeof(int));
            } else if (k > arr2[p]){
                arr[p].vec = (int*) realloc(arr[p].vec, arr2[p] * 2 * sizeof(int));
            }

            num[j] = '\0';
            arr[p].vec[k] = atoi(num);
            arr2[p]++;
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
    obj_init(new_object, 100);

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
            if (new_object->name == NULL) {
                int j = i;
                new_object->name = (char*) malloc((strlen(line) - i) * sizeof(char));
                for (; i < strlen(line); i++) {
                    new_object->name[i - j] = line[i];
                }
                new_object->name[i - j] = '\0';
            }
        }
        else if (strcmp(keyword, "v") == 0) {
            v = string_to_Vec3(new_object->vertices, &new_object->vertices_size, v, line, i);
        }
        else if (strcmp(keyword, "vn") == 0) {
            vn = string_to_Vec3(new_object->normals, &new_object->normals_size, vn, line, i);
        }
        else if (strcmp(keyword, "f") == 0) {
            f = string_to_Vec3I(new_object->faces, new_object->vertex_per_face, &new_object->faces_size, f, line, i);
        }
    }
    consolidate(new_object, v, vn, f);

    return new_object;
}

void printv(Vec3* arr, int arr_size) {
    for (int i = 0; i < arr_size; i++) {
        printf("%d [%f, %f, %f]\n", i, arr[i].x, arr[i].y, arr[i].z);
    }
}

void printvi(Vec3I* arr, int* arr2, int arr_size) {
    for (int i = 0; i < arr_size; i++) {
        for (int j = 0; j < arr2[i]; j++) {
            printf("%d ", arr[i].vec[j]);
        }
        printf("\n");
    }
}