#include "shader_utils.h"
#include <stdio.h>
#include <stdlib.h>


// Function to load shader source from file
char* loadShaderFile(const char* filename) {
    FILE* file = fopen(filename, "rb"); // Open in binary mode
    if (!file) {
        printf("Failed to open %s\n", filename);
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for buffer (+1 for null terminator)
    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    // Read file contents and null-terminate
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}
