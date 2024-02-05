#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Memory.h"
#include "Timer.h"
#include <stack>
#include <array>
#include <cstdint>

constexpr int variableRegisterLength {16};

class Interpreter {
public:
    Interpreter() = default;
    uint16_t fetch();
    void decode(uint16_t);

    // void emulate()?? and fetch() decode() etc and 700 cycles per second all private

private:
    Memory m_memory {};
    std::stack<uint16_t> m_stack {};
    Timer m_delayTimer {};
    Timer m_soundTimer {};
    uint16_t m_programCounter {}; // Points at current instruction in memory
    uint16_t m_indexRegister {}; // Points at locations in memory
    std::array<uint8_t, variableRegisterLength> m_variableRegister {}; 
};

#endif
