#include "iowrapper.h"
#include "../io/io.h"
#include <stdint.h>

IoWrapper::IoWrapper(QObject *parent) :
    QObject(parent)
{
}

void IoWrapper::tick(){
    io_tick();
}

uint8_t IoWrapper::get_state(){
    return io_stat;
}

void IoWrapper::set_state(uint8_t b){
    io_stat = b;
}
