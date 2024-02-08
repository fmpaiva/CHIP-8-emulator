#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>
#include "Constants.h"

class Memory {
public:
    Memory();
    bool load(const std::filesystem::path&);
    uint8_t& operator[](uint16_t);

private:
    std::array<uint8_t, Constants::memorySize> m_data {};
};

#endif
