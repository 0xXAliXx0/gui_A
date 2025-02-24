#include </home/ali/SDL/include/SDL.h>
#include "/home/ali/git_proj/gui_A/ahhhh/glad/include/glad/glad.h"    // Include glad header after GLFW
#include <stdio.h>   // Include stdio.h for printf
#include <stdbool.h>
#include <stdlib.h>
#include "shader_utils.h"

//#include <SDL_mixer.h>
 float vertices[] = {
         0.45f,  -0.9f, 0.0f,  // top right
         0.45f, -0.95f, 0.0f,  // bottom right
        -0.5f, -0.95f, 0.0f,  // bottom left
        -0.5f,  -0.9f, 0.0f   // top left
    };

    // Define index data
    GLuint indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };


// Function to handle window resizing
// Function to handle keyboard input



int main(int argc, char *argv[]) {
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    // Set OpenGL version and profile
 
    // Set OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // OpenGL 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // Core profile
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // Enable double buffering
     // Create an SDL window
    SDL_Window *window = SDL_CreateWindow(
        "OpenGL with SDL",                  // Window title
        SDL_WINDOWPOS_CENTERED,             // X position
        SDL_WINDOWPOS_CENTERED,             // Y position
        800, 600,                           // Width, Height
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN // Flags (OpenGL + visible)
    ); // Define vertex data
       if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
      SDL_GLContext glContext = SDL_GL_CreateContext(window); // Declare this
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("Failed to initialize OpenGL loader\n");
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;    }

    // Set the viewport and resize callback
    glViewport(0, 0, 800, 600);
 

       // Main loop flag
      // Load shaders from files
    char* vertexShaderSource = loadShaderFile("shaders/vertex.glsl");
    char* fragmentShaderSource = loadShaderFile("shaders/fragment.glsl");

    if (!vertexShaderSource || !fragmentShaderSource) {
        printf("Failed to load shader files\n");
        if (vertexShaderSource) free(vertexShaderSource);
        if (fragmentShaderSource) free(fragmentShaderSource);
        SDL_Quit();
        return -1;
    }

    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar* const*)&vertexShaderSource, NULL); 
    glCompileShader(vertexShader);

    // Check for vertex shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex shader compilation failed: %s\n", infoLog);
    }

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar* const*)&fragmentShaderSource, NULL); 
    glCompileShader(fragmentShader);

    // Check for fragment shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Fragment shader compilation failed: %s\n", infoLog);
    }

    // Create shader program and link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for shader program linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Shader program linking failed: %s\n", infoLog);
    }

    // Delete shaders as they're linked into the program now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Free shader source memory
    free(vertexShaderSource);
    free(fragmentShaderSource);
    // Set the uniform value (after the shader program is linked)
    glUseProgram(shaderProgram);
 
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Main loop
     // Main loop
    int running = 1;
    SDL_Event event;
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }        // Clear the screen
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind VAO and draw
        glUseProgram(shaderProgram); // Ensure the shader is active

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
      SDL_GL_SwapWindow(window);






}


    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

      // Clean up
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;}

