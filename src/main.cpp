#include "Interpreter.h"

int main(int argc, char* argv[]) {
    std::string path {"Space Invaders.ch8"};
    Interpreter interpreter {path};

    interpreter.emulate();

    return 0;
}
