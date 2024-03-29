#include "Interpreter.h"
#include "Constants.h"
#include "VRegister.h"
#include "Memory.h"
#include "Random.h"
#include <SDL2/SDL.h>
#include <cstdint>
#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <iostream>

uint16_t decodeNNN(const uint16_t);
uint8_t decodeNN(const uint16_t);


Interpreter::Interpreter() {
    for (std::size_t i {0}; i < m_keypad.size(); ++i) {
        m_keypad[i] = false;
    }
}

void Interpreter::emulate() {
    constexpr int frequency {700};    
    constexpr auto loopDuration {std::chrono::milliseconds(static_cast<long long>(1000.0 / frequency))};
    auto startTime {std::chrono::steady_clock::now()};
    
    while (handleInput()) {

        opcodeExec(fetch());

        // Loop Timing
        auto endTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        auto sleepDuration = loopDuration - elapsedTime;
        if (sleepDuration.count() >= 0) {
            std::this_thread::sleep_for(sleepDuration);
        }

        startTime = std::chrono::steady_clock::now();
    }
}

bool Interpreter::load(const std::filesystem::path& path) {
    return m_memory.load(path);
}

bool Interpreter::handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }

        if (e.type == SDL_KEYDOWN) {
            for (std::size_t i {0}; i < m_keypad.size(); ++i) {
                if (e.key.keysym.sym == m_keymap[i]) {
                    m_keypad[i] = true;
                }
            }
        }

        if (e.type == SDL_KEYUP) {
            for (std::size_t i {0}; i < m_keypad.size(); ++i) {
                if (e.key.keysym.sym == m_keymap[i]) {
                    m_keypad[i] = false;
                }
            }
        }
    }
    
    return true;
}

uint16_t Interpreter::fetch() { // Each instruction is 2 bytes so we have to read two consecutive bytes from memory
    uint8_t firstHalf {m_memory[m_pCounter]}; 
    uint8_t secondHalf {m_memory[m_pCounter + 1]};
    int instruction {(firstHalf << 8) | secondHalf}; // CHIP-8 stores instructions big-endian 

    m_pCounter += 2;

    return static_cast<uint16_t>(instruction);
}

void Interpreter::opDXYN(const uint16_t instruction) {
    const int xpos {m_vReg.accessVX(instruction) % PIXEL_WIDTH};
    const int ypos {m_vReg.accessVY(instruction) % PIXEL_HEIGHT};
    m_vReg[0xF] = 0x0; // VF acts as a flag
    uint8_t spriteData {};
    int x {xpos};
    int y {ypos};

    for (std::size_t row {0}; static_cast<int>(row) < (0x000F & instruction); ++row) {
        spriteData = m_memory[m_iReg + static_cast<uint16_t>(row)];
        y = (ypos + static_cast<int>(row)) % PIXEL_HEIGHT; 

        for (std::size_t col {0}; col < 8; ++col) {
            x = (xpos + static_cast<int>(col)) % PIXEL_WIDTH; 

            if (spriteData & (0x80 >> col)) { // 1000'0000

                if (m_display.isOn(y, x)) {
                    m_vReg[0xF] = 0x1;
                }

                m_display.toggle(y, x);
            }

            if (x == PIXEL_WIDTH) {
                return;
            }
        }

        if (y == PIXEL_HEIGHT) {
            return;
        } 
    }
}

void Interpreter::opcodeExec(const uint16_t instruction) {
    switch (instruction & 0xF000) {
        case 0x0000: 
            switch (instruction & (0x00F0 | 0x000F)) {
                case 0x00E0: // Clears screen
                    m_display.clearScreen();
                    m_display.render();
                    break;

                case 0x00EE: // Returns from a 2NNN subroutine
                    m_pCounter = m_stack.top();
                    m_stack.pop();
                    break;
            }
            break;

        case 0x1000: // 1NNN jump: Set PC (program counter) to NNN
            m_pCounter = decodeNNN(instruction);
            break;

        case 0x2000: // 2NNN subroutine: Calls subroutine at NNN
            m_stack.push(m_pCounter); // Same as jump but add current PC to stack so we can return later 
            m_pCounter = decodeNNN(instruction);
            break;

        case 0x3000: // 3XNN: Skips next instruction if VX == NN
            if (m_vReg.accessVX(instruction) == decodeNN(instruction)) {
                m_pCounter += 2; // An instruction is 2 bytes
            }
            break;

        case 0x4000: // 4XNN: Skips next instruction if VX != NN
            if (m_vReg.accessVX(instruction) != decodeNN(instruction)) {
                m_pCounter += 2; // An instruction is 2 bytes
            }
            break;

        case 0x5000: // 5XY0: Skips next instruction if VX == VY
            if (m_vReg.accessVX(instruction) == m_vReg.accessVY(instruction)) {
                m_pCounter += 2;
            }
            break;

        case 0x6000: // 6XNN set: Set variable register VX to NN 
            m_vReg.accessVX(instruction) = decodeNN(instruction);
            break;

        case 0x7000: // 7XNN add: Add NN to variable register VX
            m_vReg.accessVX(instruction) += decodeNN(instruction);
            break;

        case 0x8000:
            switch (0x000F & instruction) {
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

                case 0x0004: { // 8XY4: VX += VY and VF is set to one if VX overflows
                    int testValue {m_vReg.accessVX(instruction) + m_vReg.accessVY(instruction)};
                    m_vReg.accessVX(instruction) += m_vReg.accessVY(instruction);
                    if (testValue > 255) {
                        m_vReg[0xF] = 0x1;
                    } else {
                        m_vReg[0xF] = 0x0;
                    }
                    break;
                }

                case 0x0005: {// 8XY5: VX = VX - VY
                    bool testValue {m_vReg.accessVX(instruction) >= m_vReg.accessVY(instruction)};
                    m_vReg.accessVX(instruction) -= m_vReg.accessVY(instruction);
                    if (testValue) {
                        m_vReg[0xF] = 0x1; 
                    } else {
                        m_vReg[0xF] = 0x0;
                    }
                    break;
                }

                case 0x0006: { // 8XY6: Shits VX one bit to the right
                    auto shiftedBit {static_cast<uint8_t>(0x1 & m_vReg.accessVX(instruction))};
                    m_vReg.accessVX(instruction) >>= 1;
                    m_vReg[0xF] = shiftedBit;
                    break;
                }

                case 0x0007: { // 8XY7: VX = VY - VX
                    bool testValue {m_vReg.accessVY(instruction) >= m_vReg.accessVX(instruction)};
                    m_vReg.accessVX(instruction) = m_vReg.accessVY(instruction) - m_vReg.accessVX(instruction);
                    if (testValue) {
                        m_vReg[0xF] = 0x1; 
                    } else {
                        m_vReg[0xF] = 0x0;
                    }
                    break;
                }
                
                case 0x000E: // 8XYE: Shifts VX one bit to the left 
                    auto shiftedBit {static_cast<uint8_t>(0x8 & m_vReg.accessVX(instruction))};
                    m_vReg.accessVX(instruction) <<= 1;
                    if (shiftedBit) {
                        m_vReg[0xF] = 0x1;
                    } else {
                        m_vReg[0xF] = 0x0;
                    }
                    break;
            }
            break;

        case 0x9000: // 9XY0: Skips next instruction if VX != VY
            if (m_vReg.accessVX(instruction) != m_vReg.accessVY(instruction)) {
                m_pCounter += 2;
            }
            break;

        case 0xA000: // ANNN set index: set index register to value NNN
            m_iReg = decodeNNN(instruction);
            break;

        case 0xB000: // BNNN jump with offset: PC = V0 + NNN Here we implement the original COSMAC VIP behaviour
            m_pCounter = m_vReg[0x0] + decodeNNN(instruction);  
            break;

        case 0xC000: // CXNN random: VX = random_number & NN
            m_vReg.accessVX(instruction) = Random::get<uint8_t>(0, 0xFF) & decodeNN(instruction);
            break;

        case 0xD000: // DXYN draws to screen
            opDXYN(instruction);
            m_display.render();
            break;

        case 0xE000:
            switch ((0x00F0 | 0x000F) & instruction) {
                case 0x009E: // EX9E: skips one instruction if the key corresponding to value VX is pressed
                    if (m_keypad[m_vReg.accessVX(instruction)]) {
                        m_pCounter += 2;
                    }
                    break;

                case 0x0A1: // EXA1: skips one instruction if the key is not pressed
                    if (!m_keypad[m_vReg.accessVX(instruction)]) {
                        m_pCounter += 2;
                    }
                    break;
            }
            break;

        case 0xF000:
            switch ((0x00F0 | 0x000F) & instruction) {
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
                    m_iReg += m_vReg.accessVX(instruction);
                    break;

                case 0x000A: { // FX0A get key: waits for key input and assigns the input to VX
                    for (uint8_t i {0}; i < m_keypad.size(); ++i) {
                        if (m_keypad[i]) {
                            m_vReg.accessVX(instruction) = i;
                            return;
                        }
                    }
                    m_pCounter -= 2; // Decrement so it loops back and see again if a key is pressed 
                    break;
                }

                case 0x0029: // FX29 font character: index register is set to VX 
                    m_iReg = (fontLocationInMemory + m_vReg.accessVX(instruction)) & 0x000F; 
                    break;

                case 0x0033: { // FX33: binary-coded decimal conversion
                    uint8_t number {m_vReg.accessVX(instruction)};
                    m_memory[m_iReg]     =  number / 100;
                    m_memory[m_iReg + 1] = (number / 10) % 10;
                    m_memory[m_iReg + 2] =  number % 10;
                    break;
                }

                case 0x0055: // FX55: Store memory
                    for (uint16_t i {0}; i <= ((0x0F00 & instruction) >> 8); ++i) {
                        m_memory[m_iReg + i] = m_vReg[i];
                    }
                    break;

                case 0x0065: // FX65: Load memory
                    for (uint16_t i {0}; i <= ((0x0F00 & instruction) >> 8); ++i) {
                        m_vReg[i] = m_memory[m_iReg + i];
                    }
                    break;
            }
            break;

        default: 
            std::cerr << std::hex << "Instruction " << instruction << " not found.\n";
            break;
    }

    return;
}

uint16_t decodeNNN(const uint16_t instruction) {
    return (0x0F00 | 0x00F0 | 0x000F) & instruction;
}

uint8_t decodeNN(const uint16_t instruction) {
    return static_cast<uint8_t>((0x00F0 | 0x000F) & instruction);
} 

