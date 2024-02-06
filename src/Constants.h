#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <cstdint>
#include <array>

namespace Constants {
    constexpr int memorySize {4096};   
    constexpr std::size_t fontLocationInMemory {0x50};
    constexpr std::size_t programLocationInMemory {0x200};
    constexpr int variableRegisterLength {16};
    constexpr double timerFrequency {1.0/60.0}; // 60Hz
    constexpr int PIXEL_HEIGHT {32};
    constexpr int PIXEL_WIDTH {64};
    constexpr int SCREEN_WIDTH {640};
    constexpr int SCREEN_HEIGHT {320};
    constexpr int CELL_SIZE {SCREEN_WIDTH / PIXEL_WIDTH};
}

namespace Masks {
    constexpr uint16_t firstNibble  {0xF000}; // 1111'0000'0000'0000    
    constexpr uint16_t secondNibble {0x0F00}; // 0000'1111'0000'0000    
    constexpr uint16_t thirdNibble  {0x00F0}; // 0000'0000'1111'0000    
    constexpr uint16_t fourthNibble {0x000F}; // 0000'0000'0000'1111    
    constexpr std::array<uint8_t, 8> eachBit {
        0x80, // 1000'0000
        0x40, // 0100'0000
        0x20, // 0010'0000
        0x10, // 0001'0000
        0x8,  // 0000'1000 
        0x4,  // 0000'0100
        0x2,  // 0000'0010
        0x1,  // 0000'0001
    };
}

#endif
