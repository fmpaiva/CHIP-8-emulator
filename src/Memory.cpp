#include "Memory.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

constexpr std::size_t fontLocationInMemory {0x50};
constexpr std::size_t programLocationInMemory {0x200};

Memory::Memory(const std::string& path) {
    constexpr std::array<uint8_t, 80> font {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    std::copy(font.begin(), font.end(), m_data.begin() + fontLocationInMemory);

    std::ifstream ifs(path, std::ios::binary|std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char> buffer(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(buffer.data(), pos);

    std::copy(buffer.begin(), buffer.end(), m_data.begin() + programLocationInMemory);
}

uint8_t& Memory::operator[](uint16_t index) {
#ifndef RELEASE
    assert(index >= 0 && index < 4096 && "Memory access out of bounds");
#endif
    return m_data[static_cast<std::size_t>(index)];
}

// DEBUG
void Memory::print() const {
    std::cout << std::hex;
    for (std::size_t i {0}; i < m_data.size(); ++i) {
        std::cout << "Address: " << i << "\t" << "Value: " << static_cast<int>(m_data[i]) << "\n"; 
    }
}
