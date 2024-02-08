#include "Interpreter.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <ROM>\n";
        return 1;
    }

    std::filesystem::path romPath {argv[1]};
    Interpreter interpreter {};

    if (std::filesystem::exists(romPath)) {
        std::cout << "Loading ROM " << romPath << "\n";
        if (interpreter.load(romPath)) {
            std::cout << "ROM " << romPath << " loaded successfuly\n";
        } else {
            std::cerr << "Error reading ROM " << romPath << "\n";
            return 1;
        }
    } else {
        std::cerr << "File path: " << argv[1] << " is unknown.\n";
        return 1;
    }

    interpreter.emulate();

    return 0;
}
