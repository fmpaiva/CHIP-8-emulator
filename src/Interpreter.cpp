#include "Interpreter.h"
#include "Constants.h"
#include "VRegister.h"
#include "Keypad.h"
#include "Random.h"
#include <SDL2/SDL.h>
#include <cstdint>
#include <iostream>
#include <array>
#include <string>

uint16_t decodeNNN(const uint16_t);
uint8_t decodeNN(const uint16_t); 

using namespace Masks;

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
    const int xpos {m_vReg.accessVX(instruction) % PIXEL_WIDTH};
    const int ypos {m_vReg.accessVY(instruction) % PIXEL_HEIGHT};
    m_vReg.accessVF() = 0x0; // VF acts as a flag
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
                    m_vReg.accessVF() = 0x1;
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

void Interpreter::opFX0A(const uint16_t instruction) {
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e)) {
            uint8_t inputValue {Keypad::readInput(e)};
            if (inputValue) {
                m_vReg.accessVX(instruction) = inputValue;
                break;
            }
        }
    }
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
            m_programCounter = decodeNNN(instruction);
            break;

        case 0x2000: // 2NNN subroutine: Calls subroutine at NNN
            m_stack.push(m_programCounter); // Same as jump but add current PC to stack so we can return later 
            m_programCounter = decodeNNN(instruction);
            break;

        case 0x3000: // 3XNN: Skips next instruction if VX == NN
            if (m_vReg.accessVX(instruction) == decodeNN(instruction)) {
                m_programCounter += 2; // An instruction is 2 bytes
            }
            break;

        case 0x4000: // 4XNN: Skips next instruction if VX != NN
            if (m_vReg.accessVX(instruction) != decodeNN(instruction)) {
                m_programCounter += 2; // An instruction is 2 bytes
            }
            break;

        case 0x5000: // 5XY0: Skips next instruction if VX == VY
            if (m_vReg.accessVX(instruction) == m_vReg.accessVY(instruction)) {
                m_programCounter += 2;
            }
            break;

        case 0x6000: // 6XNN set: Set variable register VX to NN 
            m_vReg.accessVX(instruction) = decodeNN(instruction);
            break;

        case 0x7000: // 7XNN add: Add NN to variable register VX
            m_vReg.accessVX(instruction) += decodeNN(instruction);
            break;

        case 0x8000:
            switch (fourthNibble & instruction) {
                case 0x0000: // 8XY0: VX is set to VY 
                    m_vReg.accessVX(instruction) = m_vReg.accessVY(instruction);
                    break;

                case 0x0001: // 8XY1: VX = VX | VY
                    m_vReg.accessVX(instruction) |= m_vReg.accessVY(instruction);
                    break;

                case 0x0002: // 8XY2: VX = VX & VY
                    m_vReg.accessVX(instruction) &= m_vReg.accessVY(instruction);
                    break;

                case 0x0003: // 8XY3: VX = VX ^ VY 
                    m_vReg.accessVX(instruction) ^= m_vReg.accessVY(instruction);
                    break;

                case 0x0004: // 8XY4: VX += VY and VF is set to one if VX overflows
                    if (m_vReg.accessVX(instruction) + m_vReg.accessVY(instruction) > 255) {
                        m_vReg.accessVF() = 0x1;
                    } else {
                        m_vReg.accessVF() = 0x0;
                    }
                    m_vReg.accessVX(instruction) += m_vReg.accessVY(instruction);
                    break;

                case 0x0005: // 8XY5: VX = VX - VY
                    if (m_vReg.accessVX(instruction) >= m_vReg.accessVY(instruction)) {
                        m_vReg.accessVF() = 0x1; 
                    } else {
                        m_vReg.accessVF() = 0x0;
                    }
                    m_vReg.accessVX(instruction) -= m_vReg.accessVY(instruction);
                    break;

                case 0x0006: // 8XY6: Shits VX one bit to the right
                    if (0x100 & instruction) { // Sets VF to the bit shifted: 0000'0001'0000'0000 & instruction
                        m_vReg.accessVF() = 0x1;
                    } else {
                        m_vReg.accessVF() = 0x0;
                    }
                    m_vReg.accessVX(instruction) >>= 1;
                    break;

                case 0x0007: // 8XY7: VX = VY - VX
                    if (m_vReg.accessVY(instruction) >= m_vReg.accessVX(instruction)) {
                        m_vReg.accessVF() = 0x1; 
                    } else {
                        m_vReg.accessVF() = 0x0;
                    }
                    m_vReg.accessVX(instruction) = m_vReg.accessVY(instruction) - m_vReg.accessVX(instruction);
                    break;
                
                case 0x000E: // 8XYE: Shifts VX one bit to the left 
                    if (0x800 & instruction) { // 0000'1000'0000'0000
                        m_vReg.accessVF() = 0x1;
                    } else {
                        m_vReg.accessVF() = 0x0;
                    }
                    m_vReg.accessVX(instruction) <<= 1;
                    break;
            }
            break;

        case 0x9000: // 9XY0: Skips next instruction if VX != VY
            if (m_vReg.accessVX(instruction) != m_vReg.accessVY(instruction)) {
                m_programCounter += 2;
            }
            break;

        case 0xA000: // ANNN set index: set index register to value NNN
            m_indexRegister = decodeNNN(instruction);
            break;

        case 0xB000: // BNNN jump with offset: PC = V0 + NNN Here we implement the original COSMAC VIP behaviour
            m_programCounter = m_vReg.accessV0() + decodeNNN(instruction);  
            break;

        case 0xC000: // CXNN random: VX = random_number & NN
            m_vReg.accessVX(instruction) = Random::get<uint8_t>(0, 0xFF) & decodeNN(instruction);
            break;

        case 0xD000: // DXYN draws to screen
            opDXYN(instruction);
            m_display.render();
            break;

        // case 0xE000:
        //     switch ((thirdNibble | fourthNibble) & instruction) {
        //         case 0x009E: // EX9E: skips one instruction if the key corresponding to value VX is pressed
        //             if (m_keypad.getActiveKey() == )
        //             
        //             
        //     }
        //     break;

        case 0xF000:
            switch ((thirdNibble | fourthNibble) & instruction) {
                case 0x0007: // FX07: Sets the VX register to the current value of the delay timer
                    m_vReg.accessVX(instruction) = m_delayTimer.getValue();
                    break;

                case 0x0015: // FX15: Sets the delay timer to the value of VX
                    m_delayTimer.setValue(m_vReg.accessVX(instruction));
                    break;

                case 0x0018: // FX18: Sets the sound timer to the value of VX
                    m_soundTimer.setValue(m_vReg.accessVX(instruction));
                    break;

                case 0x001E: // FX1E: index register += VX
                    m_indexRegister += m_vReg.accessVX(instruction);
                    break;

                case 0x000A: // FX0A get key: waits for key input and assigns the input to VX
                    opFX0A(instruction);
                    break;

                case 0x0029: // FX29 font character: index register is set to VX 
                    m_indexRegister = (fontLocationInMemory + m_vReg.accessVX(instruction)) & fourthNibble; 
                    break;

                // case 0x0033: // FX33: binary-coded decimal conversion
                //     
                
            }
            break;

        default: 
            std::cerr << std::hex << "Instruction " << instruction << " not found.\n";
            break;
    }

    return;
}


uint16_t decodeNNN(const uint16_t instruction) {
    return (secondNibble | thirdNibble | fourthNibble) & instruction;
}

uint8_t decodeNN(const uint16_t instruction) {
    return (thirdNibble | fourthNibble) & instruction;
} 
