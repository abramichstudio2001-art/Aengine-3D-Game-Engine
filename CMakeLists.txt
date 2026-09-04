cmake_minimum_required(VERSION 3.16)
project(Aengine3D LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/vendor/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

set(ENGINE_SOURCES
    src/main.cpp
    src/core/Window.cpp
    src/core/Camera.cpp
    src/core/Engine.cpp
    src/renderer/Shader.cpp
    src/renderer/Mesh.cpp
    src/renderer/Renderer.cpp
    src/ui/UI.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

add_executable(Aengine3D
    ${ENGINE_SOURCES}
    ${IMGUI_SOURCES}
)

target_link_libraries(Aengine3D PRIVATE
    OpenGL::GL
    GLEW::GLEW
    glfw
    glm::glm
)
