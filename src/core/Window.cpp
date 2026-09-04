#include "core/Window.h"
#include <iostream>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->width = width;
        win->height = height;
    }
}

Window::Window(int width, int height, const std::string& title)
    : handle(nullptr), width(width), height(height), title(title) {
}

Window::~Window() {
    if (handle) {
        glfwDestroyWindow(handle);
    }
    glfwTerminate();
}

bool Window::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!handle) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(handle);
    glfwSetWindowUserPointer(handle, this);
    glfwSetFramebufferSizeCallback(handle, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return false;
    }

    glViewport(0, 0, width, height);
    return true;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(handle);
}

void Window::swapBuffers() {
    glfwSwapBuffers(handle);
}

void Window::pollEvents() {
    glfwPollEvents();
}

float Window::getAspectRatio() const {
    if (height == 0) return 1.0f;
    return static_cast<float>(width) / static_cast<float>(height);
}
