#include "core/Engine.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    Engine engine;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--screenshot" && i + 1 < argc) {
            std::string path = argv[++i];
            engine.setScreenshotMode(path, 30);
        }
    }

    if (!engine.init()) {
        std::cerr << "Failed to initialize 3D Engine!" << std::endl;
        return -1;
    }

    std::cout << "3D Game Engine Running..." << std::endl;
    engine.run();

    return 0;
}
