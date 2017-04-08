#ifndef __ZEDEIGHTY_H
#define __ZEDEIGHTY_H

#include "opcode.hxx"
#include <cstdint>

namespace zed {

    void analyze(const uint8_t* rom, uint16_t start_addr, uint16_t end_addr);

}

#endif
