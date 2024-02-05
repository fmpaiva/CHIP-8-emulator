#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Memory.h"
#include "Timer.h"
#include <stack>
#include <array>
#include <cstdint>

constexpr int variableRegisterLenght {16};

class Interpreter {
private:
    Memory m_memory {};
    std::stack<uint16_t> m_stack {};
    Timer m_delayTimer {};
    Timer m_soundTimer {};
    uint16_t m_programCounter {}; // Points at current instruction in memory
    uint16_t m_indexRegister {};
    std::array<uint8_t, variableRegisterLenght> m_variableRegister {}; 
};

#endif
