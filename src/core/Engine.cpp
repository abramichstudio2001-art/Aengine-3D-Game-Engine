#include "core/Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <cstring>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

Engine::Engine()
    : window(1280, 720, "3D Game Engine"),
      camera(glm::vec3(0.0f, 4.0f, 8.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -25.0f),
      deltaTime(0.0f),
      lastFrame(0.0f),
      currentFps(0.0f),
      frameCount(0),
      fpsTimer(0.0f),
      screenshotMode(false),
      screenshotPath(""),
      targetFrames(30),
      totalFramesRendered(0) {
}

Engine::~Engine() {
}

void Engine::setScreenshotMode(const std::string& filename, int frames) {
    screenshotMode = true;
    screenshotPath = filename;
    targetFrames = frames;
}

bool Engine::init() {
    if (!window.init()) {
        std::cerr << "Engine window initialization failed!" << std::endl;
        return false;
    }

    renderer.init();
    ui.init(window.handle);

    try {
        shader = std::make_unique<Shader>("shaders/default.vert", "shaders/default.frag");
    } catch (const std::exception& e) {
        std::cerr << "Failed to load shaders: " << e.what() << std::endl;
        return false;
    }

    // Create 3D Platform Mesh (Width: 12, Depth: 12, Height: 0.4)
    platformMesh = std::make_unique<Mesh>(Mesh::createPlatform(12.0f, 12.0f, 0.4f));

    // Create a 3D Cube Mesh sitting on top of the platform
    cubeMesh = std::make_unique<Mesh>(Mesh::createCube(1.5f));

    lastFrame = static_cast<float>(glfwGetTime());
    return true;
}

void Engine::processInput() {
    if (glfwGetKey(window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window.handle, true);

    if (glfwGetKey(window.handle, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window.handle, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window.handle, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window.handle, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window.handle, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window.handle, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void Engine::updateTiming() {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    frameCount++;
    totalFramesRendered++;
    fpsTimer += deltaTime;
    if (fpsTimer >= 0.2f || currentFps == 0.0f) { // Rapid FPS calculation for early frames
        if (fpsTimer > 0.0f) {
            currentFps = static_cast<float>(frameCount) / fpsTimer;
        }
        frameCount = 0;
        fpsTimer = 0.0f;
    }
}

void Engine::saveScreenshot(const std::string& filename) {
    int w = window.width;
    int h = window.height;
    std::vector<unsigned char> pixels(3 * w * h);
    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

    std::vector<unsigned char> flipped(3 * w * h);
    for (int y = 0; y < h; ++y) {
        std::memcpy(&flipped[(h - 1 - y) * w * 3], &pixels[y * w * 3], w * 3);
    }

    if (stbi_write_png(filename.c_str(), w, h, 3, flipped.data(), w * 3)) {
        std::cout << "Successfully saved screenshot to: " << filename << std::endl;
    } else {
        std::cerr << "Failed to write screenshot to: " << filename << std::endl;
    }
}

void Engine::run() {
    while (!window.shouldClose()) {
        updateTiming();
        processInput();

        renderer.clear(0.08f, 0.09f, 0.13f, 1.0f);

        float aspectRatio = window.getAspectRatio();

        // 1. Render Platform (Slate grey platform with grid shader effect)
        glm::mat4 platformModel = glm::mat4(1.0f);
        platformModel = glm::translate(platformModel, glm::vec3(0.0f, 0.0f, 0.0f));
        renderer.renderMesh(*platformMesh, *shader, camera, aspectRatio, platformModel, glm::vec3(0.35f, 0.45f, 0.55f), true);

        // 2. Render 3D Cube sitting on platform
        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, glm::vec3(0.0f, 0.95f, 0.0f));
        cubeModel = glm::rotate(cubeModel, static_cast<float>(glfwGetTime()) * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        renderer.renderMesh(*cubeMesh, *shader, camera, aspectRatio, cubeModel, glm::vec3(0.9f, 0.4f, 0.2f), false);

        // 3. Render Minimal UI Overlay (FPS only)
        ui.beginFrame();
        ui.render(currentFps);
        ui.endFrame();

        window.swapBuffers();
        window.pollEvents();

        if (screenshotMode && totalFramesRendered >= targetFrames) {
            saveScreenshot(screenshotPath);
            break;
        }
    }
}
