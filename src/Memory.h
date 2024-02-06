#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <cstdint>
#include <string>
#include "Constants.h"

class Memory {
public:
    Memory(const std::string&);
    uint8_t& operator[](uint16_t);

    // DEBUG 
    void print() const;

private:
    std::array<uint8_t, Constants::memorySize> m_data {};
};

#endif
