
#define GLFW_INCLUDE_NONE  // Prevent GLFW from including OpenGL headers
#include "/home/ali/git_proj/gui_A/ahhhh/glad/include/glad/glad.h"    // Include glad header after GLFW
#include <GLFW/glfw3.h>  // Include GLFW header first
#include <stdio.h>   // Include stdio.h for printf
#include <stdbool.h>
#include <stdlib.h>
#include "shader_utils.h"

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
void frame_size(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Function to handle keyboard input
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int is_point_in_triangle(float px, float py, float vertices[]) {
    // Extract triangle vertices (first 3 points from the array)
    float x1 = vertices[0], y1 = vertices[1];
    float x2 = vertices[3], y2 = vertices[4];
    float x3 = vertices[6], y3 = vertices[7];
    float x3 = vertices[9], y3 = vertices[10]

    // Barycentric coordinate check
    float denominator = ((y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3));
    float a = ((y2 - y3)*(px - x3) + (x3 - x2)*(py - y3)) / denominator;
    float b = ((y3 - y1)*(px - x3) + (x1 - x3)*(py - y3)) / denominator;
    float c = 1.0f - a - b;

    return (a >= 0 && a <= 1 && b >= 0 && b <= 1 && c >= 0);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Get window size
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Convert to NDC [-1, 1]
        float mouseX = (xpos / width) * 2.0f - 1.0f;
        float mouseY = 1.0f - (ypos / height) * 2.0f;

        // Check if the click is inside the triangle
        if (is_point_in_triangle(mouseX, mouseY, vertices)) {
            printf("Button clicked!\n");
            // Trigger your button action here
        }
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
 // Define vertex data
   
  glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize OpenGL loader\n");
        return -1;
    }

    // Set the viewport and resize callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, frame_size);

   
    // Load shaders from files
    char* vertexShaderSource = loadShaderFile("shaders/vertex.glsl");
    char* fragmentShaderSource = loadShaderFile("shaders/fragment.glsl");

    if (!vertexShaderSource || !fragmentShaderSource) {
        printf("Failed to load shader files\n");
        if (vertexShaderSource) free(vertexShaderSource);
        if (fragmentShaderSource) free(fragmentShaderSource);
        glfwTerminate();
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

    // Get window size and set uniform (after linking shader program)
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    
    // Get the location of the uniform in the shader program
    GLuint resolutionLoc = glGetUniformLocation(shaderProgram, "u_resolution");

    // Set the uniform value (after the shader program is linked)
    glUseProgram(shaderProgram);
    glUniform2f(resolutionLoc, (float)windowWidth, (float)windowHeight);

    // Inside the render loop or update loop
    float currentTime = glfwGetTime();  // Get the current time in seconds (since GLFW was initialized)
    GLuint timeLocation = glGetUniformLocation(shaderProgram, "u_time");  // Get the location of the 'u_time' uniform
    glUniform1f(timeLocation, currentTime);  // Set the value of 'u_time' to the current time
     if (timeLocation == -1) {
        fprintf(stderr, "Warning: 'u_time' uniform not found in shader.\n");
      }

     
    GLint u_mouse_loc = glGetUniformLocation(shaderProgram, "u_mouse");
      if (u_mouse_loc == -1) {
        fprintf(stderr, "Warning: 'u_mouse' uniform not found in shader.\n");
      }//I think error because the variable is not used


    // Create VAO, VBO, and EBO
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
    while (!glfwWindowShouldClose(window)) {
        // Input handling
        processInput(window);

        // Clear the screen
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Inside your render loop:
double xpos, ypos;
glfwGetCursorPos(window, &xpos, &ypos); // Get mouse position

// Optional: Normalize coordinates to [0, 1] range
int width, height;
glfwGetWindowSize(window, &width, &height);
float mx = (float)xpos / width;
float my = 1.0f - (float)ypos / height; // Flip Y if needed

// Update the uniform
glUseProgram(shaderProgram); // Ensure the shader is active
glUniform2f(u_mouse_loc, mx, my);

        // Use the shader program
      //  glUseProgram(shaderProgram);

        // Bind VAO and draw
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();
    return 0;
}

