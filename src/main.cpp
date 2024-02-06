#include <iostream>
#include "Memory.h"
#include <cstdint>
#include <thread>
#include <chrono>
#include "Interpreter.h"

/*
 * ADD CONSTANTS FILE
*/

int main(int argc, char* argv[]) {
    static constexpr std::string path {"test_opcode.ch8"};
    Interpreter interpreter {path};
    
    while (true) {
        uint16_t instruction {interpreter.fetch()};
        interpreter.opcodeExec(instruction);
        std::this_thread::sleep_for(std::chrono::duration<double>(0.1));
    }

    return 0;
}
