#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <cstdint>

class Memory {
public:
    Memory();
    uint8_t& operator[](uint16_t index);

    // DEBUG 
    void print() const;

private:
    std::array<uint8_t, 4096> m_data {};
};

#endif
