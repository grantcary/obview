#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"
#include "vec.h"

#define max(a, b) a > b ? a : b

Group* group_init(int size) {
    Group* group = (Group*) malloc(sizeof(Group));

    group->normals_size = size;
    group->normals = (Vec3*) malloc(size * sizeof(Vec3));
    
    group->faces_size = size;
    group->faces = (Vec3I*) malloc(size * sizeof(Vec3I));
    group->vertices_per_face = (int*) malloc(size * sizeof(int));
    
    return group;
}

OBJ* obj_init(int size) {
    OBJ* object = (OBJ*) malloc(sizeof(OBJ));
    
    object->vertices_size = size;
    object->vertices = (Vec3*) malloc(size * sizeof(Vec3));

    object->groups_size = max(10, (int) (size / 10));
    object->groups = (Group**) malloc(object->groups_size * sizeof(Group));

    return object;
}

Group* group_add(OBJ* object, Group* group, int group_index, int size) {
    if (group_index > object->groups_size) {
        object->groups_size *= 2;
        object->groups = (Group**) realloc(object->groups, object->groups_size * sizeof(Group));
    }
    object->groups[group_index] = group;

    return group_init(size);
}

void consolidate(OBJ* object, int g, int v, int vn, int f) {
    object->vertices_size = v;
    object->vertices = (Vec3*) realloc(object->vertices, v * sizeof(Vec3));

    // consolidate groups
    for (int i = 0; i < g + 1; i++) {
        object->groups[i]->normals_size = vn;
        object->groups[i]->normals = (Vec3*) realloc(object->groups[i]->normals, vn * sizeof(Vec3));
        
        object->groups[i]->faces_size = f;
        object->groups[i]->vertices_per_face = (int*) realloc(object->groups[i]->vertices_per_face, f * sizeof(int));
        object->groups[i]->faces = (Vec3I*) realloc(object->groups[i]->faces, f * sizeof(Vec3I));
        for (int j = 0; j < object->groups[i]->faces_size; j++) {
            object->groups[i]->faces[j].vec = (int*) realloc(object->groups[i]->faces[j].vec, object->groups[i]->vertices_per_face[j] * sizeof(int));
        }   
    }
}

void string_to_Vec3(Vec3* arr, int* arr_size, int *p, char* line, int i) {
    int j = 0, k = 0;
    char num[64];
    for (; line[i] != '\0' && j < 64; i++) {
        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\r') {
            num[j++] = line[i];
        }
        if ((line[i] == ' ' || line[i] == '\n' || line[i] == '\r' || line[i+1] == '\0') && j > 0) {
            if (*p > *arr_size) {
                *arr_size *= 2;
                arr = (Vec3*) realloc(arr, *arr_size * sizeof(Vec3));
            }

            num[j] = '\0';
            switch (k) {
                case 0: arr[*p].x = atof(num);
                case 1: arr[*p].y = atof(num);
                case 2: arr[*p].z = atof(num);
            }
            memset(num, 0, sizeof(num));
            j = 0;
            k++;
        }
    }
    ++(*p);
}

void string_to_Vec3I(Vec3I* arr, int* arr2, int* arr_size, int* p, char* line, int i) {
    arr[*p].vec = (int*) calloc(3, sizeof(int));
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
            if (*p > *arr_size) {
                *arr_size *= 2;
                arr = (Vec3I*) realloc(arr, *arr_size * sizeof(Vec3I));
                arr2 = (int*) realloc(arr2, *arr_size * sizeof(int));
            } else if (k > arr2[*p]){
                arr[*p].vec = (int*) realloc(arr[*p].vec, arr2[*p] * 2 * sizeof(int));
            }

            num[j] = '\0';
            arr[*p].vec[k] = atoi(num);
            arr2[*p]++;
            memset(num, 0, sizeof(num));
            j = 0;
            k++;
        }
    }
    ++(*p);
}

OBJ* read_obj(char* filename) {
    FILE* raw_obj = fopen(filename, "r");

    if (raw_obj == NULL) {
        perror("Error opening file");
        return NULL;
    }

    int size = 100;

    OBJ* object = obj_init(size);
    
    Group* group = group_init(size);
    int group_index = 0;
    
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
            if (object->name == NULL) {
                int j = i;
                object->name = (char*) malloc((strlen(line) - i) * sizeof(char));
                for (; i < strlen(line); i++) {
                    object->name[i - j] = line[i];
                }
                object->name[i - j] = '\0';
            }
        }
        else if (strcmp(keyword, "v") == 0) {
            string_to_Vec3(object->vertices, &object->vertices_size, &v, line, i);
        }
        else if (strcmp(keyword, "vn") == 0) {
            string_to_Vec3(group->normals, &group->normals_size, &vn, line, i);
        }
        else if (strcmp(keyword, "f") == 0) {
            string_to_Vec3I(group->faces, group->vertices_per_face, &group->faces_size, &f, line, i);
        }
    }

    group_add(object, group, group_index, size);

    consolidate(object, group_index, v, vn, f);


    return object;
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