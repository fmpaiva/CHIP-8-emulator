#include <iostream>
#include "Display.h"

constexpr double timerFrequency {1.0/60.0}; // 60Hz

int main(int argc, char* argv[]) {
    Display display {};
    while (true) {
        display.render();
    }

    return 0;
}
