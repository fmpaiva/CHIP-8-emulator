#ifndef VREGISTER_H
#define VREGISTER_H

#include "Constants.h"
#include <array>
#include <cstdint>

class VRegister {
public:
    enum Index {
        vx,
        vy,
        vf,
    };

    VRegister() = default;
    uint8_t getVX(const uint16_t) const;
    uint8_t getVY(const uint16_t) const;
    uint8_t& operator()(const uint16_t, Index);
    uint8_t& operator()(Index index);

private:
    std::array<uint8_t, Constants::variableRegisterLength> m_variableRegister {}; // Variable register numberer V0 to VF 
};

#endif
