#ifndef ENGINE_H
#define ENGINE_H

#include "core/Window.h"
#include "core/Camera.h"
#include "renderer/Renderer.h"
#include "renderer/Shader.h"
#include "renderer/Mesh.h"
#include "ui/UI.h"

#include <memory>
#include <string>

class Engine {
public:
    Engine();
    ~Engine();

    bool init();
    void run();
    void setScreenshotMode(const std::string& filename, int frames = 30);

private:
    void processInput();
    void updateTiming();
    void saveScreenshot(const std::string& filename);

    Window window;
    Camera camera;
    Renderer renderer;
    UI ui;

    std::unique_ptr<Shader> shader;
    std::unique_ptr<Mesh> platformMesh;
    std::unique_ptr<Mesh> cubeMesh;

    float deltaTime;
    float lastFrame;
    float currentFps;
    int frameCount;
    float fpsTimer;

    bool screenshotMode;
    std::string screenshotPath;
    int targetFrames;
    int totalFramesRendered;
};

#endif
