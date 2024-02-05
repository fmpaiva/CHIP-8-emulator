#include "Interpreter.h"
#include <cstdint>

uint16_t Interpreter::fetch() { // Each instruction is 2 bytes so we have to read two consecutive bytes from memory
    uint8_t firstHalf {m_memory[m_programCounter]}; 
    uint8_t secondHalf {m_memory[m_programCounter + 1]};
    int instruction {(firstHalf << 8) | secondHalf}; // CHIP-8 stores instructions big-endian 

    m_programCounter += 2;

    return static_cast<uint16_t>(instruction);
}

void Interpreter::decode(uint16_t instruction) {
      
}
