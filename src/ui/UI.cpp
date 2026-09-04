#include "ui/UI.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

UI::~UI() {
    shutdown();
}

void UI::init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = nullptr; // Disable imgui.ini file generation

    ImGui::StyleColorsDark();

    // Custom dark sleek style
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.WindowPadding = ImVec2(12, 10);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::render(float fps) {
    // Position FPS window at top-left corner
    ImGui::SetNextWindowPos(ImVec2(15.0f, 15.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.5f);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration |
                             ImGuiWindowFlags_AlwaysAutoResize |
                             ImGuiWindowFlags_NoSavedSettings |
                             ImGuiWindowFlags_NoFocusOnAppearing |
                             ImGuiWindowFlags_NoNav |
                             ImGuiWindowFlags_NoMove;

    if (ImGui::Begin("FPS Overlay", nullptr, flags)) {
        ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.4f, 1.0f), "FPS: %.1f", fps);
    }
    ImGui::End();
}

void UI::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::shutdown() {
    static bool shutdownDone = false;
    if (!shutdownDone) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        shutdownDone = true;
    }
}
