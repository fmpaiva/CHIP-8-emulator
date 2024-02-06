#include <iostream>
#include "Memory.h"
#include <thread>
#include <chrono>
#include "Interpreter.h"

int main(int argc, char* argv[]) {
    static constexpr std::string path {"test_opcode.ch8"};
    Interpreter interpreter {path};

    std::cout << sizeof(interpreter);

    return 0;
}
