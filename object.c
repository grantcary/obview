#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"

#define max(a, b) ((a) > (b) ? (a) : (b))


OBJ* obj_init(int size) {
    OBJ* object = malloc(sizeof(OBJ));
    
    object->vertices_size = size;
    object->vertices = malloc(size * sizeof(Vertex));
    
    object->normals_size = size;
    object->normals = malloc(size * sizeof(Vertex_Normal));
    
    object->groups_size = (int) max(10, (int) (size / 10));
    object->groups = malloc(object->groups_size * sizeof(Group*));
    
    return object;
}

Group* group_init(int size) {
    Group* group = malloc(sizeof(Group));

    group->faces_size = size;
    group->faces = malloc(size * sizeof(Face));

    for (int i = 0; i < size; i++) {
        group->faces[i].vertices_count = 3;
        group->faces[i].vertices = calloc(3, sizeof(int));
        group->faces[i].vertex_texture_count = 3;
        group->faces[i].vertex_texture = calloc(3, sizeof(int));
        group->faces[i].vertex_normals_count = 3;
        group->faces[i].vertex_normals = calloc(3, sizeof(int));
    }
    
    return group;
}

void consolidate_group(Group* g, int f) {
    g->faces_size = f;
    g->faces = realloc(g->faces, f * sizeof(Face));
    for (int j = 0; j < g->faces_size; j++) {
        g->faces[j].vertices = realloc(g->faces[j].vertices, g->faces[j].vertices_count * sizeof(int));
        g->faces[j].vertex_texture = realloc(g->faces[j].vertex_texture, g->faces[j].vertex_texture_count * sizeof(int));
        g->faces[j].vertex_normals = realloc(g->faces[j].vertex_normals, g->faces[j].vertex_normals_count * sizeof(int));
    }   
}

void group_add(OBJ* object, Group* group, int group_index, int f) {
    consolidate_group(group, f);
    if (group_index >= object->groups_size) {
        object->groups_size *= 2;
        object->groups = realloc(object->groups, object->groups_size * sizeof(Group*));
    }
    object->groups[group_index] = group;
}

void consolidate(OBJ* object, int g, int v, int vn) {
    object->vertices_size = v;
    object->vertices = realloc(object->vertices, v * sizeof(Vertex));

    object->normals_size = vn;
    object->normals = realloc(object->normals, vn * sizeof(Vertex_Normal));
    
    object->groups_size = g + 1;
}

void stov(Vertex* v, char* line, int i) {
    int j = 0, k = 0;
    char num[64];
    for (; line[i] != '\0' && j < 64; i++) {
        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\r') {
            num[j++] = line[i];
        }
        if ((line[i] == ' ' || line[i] == '\n' || line[i] == '\r' || line[i+1] == '\0') && j > 0) {
            num[j] = '\0';
            switch (k) {
                case 0: v->x = atof(num); break;
                case 1: v->y = atof(num); break;
                case 2: v->z = atof(num); break;
                case 3: v->w = atof(num); break;
            }
            memset(num, 0, sizeof(num));
            j = 0;
            k++;
        }
    }
    
    if (k != 4) {
        v->w = 1.0;
    }
}

void stovn(Vertex_Normal* vn, char* line, int x) {
    int y = 0, z = 0;
    char num[64];
    for (; line[x] != '\0' && y < 64; x++) {
        if (line[x] != ' ' && line[x] != '\n' && line[x] != '\r') {
            num[y++] = line[x];
        }
        if ((line[x] == ' ' || line[x] == '\n' || line[x] == '\r' || line[x+1] == '\0') && y > 0) {
            num[y] = '\0';
            switch (z) {
                case 0: vn->i = atof(num);
                case 1: vn->j = atof(num);
                case 2: vn->k = atof(num);
            }
            memset(num, 0, sizeof(num));
            y = 0;
            z++;
        }
    }
}

void stof(Face* face, char* line, int i) {
    int j = 0, k = 0, l = 0, m = 0;
    char num[64];
    int last = 0;
    for (; line[i - 1] != '\0' && j < 64; i++) {
        if (line[i] == ' ') {
            last = 0;   
        }
        else if (line[i] == '/' || line[i] == '\0') {
            last++;
        } 
        
        if (line[i] != ' ' && line[i] != '\n' && line[i] != '\r' && line[i] != '/') {
            num[j++] = line[i];
        }

        if (last == 0 && (line[i + 1] == ' ' || line[i + 1] == '/' || line[i + 1] == '\n' || line[i + 1] == '\r' || line[i + 1] == '\0') && j > 0) {
            if (k > face->vertices_count){
                face->vertices_count *= 2;
                face->vertices = realloc(face->vertices, face->vertices_count * sizeof(int));
            }
            
            num[j] = '\0';
            face->vertices[k] = atoi(num);
            memset(num, 0, sizeof(num));
            j = 0;
            k++;
        }
        else if (last == 1 && line[i + 1] == '/' && j > 0) {
            if (l > face->vertex_texture_count){
                face->vertex_texture_count *= 2;
                face->vertex_texture = realloc(face->vertex_texture, face->vertex_texture_count * sizeof(int));
            }

            num[j] = '\0';
            face->vertex_texture[l] = atoi(num);
            memset(num, 0, sizeof(num));
            j = 0;
            l++;
        }
        else if (last == 2 && (line[i + 1] == ' ' || line[i + 1] == '\n' || line[i + 1] == '\r' || line[i + 1] == '\0') && j > 0) {
            if (l > face->vertex_normals_count){
                face->vertex_normals_count *= 2;
                face->vertex_normals = realloc(face->vertex_normals, face->vertex_normals_count * sizeof(int));
            }

            num[j] = '\0';
            face->vertex_normals[m] = atoi(num);
            memset(num, 0, sizeof(num));
            j = 0;
            m++;
        }
        // printf("%d %d %d\n", k, l, m);
    }
    
    face->vertices_count = k;
    face->vertices = (int*) realloc(face->vertices, k * sizeof(int));
    face->vertex_texture_count = l;
    face->vertex_texture = (int*) realloc(face->vertex_texture, l * sizeof(int));
    face->vertex_normals_count = m;
    face->vertex_normals = (int*) realloc(face->vertex_normals, m * sizeof(int));
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

    int v = 0, vt = 0, vn = 0, f = 0;
    
    char line[256];
    
    while (fgets(line, sizeof(line), raw_obj)) {
        // grabs keyword pattern from line
        int pattern_limit = 6;
        char* keyword = malloc(pattern_limit * sizeof(char));
        int i = 0;
        for (; line[i] != ' ' && i < pattern_limit; i++) {
            keyword[i] = line[i];
        }
        keyword[i] = '\0';
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
        else if (strcmp(keyword, "g") == 0) {
            group_add(object, group, group_index, f);
            f = 0;
            group_index++;
            group = group_init(size);
        }
        else if (strcmp(keyword, "v") == 0) {
            if (v >= object->vertices_size) {
                object->vertices_size = v * 2;
                object->vertices = realloc(object->vertices, object->vertices_size * sizeof(Vertex));
            }
            
            stov(&object->vertices[v], line, i);
            v++;
        }
        else if (strcmp(keyword, "vn") == 0) {
            if (vn >= object->normals_size) {
                object->normals_size = vn * 2;
                object->normals = realloc(object->normals, object->normals_size * sizeof(Vertex_Normal));
            }
            
            stovn(&object->normals[vn], line, i);
            vn++;
        }
        else if (strcmp(keyword, "f") == 0) {
            if (f >= group->faces_size) {
                group->faces_size = f * 2;
                group->faces = realloc(group->faces, group->faces_size * sizeof(Face));
            }
        
            stof(&group->faces[f], line, i);
            f++;
        }
        
        free(keyword);
    }
    group_add(object, group, group_index, f);
    f = 0;

    consolidate(object, group_index, v, vn);


    return object;
}

void printvertex(Vertex* arr, int arr_size) {
    for (int i = 0; i < arr_size; i++) {
        printf("%d [%f, %f, %f, %f]\n", i, arr[i].x, arr[i].y, arr[i].z, arr[i].w);
    }
}

void printvertexnormals(Vertex_Normal* arr, int arr_size) {
    for (int x = 0; x < arr_size; x++) {
        printf("%d [%f, %f, %f]\n", x, arr[x].i, arr[x].j, arr[x].k);
    }
}

void printface(Face* arr, int arr_size) {
    for (int i = 0; i < arr_size; i++) {
        for (int j = 0; j < arr[i].vertices_count; j++) {
            printf("%d ", arr[i].vertices[j]);
        }
        printf("\n");
        for (int j = 0; j < arr[i].vertex_texture_count; j++) {
            printf("%d ", arr[i].vertex_texture[j]);
        }
        printf("\n");
        for (int j = 0; j < arr[i].vertex_normals_count; j++) {
            printf("%d ", arr[i].vertex_normals[j]);
        }
        printf("\n\n");
    }
}