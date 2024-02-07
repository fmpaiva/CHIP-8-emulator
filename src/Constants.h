#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <cstdint>
#include <array>
#include <SDL2/SDL.h>

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
    constexpr std::array<SDL_Keycode, 16> m_keymap { // The layout in qwerty
        SDLK_x, // 0x0
        SDLK_1, // 0x1
        SDLK_2, // 0x2
        SDLK_3, // 0x3
        SDLK_q, // 0x4 
        SDLK_w, // 0x5
        SDLK_e, // 0x6
        SDLK_a, // 0x7
        SDLK_s, // 0x8
        SDLK_d, // 0x9
        SDLK_z, // 0xA
        SDLK_c, // 0xB
        SDLK_4, // 0xC
        SDLK_r, // 0xD
        SDLK_f, // 0xE 
        SDLK_v, // 0xF
    };
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
