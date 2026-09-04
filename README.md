# Aengine-3D - C++ 3D Game Engine

A modern, lightweight 3D Game Engine written in C++17 using OpenGL, GLFW, GLEW, GLM, and Dear ImGui.

---

## Features

- **Modern 3D Graphics Pipeline**: Modern OpenGL (3.3 Core Profile) shader-based rendering with Phong lighting and depth testing.
- **3D Platform Scene**: Procedurally generated 3D platform with dynamic grid shading and an interactive 3D object.
- **Camera Subsystem**: 3D perspective camera with view/projection matrix generation and movement controls.
- **Minimal User Interface**: Simple ImGui overlay displaying an FPS counter and nothing else.
- **Clean Architecture**: Modular C++ design separating Windowing, Camera, Shaders, Geometry Meshes, Renderer, and UI.
- **Offscreen Verification Support**: Command-line support for offscreen headless rendering and frame screenshot capture.

---

## Directory Structure

```text
├── CMakeLists.txt        # CMake build configuration
├── README.md             # Documentation
├── shaders/              # GLSL shader files
│   ├── default.vert      # Vertex shader (positions, normals, perspective projection)
│   └── default.frag      # Fragment shader (Phong lighting & platform grid shader)
├── src/
│   ├── main.cpp          # Entry point
│   ├── core/             # Engine core systems (Window, Camera, Engine game loop)
│   ├── renderer/         # Graphics systems (Shader, Mesh, Renderer)
│   └── ui/               # User interface overlay (Dear ImGui)
└── vendor/               # Third-party dependencies (Dear ImGui)
```

---

## Prerequisites

On Ubuntu / Debian systems, install the required packages:

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libglfw3-dev libglew-dev libglm-dev libstb-dev
```

---

## Building the Engine

To build the 3D game engine, configure and compile with CMake:

```bash
# Configure CMake
cmake -B build

# Build the executable
cmake --build build
```

The compiled executable binary `Aengine3D` will be placed in the `build/` directory.

---

## Running the Engine

Run the compiled executable:

```bash
./build/Aengine3D
```

### Controls

- **W / A / S / D**: Move camera horizontally (forward, left, backward, right)
- **E / Q**: Move camera vertically (up / down)
- **Esc**: Exit the engine application

### Offscreen / Headless Mode & Screenshots

To run the engine in a headless environment (or capture an automated screenshot):

```bash
LIBGL_ALWAYS_SOFTWARE=1 xvfb-run -a ./build/Aengine3D --screenshot screenshot.png
```

---

## License

This project is open-source.
