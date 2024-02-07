#include <iostream>
#include "Memory.h"
#include <cstdint>
#include <memory>
#include <thread>
#include <chrono>
#include "Interpreter.h"

int main(int argc, char* argv[]) {
    std::string path {"test_opcode.ch8"};
    auto interpreter {std::make_unique<Interpreter>(path)}; 

    constexpr int frequency {700};    
    constexpr auto loopDuration = std::chrono::milliseconds(static_cast<long long>(1000.0 / frequency));
    long long loopCount {0};    
    auto startTime = std::chrono::steady_clock::now();
    
    while (true) {
        uint16_t instruction {interpreter->fetch()};
        interpreter->opcodeExec(instruction);
        ++loopCount;        
        auto endTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        auto sleepDuration = loopDuration - elapsedTime;

        if (sleepDuration.count() < 0) {
            std::cerr << "Warning: Loop iteration took longer than the desired frequency!" << std::endl;
        } else {
            // Sleep until the next iteration
            std::this_thread::sleep_for(sleepDuration);
        }

        // Update start time for the next iteration
        startTime = std::chrono::steady_clock::now();
    }

    return 0;
}
