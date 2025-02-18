// shader_utils.h
#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#include <stdio.h>
#include <stdlib.h>

// Function to load text from a file
char* loadShaderFile(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open %s\n", filename);
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate buffer (+1 for null terminator)
    char* buffer = (char*)malloc(length + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    // Read file and null-terminate
    fread(buffer, 1, length, file);
    buffer[length] = '\0';

    fclose(file);
    return buffer;
}

#endif // SHADER_UTILS_H
