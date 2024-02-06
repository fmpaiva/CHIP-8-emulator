#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Memory.h"
#include "Timer.h"
#include "Constants.h"
#include "Display.h"
#include <stack>
#include <array>
#include <cstdint>
#include <string>


class Interpreter {
public:
    Interpreter(const std::string&);
    uint16_t fetch();
    void opcodeExec(const uint16_t);

    // void emulate()?? and fetch() decode() etc and 700 cycles per second all private

private:
    Memory m_memory;
    Display m_display {};
    Timer m_delayTimer {};
    Timer m_soundTimer {};
    uint16_t m_indexRegister {}; // Points at locations in memory
    uint16_t m_programCounter {Constants::programLocationInMemory}; // PC: Points at current instruction in memory
    std::stack<uint16_t> m_stack {};
    std::array<uint8_t, Constants::variableRegisterLength> m_variableRegister {}; // Variable register numberer V0 to VF 

    void opDXYN(const uint16_t); 
    void op8XY4(const uint16_t);
};

#endif
