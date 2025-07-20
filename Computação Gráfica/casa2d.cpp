// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// GLM header file
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

// shaders header file
#include <common/shader.hpp>

// Vertex array object (VAO)
GLuint VertexArrayID;

// Vertex buffer object (VBO)
GLuint vertexbuffer;

// color buffer object (CBO)
GLuint colorbuffer;

// GLSL program from the shaders
GLuint programID;

GLint WindowWidth = 800;
GLint WindowHeight = 800;

float delta = 0.0;

//--------------------------------------------------------------------------------
void transferDataToGPUMemory(void)
{
    // VAO
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("/Users/abelgomes/Documents/myC/movinghouse/SimpleVertexShader.vertexshader", "/Users/abelgomes/Documents/myC/movinghouse/SimpleFragmentShader.fragmentshader");


    static const GLfloat g_vertex_buffer_data[] = {
        // Building Body
        -10.0f,  0.0f,  0.0f,
         10.0f,  0.0f,  0.0f,
         10.0f, 20.0f,  0.0f,
        -10.0f,  0.0f,  0.0f,
         10.0f, 20.0f,  0.0f,
        -10.0f, 20.0f,  0.0f,

        // Roof
        -12.0f, 20.0f, 0.0f,
         12.0f, 20.0f, 0.0f,
          0.0f, 30.0f, 0.0f,

          // Window 1
          -4.0f, 5.0f, 0.0f,
           0.0f, 5.0f, 0.0f,
           0.0f, 9.0f, 0.0f,
          -4.0f, 5.0f, 0.0f,
           0.0f, 9.0f, 0.0f,
          -4.0f, 9.0f, 0.0f,

          // Window 2
           4.0f, 5.0f, 0.0f,
           8.0f, 5.0f, 0.0f,
           8.0f, 9.0f, 0.0f,
           4.0f, 5.0f, 0.0f,
           8.0f, 9.0f, 0.0f,
           4.0f, 9.0f, 0.0f,

           // Door
           -2.0f, 0.0f, 0.0f,
            2.0f, 0.0f, 0.0f,
            2.0f, 8.0f, 0.0f,
           -2.0f, 0.0f, 0.0f,
            2.0f, 8.0f, 0.0f,
           -2.0f, 8.0f, 0.0f,
    };

    // Colors for each vertex
    static const GLfloat g_color_buffer_data[] = {
        // Building Body
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        // Roof
        0.3f, 0.3f, 0.3f,
        0.3f, 0.3f, 0.3f,
        0.3f, 0.3f, 0.3f,

        // Window 1
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // Window 2
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // Door
        0.5f, 0.3f, 0.0f,
        0.5f, 0.3f, 0.0f,
        0.5f, 0.3f, 0.0f,
        0.5f, 0.3f, 0.0f,
        0.5f, 0.3f, 0.0f,
        0.5f, 0.3f, 0.0f,
    };

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // Move color data to video memory; specifically to CBO called colorbuffer
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

}

//--------------------------------------------------------------------------------
void cleanupDataFromGPU()
{
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
}

//--------------------------------------------------------------------------------
void draw(void)
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Use our shader
    glUseProgram(programID);

    // Create transformations
    glm::mat4 mvp = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f);

    // Set the matrix uniform locations
    unsigned int matrix = glGetUniformLocation(programID, "mvp");
    glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);

    // Translate the building body
    glm::mat4 transBuildingBody = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f));
    unsigned int mBuildingBody = glGetUniformLocation(programID, "trans");
    glUniformMatrix4fv(mBuildingBody, 1, GL_FALSE, &transBuildingBody[0][0]);

    // Draw the building body
    glDrawArrays(GL_TRIANGLES, 0, 18);

    // Translate the roof
    glm::mat4 transRoof = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f));
    unsigned int mRoof = glGetUniformLocation(programID, "trans");
    glUniformMatrix4fv(mRoof, 1, GL_FALSE, &transRoof[0][0]);

    // Draw the roof
    glDrawArrays(GL_TRIANGLES, 18, 3);

    // Translate and draw the first window
    glm::mat4 transWindow1 = glm::translate(glm::mat4(1.0), glm::vec3(-6.0f, 5.0f, 0.0f));
    unsigned int mWindow1 = glGetUniformLocation(programID, "trans");
    glUniformMatrix4fv(mWindow1, 1, GL_FALSE, &transWindow1[0][0]);
    glDrawArrays(GL_TRIANGLES, 21, 6);

    // Translate and draw the second window
    glm::mat4 transWindow2 = glm::translate(glm::mat4(1.0), glm::vec3(6.0f, 5.0f, 0.0f));
    unsigned int mWindow2 = glGetUniformLocation(programID, "trans");
    glUniformMatrix4fv(mWindow2, 1, GL_FALSE, &transWindow2[0][0]);
    glDrawArrays(GL_TRIANGLES, 21, 6);

    // Translate and draw the door
    glm::mat4 transDoor = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, 0.0f));
    unsigned int mDoor = glGetUniformLocation(programID, "trans");
    glUniformMatrix4fv(mDoor, 1, GL_FALSE, &transDoor[0][0]);
    glDrawArrays(GL_TRIANGLES, 27, 6);

    // Disable attribute arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

//--------------------------------------------------------------------------------

int main(void)
{
    // Initialize GLFW
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window
    window = glfwCreateWindow(WindowWidth, WindowHeight, "Moving House in 2D ", NULL, NULL);

    // Create window context
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    glewInit();

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // White background
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // Transfer data (vertices, colors, and shaders) to GPU
    transferDataToGPUMemory();

    // Render the scene for each frame
    do {
        // Drawing callback
        draw();

        // Swap buffers
        glfwSwapBuffers(window);

        // Look for events
        glfwPollEvents();

        if (delta < 10)
            delta += 0.1;

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    // Clean up VAO, VBOs, and shaders from GPU
    cleanupDataFromGPU();

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
