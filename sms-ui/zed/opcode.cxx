#include "opcode.hxx"

using namespace zed;

Address zed::operator+(const Address& addr, int offset)
{
    return Address(addr.z80 + offset, addr.real + offset, addr.z80_invalid, addr.real_invalid);
}

zed::Address::Address()
{
    z80 = 0;
    real = 0;
    z80_invalid = true;
    real_invalid = true;
}

zed::Address::Address(uint16_t address_z80, uint32_t address_real, bool invalid_z80, bool invalid_real)
{
    this->real = address_real;
    this->real = invalid_real;
    this->z80 = address_z80;
    this->z80_invalid = invalid_z80;
}

// --- Opcode classes ---

zed::Opcode::Opcode(Address addr, uint8_t size, std::string op_text)
{
    this->address_opcode = addr;
    this->opcode_size = size;
    this->opcode_text = op_text;
}

zed::Opcode::~Opcode()
{
}

bool zed::Opcode::isJump() const
{
    return false;
}

Address zed::Opcode::getNextAddress() const
{
    return address_opcode + this->opcode_size;
}

Address zed::Opcode::getAddress() const
{
    return address_opcode;
}

Address zed::Opcode::getJumpAddress() const
{
    return Address();
}

uint8_t zed::Opcode::getSize() const
{
    return uint8_t();
}
