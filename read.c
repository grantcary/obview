#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vec.h"

OBJ* read_obj(char* filename) {
    FILE* raw_obj = fopen(filename, "r");

    if (raw_obj == NULL) {
        perror("Error opening file");
        return NULL;
    }

    OBJ* new_object = (OBJ*) malloc(sizeof(OBJ));

    char line[256];
    
    while (fgets(line, sizeof(line), raw_obj)) {
        // grabs keyword pattern from line
        int pattern_size = 6;
        char* keyword = malloc(pattern_size * sizeof(char));
        int i = 0;
        for (; line[i] != ' ' && i < pattern_size; i++) {
            keyword[i] = line[i];
        }
        // skips white space
        i++;
        
        // j retains index in line after keyword characters + space
        int j = i;
        if (strcmp(keyword, "o") == 0) {
            new_object->name = (char*) malloc((strlen(line) - 1 - i) * sizeof(char));
            for (; i < strlen(line); i++) {
                new_object->name[i - j] = line[i];
            }
        }
        else if (strcmp(keyword, "v") == 0) {
            printf("%d %s", i, line);
        }
        else if (strcmp(keyword, "vn") == 0) {
            printf("%d %s", i, line);
        }
        else if (strcmp(keyword, "f") == 0) {
            printf("%d %s", i, line);
        }
    }
    printf("\n");
    return new_object;
}


int main() {
    char* filename = "cube.obj";
    OBJ* o = read_obj(filename);
    printf("%s", o->name);

    // Vec3 a = {0.0, 0.1, 0.2};

    return 0;
}
