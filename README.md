# Aengine-3D - C++ 3D Game Engine

A modern, lightweight 3D Game Engine written in C++17 using OpenGL, GLFW, GLM, GLAD, and Dear ImGui.

![Engine Screenshot](screenshot.png)

---

## Features

- **Modern 3D Graphics Pipeline**: OpenGL 3.3 Core Profile shader-based rendering with Blinn-Phong lighting, ACES tone mapping, and depth testing.
- **Dynamic Shadow Mapping**: Two-pass directional shadow mapping with 2048x2048 depth maps and Percentage-Closer Filtering (PCF) soft shadows.
- **Material Subsystem**: Material definitions supporting ambient, diffuse, specular, and shininess properties (Emerald, Gold, Bronze, Slate Platform, etc.).
- **3D Platform Scene**: Procedurally generated 3D platform with anti-aliased dynamic grid shading and interactive illuminated 3D objects casting soft shadows.
- **Camera Subsystem**: 3D perspective camera with view/projection matrix generation and keyboard/mouse movement controls.
- **Minimal User Interface**: Simple ImGui overlay displaying an FPS counter and nothing else.
- **Zero External DLL Setup**: Dependencies (GLFW, GLM, glad, ImGui) are automatically downloaded and statically compiled by CMake on first build (`FetchContent`).
- **Clean Architecture**: Modular C++ design separating Windowing, Camera, Shaders, ShadowMap, Geometry Meshes, Renderer, and UI.
- **One-Click Launch Scripts**: Convenient `run.sh` and `run.bat` scripts to build and run automatically.
- **Offscreen Verification Support**: Command-line support for offscreen headless rendering and frame screenshot capture.

---

## Directory Structure

```text
├── CMakeLists.txt        # CMake build configuration (auto-fetches GLFW & GLM)
├── README.md             # Documentation
├── run.sh                # Linux / macOS build and run script
├── run.bat               # Windows build and run script
├── shaders/              # GLSL shader files
│   ├── default.vert      # Vertex shader (positions, normals, shadow light-space matrix)
│   ├── default.frag      # Fragment shader (Blinn-Phong lighting, ACES tone mapping, soft shadows)
│   ├── shadow.vert       # Shadow pass vertex shader
│   └── shadow.frag       # Shadow pass fragment shader
├── src/
│   ├── main.cpp          # Entry point
│   ├── core/             # Engine core systems (Window, Camera, Engine game loop)
│   ├── renderer/         # Graphics systems (Shader, Mesh, Material, ShadowMap, Renderer)
│   └── ui/               # User interface overlay (Dear ImGui)
└── vendor/               # Embedded libraries (Dear ImGui, glad)
```

---

## Quick Start (Run Scripts)

All dependencies are **automatically fetched and compiled statically by CMake**. You don't need to manually install or copy external DLLs!

### Linux / macOS

```bash
chmod +x run.sh
./run.sh
```

### Windows

```cmd
run.bat
```

---

## Manual Building & Running

If you prefer using CMake directly:

```bash
# Configure CMake (downloads GLFW and GLM automatically on first run)
cmake -B build

# Build the executable statically
cmake --build build --config Release

# Run the engine
./build/Aengine3D
```

---

## Controls & Usage

- **W / A / S / D**: Move camera horizontally (forward, left, backward, right)
- **E / Q**: Move camera vertically (up / down)
- **Esc**: Exit the engine application

### Offscreen / Headless Mode & Screenshots

To run the engine in a headless environment (or capture an automated screenshot):

```bash
LIBGL_ALWAYS_SOFTWARE=1 xvfb-run -a ./run.sh --screenshot screenshot.png
```

---

## License

This project is open-source.
