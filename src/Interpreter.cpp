#include "Interpreter.h"
#include <cstdint>
#include <iostream>
#include <array>
#include <string>

// Masks
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

constexpr std::array<uint8_t, 8> eachBitRev {
    0x1,  // 0000'0001
    0x2,  // 0000'0010
    0x4,  // 0000'0100
    0x8,  // 0000'1000 
    0x10, // 0001'0000
    0x20, // 0010'0000
    0x40, // 0100'0000
    0x80, // 1000'0000
};

Interpreter::Interpreter(const std::string& path): m_memory {path} 
{}

uint16_t Interpreter::fetch() { // Each instruction is 2 bytes so we have to read two consecutive bytes from memory
    uint8_t firstHalf {m_memory[m_programCounter]}; 
    uint8_t secondHalf {m_memory[m_programCounter + 1]};
    int instruction {(firstHalf << 8) | secondHalf}; // CHIP-8 stores instructions big-endian 

    m_programCounter += 2;

    return static_cast<uint16_t>(instruction);
}

void Interpreter::opDXYN(const uint16_t instruction) {
    const int xpos {m_variableRegister[(secondNibble & instruction) >> 8] % PIXEL_WIDTH};
    const int ypos {m_variableRegister[(thirdNibble & instruction) >> 4] % PIXEL_HEIGHT};
    m_variableRegister[0xF] = 0x0; // VF acts as a flag
    uint8_t spriteData {};
    int x {xpos};
    int y {ypos};

    for (std::size_t row {0}; static_cast<int>(row) < (fourthNibble & instruction); ++row) {
        spriteData = m_memory[m_indexRegister + row];
        y = (ypos + static_cast<int>(row)) % PIXEL_HEIGHT; 

        for (std::size_t col {0}; col < eachBit.size(); ++col) {
            x = (xpos + static_cast<int>(col)) % PIXEL_WIDTH; 

            if (spriteData & eachBit[col]) {

                if (m_display.isOn(y, x)) {
                    m_variableRegister[0xF] = 0x1;
                }

                m_display.toggle(y, x);
            }

            if (x == PIXEL_WIDTH - 1) {
                return;
            }

        }

        if (y == PIXEL_HEIGHT - 1) {
            return;
        } 
    }
}

void Interpreter::op8XY4(const uint16_t instruction) {
    int value {m_variableRegister[(secondNibble & instruction) >> 8] + 
                m_variableRegister[(thirdNibble & instruction) >> 4]};
    if (value > 255) {
        m_variableRegister[0xF] = 0x1;
    } else {
        m_variableRegister[0xF] = 0x0;
    }
    
    m_variableRegister[(secondNibble & instruction) >> 8] += 
        m_variableRegister[(thirdNibble & instruction) >> 4];
}

void Interpreter::opcodeExec(const uint16_t instruction) {
    switch (instruction & firstNibble) {
        case 0x0000: 
            switch (instruction & (thirdNibble | fourthNibble)) {
                case 0x00E0: // Clears screen
                    m_display.clearScreen();
                    m_display.render();
                    break;

                case 0x00EE: // Returns from a 2NNN subroutine
                    m_programCounter = m_stack.top();
                    m_stack.pop();
                    break;
            }
            
            break;

        case 0x1000: // 1NNN jump: Set PC (program counter) to NNN
            m_programCounter = (secondNibble | thirdNibble | fourthNibble) & instruction;
            break;

        case 0x2000: // 2NNN subroutine: Calls subroutine at NNN
            m_stack.push(m_programCounter); // Same as jump but add current PC to stack so we can return later 
            m_programCounter = (secondNibble | thirdNibble | fourthNibble) & instruction;
            break;

        case 0x3000: // 3XNN: Skips next instruction if VX == NN
            if (m_variableRegister[(secondNibble & instruction) >> 8] == 
                        ((thirdNibble | fourthNibble) & instruction))
            {
                m_programCounter += 2; // An instruction is 2 bytes
            }
            break;

        case 0x4000: // 4XNN: Skips next instruction if VX != NN
            if (m_variableRegister[(secondNibble & instruction) >> 8] != 
                        ((thirdNibble | fourthNibble) & instruction)) 
            {
                m_programCounter += 2; // An instruction is 2 bytes
            }
            break;

        case 0x5000: // 5XY0: Skips next instruction if VX == VY
            if (m_variableRegister[(secondNibble & instruction) >> 8] == 
                m_variableRegister[(thirdNibble & instruction) >> 4]) 
            {
                m_programCounter += 2;
            }
            break;

        case 0x6000: // 6XNN set: Set variable register VX to NN 
            m_variableRegister[(secondNibble & instruction) >> 8] = (thirdNibble | fourthNibble) & instruction;
            break;

        case 0x7000: // 7XNN add: Add NN to variable register VX
            m_variableRegister[(secondNibble & instruction) >> 8] += (thirdNibble | fourthNibble) & instruction;
            break;

        case 0x8000:
            switch (fourthNibble & instruction) {
                case 0x0000: // 8XY0: VX is set to VY 
                    m_variableRegister[(secondNibble & instruction) >> 8] = 
                        m_variableRegister[(thirdNibble & instruction) >> 4];
                    break;

                case 0x0001: // 8XY1: VX = VX | VY
                    m_variableRegister[(secondNibble & instruction) >> 8] = 
                        m_variableRegister[(secondNibble & instruction) >> 8] | 
                        m_variableRegister[(thirdNibble & instruction) >> 4];
                    break;

                case 0x0002: // 8XY2: VX = VX & VY
                    m_variableRegister[(secondNibble & instruction) >> 8] = 
                        m_variableRegister[(secondNibble & instruction) >> 8] & 
                        m_variableRegister[(thirdNibble & instruction) >> 4];
                    break;

                case 0x0003: // 8XY3: VX = VX ^ VY 
                    m_variableRegister[(secondNibble & instruction) >> 8] = 
                        m_variableRegister[(secondNibble & instruction) >> 8] ^ 
                        m_variableRegister[(thirdNibble & instruction) >> 4];
                    break;

                case 0x0004: // 8XY4: VX += VY and VF is set to one if VX overflows
                    op8XY4(instruction);
                    break;

                // case 0x0005: // 8XY5: VX = VX - VY
            
            }
            break;

        case 0x9000: // 9XY0: Skips next instruction if VX != VY
            if (m_variableRegister[(secondNibble & instruction) >> 8] != 
                m_variableRegister[(thirdNibble & instruction) >> 4]) 
            {
                m_programCounter += 2;
            }
            break;

        case 0xA000: // ANNN set index: set index register to value NNN
            m_indexRegister = (secondNibble | thirdNibble | fourthNibble) & instruction;
            break;

        case 0xD000: // DXYN draws to screen
            opDXYN(instruction);
            m_display.render();
            break;

        default: 
            std::cerr << std::hex << "Instruction " << instruction << " not found.\n";
            break;
    }

    return;
}
