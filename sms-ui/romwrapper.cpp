#include "romwrapper.h"
#include "../rom/rom.h"

RomWrapper::RomWrapper(QObject *parent) :
    QObject(parent)
{

}

void RomWrapper::tick(){
    rom_tick();
}

uint8_t* RomWrapper::get_rom(){
    return (uint8_t*) romdbg_get_rom();
}

void RomWrapper::set_rom(uint8_t* buffer, unsigned int size){
    rom_set_image(buffer, size);
}

uint8_t RomWrapper::get_slot(uint8_t slot_number){
    return romdbg_get_slot(slot_number);
}

//--- Static methods ---
QJSValue RomWrapper::singleton_provider(QQmlEngine *engine, QJSEngine *scriptEngine){
    Q_UNUSED(engine);
    QJSValue example = scriptEngine->newObject();
    example.setProperty("someProperty", 916);
    return example;
}
