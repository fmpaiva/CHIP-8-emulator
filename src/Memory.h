#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <cstdint>

class Memory {
public:
    Memory();

    // DEBUG 
    void print() const;

private:
    std::array<std::uint8_t, 4096> m_memory {};
};

#endif
