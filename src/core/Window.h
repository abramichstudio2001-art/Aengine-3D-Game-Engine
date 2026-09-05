#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
    GLFWwindow* handle;
    int width;
    int height;
    std::string title;

    Window(int width = 1280, int height = 720, const std::string& title = "3D Game Engine");
    ~Window();

    bool init();
    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    float getAspectRatio() const;
};

#endif
