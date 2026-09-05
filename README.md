# Aengine 3D Game Engine (Release: Alpha 1)

A modern, high-performance C++ 3D Game Engine built with OpenGL 3.3 Core Profile, GLFW3, GLM, GLAD, and Dear ImGui. Designed for 3D platform environments, lightweight real-time graphics, and minimal overhead.

![Engine Screenshot](screenshot.png)

---

## Key Features

- **Release Version**: Alpha 1
- **Platform Scene**: Procedural 3D platform with anti-aliased dynamic grid lines and interactive material-lit 3D primitives (Emerald, Gold, Vibrant Orange, Slate Platform).
- **Minimal FPS UI**: Clean, non-intrusive Dear ImGui overlay displaying real-time frames per second (FPS) at top-left.
- **Lighting & Shadows**:
  - Blinn-Phong specular lighting and directional sun positioning.
  - Two-pass directional depth shadow map (2048x2048 FBO) with 16-sample Percentage-Closer Filtering (PCF) soft shadows.
  - ACES filmic tone mapping and 2.2 gamma correction in GLSL shaders.
- **Camera Navigation**: Smooth 3D perspective camera with keyboard navigation (`WASD` for planar movement, `E`/`Q` for elevation).
- **Zero-Dependency Automated Build**:
  - Uses CMake `FetchContent` to download and statically link GLFW, GLM, and Dear ImGui automatically on first configure.
  - Cross-platform support for Windows (MSVC / MinGW) and Linux / macOS.
  - Auto-detection for MSYS2/MinGW PATH setup in `run.bat` to eliminate runtime DLL entry point errors.

---

## Directory Structure

```text
Aengine3D/
├── CMakeLists.txt          # Root CMake build configuration
├── README.md               # Documentation and usage guide
├── run.bat                 # One-click Windows auto-detect, build & run script
├── run.sh                  # One-click Linux/macOS auto-build & run script
├── screenshot.png          # Visual preview render of Alpha 1 release
├── shaders/                # GLSL shader source code
│   ├── default.vert        # Main scene vertex shader
│   ├── default.frag        # Blinn-Phong & PCF shadow fragment shader
│   ├── shadow.vert         # Shadow pass vertex shader
│   └── shadow.frag         # Shadow depth fragment shader
├── src/                    # C++ Engine Core Source Code
│   ├── main.cpp            # Entry point
│   ├── core/               # Window, Camera, Engine game loop
│   ├── renderer/           # Shader, Mesh, Material, ShadowMap, Renderer
│   └── ui/                 # Dear ImGui minimal FPS UI overlay
└── vendor/                 # GLAD GL loader and STB image headers
```

---

## Quick Start (Build & Run)

### Windows (One-Click)
Double-click `run.bat` or run in Command Prompt:
```cmd
run.bat
```
*`run.bat` automatically checks for CMake, detects MSYS2/MinGW environments, configures, compiles `build/Release/Aengine3D.exe`, and launches the engine.*

### Linux / macOS (One-Click)
Make `run.sh` executable and launch:
```bash
chmod +x run.sh
./run.sh
```

---

## Controls

| Key | Action |
| --- | --- |
| `W` | Move camera forward |
| `S` | Move camera backward |
| `A` | Strafe camera left |
| `D` | Strafe camera right |
| `E` | Elevate camera up |
| `Q` | Lower camera down |
| `ESC` | Exit engine |

---

## Offscreen Render / Headless Screenshot Mode

To generate an offscreen render frame and exit (useful for testing or CI pipelines):
```bash
./build/Aengine3D --screenshot output.png
```
