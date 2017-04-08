#ifndef __ZED_OPCODE_H
#define __ZED_OPCODE_H

#include <cstdint>
#include <string>
#include <stdexcept>

namespace zed {

    class Address {
    public:
        uint16_t z80;
        uint32_t real;
        bool z80_invalid;
        bool real_invalid;
    public:
        Address();
        Address(
            uint16_t address_z80, 
            uint32_t address_real, 
            bool invalid_z80 = false, 
            bool invalid_real = false
        );
    };

    Address operator+ (const Address &addr, int offset);

    class Opcode {
    protected:
        Address address_opcode;
        uint8_t  opcode_size;
        std::string opcode_text;

    public:
        Opcode(
            Address addr,
            uint8_t size,
            std::string op_text
        );
        virtual ~Opcode();

        bool isJump() const;
        Address getNextAddress() const;
        Address getAddress() const;
        Address getJumpAddress() const;
        uint8_t getSize() const;
    };

};

#endif
