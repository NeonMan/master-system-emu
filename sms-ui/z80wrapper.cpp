#include "z80wrapper.h"
#include "../z80/z80.h"

Z80Wrapper::Z80Wrapper(QObject *parent) :
    QObject(parent)
{
}

void Z80Wrapper::tick(){
    z80_tick();
}
