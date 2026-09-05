#include "core/Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <cstring>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

Engine::Engine()
    : window(1280, 720, "3D Game Engine"),
      camera(glm::vec3(0.0f, 5.0f, 9.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -25.0f),
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
        shadowShader = std::make_unique<Shader>("shaders/shadow.vert", "shaders/shadow.frag");
    } catch (const std::exception& e) {
        std::cerr << "Failed to load shaders: " << e.what() << std::endl;
        return false;
    }

    // Create 3D Platform Mesh (Width: 12, Depth: 12, Height: 0.4)
    platformMesh = std::make_unique<Mesh>(Mesh::createPlatform(12.0f, 12.0f, 0.4f));

    // Create 3D Cube Mesh
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
    if (fpsTimer >= 0.2f || currentFps == 0.0f) {
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

        float time = static_cast<float>(glfwGetTime());

        // Define scene object transformations
        glm::mat4 platformModel = glm::mat4(1.0f);

        glm::mat4 mainCubeModel = glm::mat4(1.0f);
        mainCubeModel = glm::translate(mainCubeModel, glm::vec3(0.0f, 0.95f, 0.0f));
        mainCubeModel = glm::rotate(mainCubeModel, time * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 emeraldCubeModel = glm::mat4(1.0f);
        emeraldCubeModel = glm::translate(emeraldCubeModel, glm::vec3(-2.8f, 0.65f, 1.2f));
        emeraldCubeModel = glm::rotate(emeraldCubeModel, time * -0.4f, glm::vec3(0.0f, 1.0f, 0.0f));
        emeraldCubeModel = glm::scale(emeraldCubeModel, glm::vec3(0.6f));

        glm::mat4 goldCubeModel = glm::mat4(1.0f);
        goldCubeModel = glm::translate(goldCubeModel, glm::vec3(2.5f, 0.8f, -1.5f));
        goldCubeModel = glm::rotate(goldCubeModel, time * 0.7f, glm::vec3(1.0f, 1.0f, 0.0f));
        goldCubeModel = glm::scale(goldCubeModel, glm::vec3(0.8f));

        // PASS 1: Render Depth to Shadow Map
        renderer.beginShadowPass(*shadowShader);
        renderer.renderMeshToShadowMap(*platformMesh, *shadowShader, platformModel);
        renderer.renderMeshToShadowMap(*cubeMesh, *shadowShader, mainCubeModel);
        renderer.renderMeshToShadowMap(*cubeMesh, *shadowShader, emeraldCubeModel);
        renderer.renderMeshToShadowMap(*cubeMesh, *shadowShader, goldCubeModel);
        renderer.endShadowPass(window.width, window.height);

        // PASS 2: Render Main Scene with Lighting, Materials, and PCF Soft Shadows
        renderer.clear(0.05f, 0.06f, 0.09f, 1.0f);
        float aspectRatio = window.getAspectRatio();

        // Render Platform
        renderer.renderMeshWithShadows(*platformMesh, *shader, camera, aspectRatio, platformModel, Material::SlatePlatform(), true);

        // Render Main Cube (Vibrant Orange Material)
        renderer.renderMeshWithShadows(*cubeMesh, *shader, camera, aspectRatio, mainCubeModel, Material::VibrantOrange(), false);

        // Render Emerald Cube
        renderer.renderMeshWithShadows(*cubeMesh, *shader, camera, aspectRatio, emeraldCubeModel, Material::Emerald(), false);

        // Render Gold Cube
        renderer.renderMeshWithShadows(*cubeMesh, *shader, camera, aspectRatio, goldCubeModel, Material::Gold(), false);

        // PASS 3: Render Minimal UI Overlay (FPS ONLY)
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
