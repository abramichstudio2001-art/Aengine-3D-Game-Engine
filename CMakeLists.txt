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
    src/renderer/ShadowMap.cpp
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

# On Windows, statically link C/C++ runtimes when using MinGW and automatically copy runtime DLLs
if(WIN32)
    if(MINGW)
        target_link_options(Aengine3D PRIVATE -static-libgcc -static-libstdc++)
    endif()

    add_custom_command(TARGET Aengine3D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:Aengine3D>
            $<TARGET_FILE_DIR:Aengine3D>
        COMMAND_EXPAND_LISTS
        IGNORE_EXIT_CODE
    )
endif()
