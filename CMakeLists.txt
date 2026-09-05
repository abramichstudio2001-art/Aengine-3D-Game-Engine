cmake_minimum_required(VERSION 3.16)
project(Aengine3D LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Automatically download and compile GLFW3
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)

# Automatically download and compile GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Automatically download ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.89.9
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

find_package(OpenGL REQUIRED)

set(GLAD_DIR ${CMAKE_SOURCE_DIR}/vendor/glad)

set(GLAD_SOURCES
    ${GLAD_DIR}/src/glad.c
)

# Custom wrapper to include glad.h before glfw/opengl3 in imgui_impl_glfw.cpp
set(IMGUI_GLFW_WRAPPER ${CMAKE_BINARY_DIR}/imgui_impl_glfw_glad.cpp)
file(WRITE ${IMGUI_GLFW_WRAPPER}
"#include <glad/glad.h>\n"
"#include \"backends/imgui_impl_glfw.cpp\"\n"
)

set(IMGUI_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${IMGUI_GLFW_WRAPPER}
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

set(ENGINE_SOURCES
    src/main.cpp
    src/core/Window.cpp
    src/core/Camera.cpp
    src/core/Engine.cpp
    src/renderer/Shader.cpp
    src/renderer/Mesh.cpp
    src/renderer/Renderer.cpp
    src/renderer/ShadowMap.cpp
    src/ui/UI.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/vendor
    ${GLAD_DIR}/include
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

add_executable(Aengine3D
    ${ENGINE_SOURCES}
    ${GLAD_SOURCES}
    ${IMGUI_SOURCES}
)

target_compile_definitions(Aengine3D PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_link_libraries(Aengine3D PRIVATE
    OpenGL::GL
    glfw
    glm::glm
)

# Copy shaders directory to build executable directory post-build
add_custom_command(TARGET Aengine3D POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:Aengine3D>/shaders
)

# On Windows, statically link C/C++ runtimes when using MinGW
if(WIN32 AND MINGW)
    target_link_options(Aengine3D PRIVATE -static-libgcc -static-libstdc++)
endif()
