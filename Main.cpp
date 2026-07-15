#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include"Constants.h"
#include "Player/Camera.h"
#include "Shaders/ShaderClass.h"
#include "World/WorldGeneration.h"

void framebuffer_size_callback(GLFWwindow* window, int w, int h);

int main()
{
#pragma region GLFW Initialization
    //==================================
    // Initialize GLFW
    //==================================
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(Constants::windowWidth, Constants::windowHeight, "OpenGL Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, Constants::windowWidth, Constants::windowHeight);
#pragma endregion
    
    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag");
    Camera camera(Constants::windowWidth, Constants::windowHeight, glm::vec3(0.0f, 0.0f, 2.0f));
    WorldGeneration world(10);
    
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glfwSetWindowUserPointer(window, &camera);
    double previousFrameTime = glfwGetTime();
    
#pragma region Main Loop
    //==================================
    // Main Loop
    //==================================
    while (!glfwWindowShouldClose(window))
    {
        const double currentFrameTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrameTime - previousFrameTime);
        previousFrameTime = currentFrameTime;
        deltaTime = std::min(deltaTime, 0.1f);
        
        //Specify the color of the background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window, deltaTime);
        camera.UpdateMatrix(45.0f, 0.1f, 500.0f);

        world.Update(camera);
        world.Render(shaderProgram, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
#pragma endregion

#pragma region Delete and Terminate
    world.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
#pragma endregion

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);

    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam)
    {
        cam->width = w;
        cam->height = h;
    }
}
