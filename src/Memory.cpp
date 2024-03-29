#include <array>
#include <cstdint>
#include <iostream>
#include <cassert>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include "Memory.h"
#include "Constants.h"

Memory::Memory() {
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

    std::copy(font.begin(), font.end(), m_data.begin() + Constants::fontLocationInMemory);
}

bool Memory::load(const std::filesystem::path& path) {
    for (std::size_t i {Constants::programLocationInMemory}; i < m_data.size(); ++i) {
        m_data[i] = 0;
    }

    std::ifstream rom {path, std::ios::binary | std::ios::ate};
    if (!rom) {
        return false;
    }

    std::ifstream::pos_type pos {rom.tellg()};
    std::vector<char> buffer(pos);

    rom.seekg(0, std::ios::beg);
    rom.read(buffer.data(), pos);

    rom.close();

    std::copy(buffer.begin(), buffer.end(), m_data.begin() + Constants::programLocationInMemory);

    return true;
}

uint8_t& Memory::operator[](uint16_t index) {
#ifndef RELEASE
    assert(index >= 0 && index < 4096 && "Memory access out of bounds");
#endif
    return m_data[static_cast<std::size_t>(index)];
}
