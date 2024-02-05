#ifndef KEYPAD_H
#define KEYPAD_H

#include <SDL2/SDL.h>
#include <cassert>

/* Computers that CHIP-8 used had a hexadecimal keyboard numbered from 0 to F
 *              1 2 3 C
 *              4 5 6 D
 *              7 8 9 E
 *              A 0 B F
 * We implement the following layout on a QWERTY keyboard:
 *              1 2 3 4
 *              Q W E R
 *              A S D F
 *              Z X C V
 */

namespace Keypad {
    inline int readInput(const SDL_Event& e) {
#ifndef RELEASE
        assert(e.type == SDL_KEYDOWN && "Non-keypress passed to Keypad::readInput");
#endif
        switch (e.key.keysym.sym) {
            case SDLK_1: return 0x1;
            case SDLK_2: return 0x2;
            case SDLK_3: return 0x3;
            case SDLK_4: return 0xC;
            case SDLK_q: return 0x4;
            case SDLK_w: return 0x5;
            case SDLK_e: return 0x6;
            case SDLK_r: return 0xD;
            case SDLK_a: return 0x7;
            case SDLK_s: return 0x8;
            case SDLK_d: return 0x9;
            case SDLK_f: return 0xE;
            case SDLK_z: return 0xA;
            case SDLK_x: return 0x0;
            case SDLK_c: return 0xB;
            case SDLK_v: return 0xF;
            default:     return -1;
        }
    }
}

#endif
