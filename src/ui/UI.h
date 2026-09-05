#ifndef UI_H
#define UI_H

#include <string>
#include <vector>

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

private:
    void renderMainMenuBar();
    void renderWorldOutliner();
    void renderDetailsPanel();
    void renderContentBrowser();
    void renderIDEScriptEditor();
    void renderConsoleLog();
    void renderViewportStats(float fps);
    void renderImportAssetDialog();

    // UI State & Variables
    int selectedEntity = 0;
    float cubePos[3] = { 0.0f, 0.95f, 0.0f };
    float cubeRot[3] = { 0.0f, 0.0f, 0.0f };
    float cubeScale[3] = { 1.0f, 1.0f, 1.0f };
    float lightIntensity = 1.2f;
    float lightColor[3] = { 1.0f, 0.97f, 0.88f };

    bool showImportDialog = false;
    char importFilePath[256] = "models/character.glb";
    int selectedAssetType = 0; // 0: GLB/GLTF, 1: FBX, 2: MP3/WAV Audio

    // IDE Code Editor State
    char scriptBuffer[2048] =
        "// Aengine 3D Scripting Engine (Unreal-style C++/Lua API)\n"
        "#include <Engine/Scripting.h>\n\n"
        "void OnBeginPlay() {\n"
        "    Log(\"Initializing 3D Entity Components...\");\n"
        "    SetTransform(Vec3(0.0f, 0.95f, 0.0f));\n"
        "}\n\n"
        "void OnUpdate(float deltaTime) {\n"
        "    RotateY(25.0f * deltaTime);\n"
        "}\n";

    std::vector<std::string> consoleLogs;
};

#endif
