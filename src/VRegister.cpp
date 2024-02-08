#include "VRegister.h"
#include <cstdint>

using namespace Masks;

uint8_t& VRegister::accessVX(const uint16_t instruction) {
    return m_variableRegister[(0x0F00 & instruction) >> 8];
}

uint8_t& VRegister::accessVY(const uint16_t instruction) {
    return m_variableRegister[(0x00F0 & instruction) >> 4];
}

uint8_t& VRegister::operator[](std::size_t i) {
    return m_variableRegister[i];
}
