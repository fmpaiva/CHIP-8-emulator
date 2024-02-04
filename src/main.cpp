#include <iostream>
#include <thread>
#include <chrono>
#include "Timer.h"

constexpr double timerFrequency {1.0/60.0}; // 60Hz

int main() {
    Timer timer {};
    std::cout << static_cast<int>(timer.getValue());
    std::this_thread::sleep_for(std::chrono::duration<double>(timerFrequency));
    std::cout << "\n";
    timer.setValue(200);
    std::cout << static_cast<int>(timer.getValue());
    std::cout << "\n";
    std::this_thread::sleep_for(std::chrono::duration<double>(0.5));
    std::cout << "\n";
    std::cout << static_cast<int>(timer.getValue());
    std::cout << "\n";
    timer.setValue(100);
    std::cout << "\n";
    std::cout << static_cast<int>(timer.getValue());
    std::cout << "\n";
    std::this_thread::sleep_for(std::chrono::duration<double>(4.0));
    std::cout << "\n";
    std::cout << static_cast<int>(timer.getValue());
    std::cout << "\n";
    timer.setValue(100);
    std::cout << "\n";
    std::cout << static_cast<int>(timer.getValue());
    std::cout << "\n";
    return 0;
}
