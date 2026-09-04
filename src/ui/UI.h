#ifndef UI_H
#define UI_H

struct GLFWwindow;

class UI {
public:
    UI() = default;
    ~UI();

    void init(GLFWwindow* window);
    void beginFrame();
    void render(float fps);
    void endFrame();
    void shutdown();
};

#endif
