#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <cstdint>
#include <array>
#include <SDL2/SDL.h>

namespace Constants {
    constexpr int memorySize {4096};   
    constexpr uint16_t fontLocationInMemory {0x50};
    constexpr uint16_t programLocationInMemory {0x200};
    constexpr int variableRegisterLength {16};
    constexpr double timerFrequency {1.0/60.0}; // 60Hz
    constexpr int PIXEL_WIDTH {64};
    constexpr int PIXEL_HEIGHT {32};
    constexpr int SCREEN_WIDTH {640};
    constexpr int SCREEN_HEIGHT {320};
    constexpr int CELL_SIZE {SCREEN_WIDTH / PIXEL_WIDTH};
    constexpr std::array<SDL_Keycode, 16> m_keymap { // The layout in qwerty
        SDLK_x, // 0x0   Original layout:    
        SDLK_1, // 0x1          1 2 3 C
        SDLK_2, // 0x2          4 5 6 D
        SDLK_3, // 0x3          7 8 9 E
        SDLK_q, // 0x4          A 0 B F
        SDLK_w, // 0x5   QWERTY layout:
        SDLK_e, // 0x6          1 2 3 4
        SDLK_a, // 0x7          Q W E R
        SDLK_s, // 0x8          A S D F
        SDLK_d, // 0x9          Z X C V
        SDLK_z, // 0xA
        SDLK_c, // 0xB
        SDLK_4, // 0xC
        SDLK_r, // 0xD
        SDLK_f, // 0xE 
        SDLK_v, // 0xF
    };
}

#endif
