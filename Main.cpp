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
    Shader shadowShader("Shaders/shadowMap.vert", "Shaders/shadowMap.frag");

    Camera camera(Constants::windowWidth, Constants::windowHeight, glm::vec3(0.0f, 0.0f, 2.0f));
    WorldGeneration world(20);

#pragma region Shadow Map
    constexpr int shadowResolution = 7680;
    constexpr float shadowHalfExtent = 256.0f;
    const glm::vec3 sunDirection = glm::normalize(glm::vec3(-1.0f, -2.0f, -1.0f));

    GLuint shadowFBO = 0;
    GLuint shadowDepthTexture = 0;

    glGenFramebuffers(1, &shadowFBO);
    glGenTextures(1, &shadowDepthTexture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowResolution, shadowResolution, 0, GL_DEPTH_COMPONENT,
                 GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    const float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowDepthTexture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);

#pragma endregion

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

        int frameBufferWidth;
        int frameBufferHeight;
        glfwGetWindowSize(window, &frameBufferWidth, &frameBufferHeight);

        camera.width = frameBufferWidth;
        camera.height = frameBufferHeight;

        camera.Inputs(window, deltaTime);
        camera.UpdateMatrix(45.0f, 0.1f, 1000.0f);
        world.Update(camera);

        const float worldUnitPerTexel = (2.0f * shadowHalfExtent) / static_cast<float>(shadowResolution);
        const glm::mat4 lightRotation = glm::lookAt(glm::vec3(0.0f), sunDirection, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 centerLightSpace = glm::vec3(lightRotation * glm::vec4(camera.Position, 1.0f));

        centerLightSpace.x = std::round(centerLightSpace.x / worldUnitPerTexel) * worldUnitPerTexel;
        centerLightSpace.y = std::round(centerLightSpace.y / worldUnitPerTexel) * worldUnitPerTexel;

        const glm::mat4 lightView = glm::translate(glm::mat4(1.0f),
                                                   glm::vec3(
                                                       -centerLightSpace.x,
                                                       -centerLightSpace.y,
                                                       -centerLightSpace.z - 512.0f
                                                   )) * lightRotation;
        
        const glm::mat4 lightProjection = glm::ortho(-shadowHalfExtent, shadowHalfExtent, -shadowHalfExtent,
                                                     shadowHalfExtent, 1.0f, 1024.0f);
        const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        //Render Pass 1: render from the sun's POV
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glViewport(0, 0, shadowResolution, shadowResolution);
        glClear(GL_DEPTH_BUFFER_BIT);

        shadowShader.Activate();
        shadowShader.SetMat4("lightProjection", lightSpaceMatrix);

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(2.0f, 4.0f);
        
        world.Render(shadowShader, camera);
        
        glDisable(GL_POLYGON_OFFSET_FILL);

        //Render Pass 2: render from the player's POV
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, frameBufferWidth, frameBufferHeight);

        //Sky color
        glClearColor(0.482f, 0.714f, 0.91f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();
        shaderProgram.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
        shaderProgram.SetVec3("directionalLight", sunDirection);
        shaderProgram.SetVec3("cameraPosition", camera.Position);
        shaderProgram.SetInt("shadowMap", 1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadowDepthTexture);

        world.Render(shaderProgram, camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
#pragma endregion

#pragma region Delete and Terminate
    world.Delete();
    shaderProgram.Delete();
    shadowShader.Delete();
    glDeleteTextures(1, &shadowDepthTexture);
    glDeleteFramebuffers(1, &shadowFBO);
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
