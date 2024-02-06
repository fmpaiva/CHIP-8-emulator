#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Memory.h"
#include "Timer.h"
#include "Constants.h"
#include "Display.h"
#include "VRegister.h"
#include <stack>
#include <array>
#include <cstdint>
#include <string>

class Interpreter {
public:
    Interpreter(const std::string&);
    uint16_t fetch();
    void opcodeExec(const uint16_t);

private:
    Memory m_memory;
    Display m_display {};
    Timer m_delayTimer {};
    Timer m_soundTimer {};
    VRegister m_vReg {};
    uint16_t m_indexRegister {}; // Points at locations in memory
    uint16_t m_programCounter {Constants::programLocationInMemory}; // PC: Points at current instruction in memory
    std::stack<uint16_t> m_stack {};
    
    void opDXYN(const uint16_t); 
    void op8XY4(const uint16_t);
    uint16_t getNNN(const uint16_t);
    uint8_t getNN(const uint16_t); 
};

#endif
