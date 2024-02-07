#include "VRegister.h"
#include "Constants.h"
#include <cstdint>

using namespace Masks;

uint8_t& VRegister::accessVX(const uint16_t instruction) {
    return m_variableRegister[(secondNibble & instruction) >> 8];
}

uint8_t& VRegister::accessVY(const uint16_t instruction) {
    return m_variableRegister[(thirdNibble & instruction) >> 4];
}

uint8_t& VRegister::accessVF() {
    return m_variableRegister[0xF];
}

uint8_t VRegister::accessV0() {
    return m_variableRegister[0x0];
}


