# Aengine 3D Game Engine (Release: Full Alpha 1 Editor)

A modern, high-performance C++ 3D Game Engine & Studio Environment built with OpenGL 3.3 Core Profile, GLFW3, GLM, GLAD, and Dear ImGui. Designed for 3D platform environments, real-time graphics, built-in C++/Lua scripting IDE, and Unreal Engine-style dark workspace interface.

![Engine Screenshot](screenshot.png)

---

## Key Features

- **Release Version**: Full Alpha 1 Studio Release
- **Unreal Engine-Style Workspace Interface**:
  - **World Outliner**: Scene hierarchy manager with entity selection.
  - **Details / Inspector**: Interactive transform controls (Location, Rotation, Scale), material properties, and directional light inspector.
  - **Content Browser**: Project directory navigator with Asset Importer modal for `.glb`, `.fbx`, `.mp3`, `.wav`, and `.png` files.
  - **Built-in IDE & Scripting Editor**: Code editor with syntax styling, script compilation, and execution assembly.
  - **Engine Console Log**: Real-time event log tracking engine status and user operations.
  - **Viewport Stats**: Real-time FPS, frame timing, shadow map stats, and shading mode display.
- **3D Graphics & Rendering Pipeline**:
  - Procedural 3D platform with anti-aliased grid shader and material-lit 3D primitives (Emerald, Gold, Vibrant Orange, Slate Platform).
  - Blinn-Phong specular lighting and directional light positioning.
  - Two-pass directional depth shadow map (2048x2048 FBO) with 16-sample Percentage-Closer Filtering (PCF) soft shadows.
  - ACES filmic tone mapping and 2.2 gamma correction in GLSL shaders.
- **Camera Navigation**: Smooth 3D perspective camera with keyboard navigation (`WASD` for planar movement, `E`/`Q` for elevation).
- **Cross-Platform Executable Builds**:
  - Includes compiled binaries for Linux (`build/Aengine3D`) and Windows 64-bit (`build/Aengine3D.exe`).
  - `run.bat` and `run.sh` provide one-click automatic build & launch with PATH prioritization.

---

## Directory Structure

```text
Aengine3D/
├── CMakeLists.txt          # Root CMake build configuration
├── README.md               # Documentation and usage guide
├── run.bat                 # One-click Windows auto-detect, build & run script
├── run.sh                  # One-click Linux/macOS auto-build & run script
├── screenshot.png          # Visual preview render of Full Release
├── build/                  # Target build output directory
│   ├── Aengine3D           # Linux executable binary
│   └── Aengine3D.exe       # Windows 64-bit executable binary
├── shaders/                # GLSL shader source code
│   ├── default.vert        # Main scene vertex shader
│   ├── default.frag        # Blinn-Phong & PCF shadow fragment shader
│   ├── shadow.vert         # Shadow pass vertex shader
│   └── shadow.frag         # Shadow depth fragment shader
├── src/                    # C++ Engine Core Source Code
│   ├── main.cpp            # Entry point
│   ├── core/               # Window, Camera, Engine game loop
│   ├── renderer/           # Shader, Mesh, Material, ShadowMap, Renderer
│   └── ui/                 # Unreal Engine-style dark theme IDE UI workspace
└── vendor/                 # GLAD GL loader and STB image headers
```

---

## Quick Start (Build & Run)

### Windows (One-Click)
Double-click `run.bat` or run in Command Prompt:
```cmd
run.bat
```
*`run.bat` automatically checks for CMake, detects MSYS2/MinGW environments, configures, compiles `build/Aengine3D.exe`, and launches the engine.*

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
