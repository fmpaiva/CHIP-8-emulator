#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Memory.h"
#include <stack>
#include <cstdint>

class Interpreter {
private:
    Memory m_memory {};
    std::stack<uint16_t> m_stack {};
    uint8_t m_delayTimer {};
    uint8_t m_soundTimer {};
};

#endif
