#include "zedeighty.h"
#include "opcode.h"

#include <z80dasm/z80_dasm.h>

#include <cstdint>
#include <list>


using namespace zed;

void zed::analyze(const uint8_t * rom, uint16_t start_addr, uint16_t end_addr)
{
    std::list<uint16_t> frontier;

    std::list<z80d_opcode> jumps;
    std::list<z80d_opcode> unsafe_jumps;
    std::list<z80d_opcode> normal_ops;

    bool hit_address[256 * 256];

    for (int i = 0; i < (256 * 256); i++) {
        hit_address[i] = false;
    }

    //Push the initial address into the frontier
    frontier.push_back(start_addr);
    //While addresses on the frontier list
    while (frontier.size() > 0) {
        //Get the current address
        uint16_t addr;
        addr = frontier.front();
        frontier.pop_front();

        //If address within range...
        if (addr > end_addr) {
            printf("Out of range: %04Xh\r\n", addr);
            continue;
        }

        //If address not hit...
        if (hit_address[addr]) {
            continue;
        }
        else {
            hit_address[addr] = true;
        }

        //...Diasassemble opcode at address
        z80d_opcode op = z80d_decode_op(rom + addr, addr);

        //If opcode is "normal" push next addres into frontier
        if ((op.flags & Z80D_TYPE_NORMAL)) {
            frontier.push_back(op.address_next);
            normal_ops.push_back(op);
        }

        //If opcode is "Constant jump" push into frontier and save
        if ((op.flags & Z80D_TYPE_CONST_JUMP)) {
            frontier.push_back(op.address_jump);
            jumps.push_back(op);
        }

        //If opcode is "Relative jump" push into frontier and save
        if ((op.flags & Z80D_TYPE_REL_JUMP)) {
            frontier.push_back(op.address_jump);
            jumps.push_back(op);
        }

        //If opcode is "Indirect", save
        if ((op.flags & Z80D_TYPE_INDIRECT_JUMP)) {
            unsafe_jumps.push_back(op);
        }
    }
}
