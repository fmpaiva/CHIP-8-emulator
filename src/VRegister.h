#ifndef VREGISTER_H
#define VREGISTER_H

#include "Constants.h"
#include <array>
#include <cstdint>

class VRegister {
public:
    VRegister() = default;
    uint8_t& accessVX(const uint16_t);
    uint8_t& accessVY(const uint16_t);
    uint8_t& operator[](std::size_t);

private:
    std::array<uint8_t, Constants::variableRegisterLength> m_variableRegister {}; // Variable register numberer V0 to VF 
};

#endif
