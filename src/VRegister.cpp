#include "VRegister.h"
#include "Constants.h"
#include <cstdint>

using namespace Masks;

uint8_t VRegister::getVX(const uint16_t instruction) const {
    return m_variableRegister[(secondNibble & instruction) >> 8];
}

uint8_t VRegister::getVY(const uint16_t instruction) const {
    return m_variableRegister[(thirdNibble & instruction) >> 4];
}

uint8_t& VRegister::operator()(const uint16_t instruction, Index index) {
    if (index == Index::vx) {
        return m_variableRegister[(secondNibble & instruction) >> 8];
    } else { // VY
        return m_variableRegister[(thirdNibble & instruction) >> 4];
    }
}

uint8_t& VRegister::operator()(Index index) {
    return m_variableRegister[index];
}

