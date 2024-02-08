#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Memory.h"
#include "Timer.h"
#include "Constants.h"
#include "Display.h"
#include "VRegister.h"
#include <SDL2/SDL.h>
#include <filesystem>
#include <stack>
#include <array>
#include <cstdint>
#include <string>

class Interpreter {
public:
    Interpreter();
    void emulate();
    bool load(const std::filesystem::path&);

private:
    Memory m_memory {};
    Display m_display {};
    Timer m_delayTimer {};
    Timer m_soundTimer {};
    VRegister m_vReg {}; // Variable register 
    uint16_t m_iReg {}; // Points at locations in memory
    uint16_t m_pCounter {Constants::programLocationInMemory}; // PC: Points at current instruction in memory
    std::stack<uint16_t> m_stack {};
    std::array<bool, 16> m_keypad {}; // False if key not pressed
    
    uint16_t fetch();
    void opcodeExec(const uint16_t);
    void opDXYN(const uint16_t);
    bool handleInput();
};

#endif
