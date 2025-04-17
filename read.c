#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vec.h"

void read_obj(char* filename) {
    FILE* raw_obj = fopen(filename, "r");

    char line[256];
    
    while (fgets(line, sizeof(line), raw_obj)) {
        int pattern_size = 6;
        char* pattern = malloc(pattern_size * sizeof(char));
        int i = 0;
        for (; line[i] != ' ' && i < pattern_size; i++) {
            pattern[i] = line[i];
        }
        // printf("%s\n", pattern);
        if (strcmp(pattern, "v") == 0) {
            printf("%d %s", i, line);
        }
        else if (strcmp(pattern, "vn") == 0) {
            printf("%d %s", i, line);
        }
        else if (strcmp(pattern, "f") == 0) {
            printf("%d %s", i, line);
        }
    }
    printf("\n");
}


int main() {
    char* filename = "cube.obj";
    read_obj(filename);
    
    // Vec3 a = {0.0, 0.1, 0.2};

    return 0;
}
