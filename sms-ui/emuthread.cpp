#include "emuthread.h"
#include <QMutex>
#include <QtGlobal>
#include <QElapsedTimer>


//Emulator includes
#include "../ram/ram.h"
#include "../rom/rom.h"
#include "../psg/psg.h"
#include "../peripherial/peripherial.h"
#include "../z80/z80.h"
#include "../io/io.h"
#include "../z80/z80_macros.h"

// --- Static stuff ---
EmuThread* EmuThread::m_instance = 0;

EmuThread* EmuThread::instance(){
    static QMutex mutex;
    if(!m_instance){
        mutex.lock();
        if(!m_instance)
            m_instance = new EmuThread;
        mutex.unlock();
    }
    return m_instance;
}

void EmuThread::drop(){
    static QMutex mutex;
    mutex.lock();
    delete m_instance;
    m_instance = 0;
    mutex.unlock();
}

// SDSC callbacks
void EmuThread::sdsc_data_callback(uint8_t b){
    EmuThread::instance()->sdscData(b);
}

///Process SDSC control commands
void EmuThread::sdsc_control_callback(uint8_t b){
    EmuThread::instance()->sdscControl(b);
}

//Constructor
EmuThread::EmuThread()
{
    qDebug("Configuring emulator");
    running = RUN_RUNNING;
    z80_init(EmuThread::sdsc_data_callback, EmuThread::sdsc_control_callback);
}

//Thread entry point
void EmuThread::run(){
    qDebug("Emulator thread started");
    while(running == RUN_RUNNING)
        tick();
    qDebug("Emulator thread finished");
}

//Execute one z80 cycle
void EmuThread::tick(){
    unsigned int cycle_count = 0;
    struct z80_s* const z80p = z80dbg_get_z80();
    QElapsedTimer timer;
    timer.start();
    do{

        z80_tick();
        z80_tick();
        io_tick();
        //ram_tick();
        rom_tick();
        //per_tick();
        cycle_count += 1;
    }while (z80p->stage != Z80_STAGE_RESET);
    const qint64 wait_time = (((1000 * cycle_count)/Z80_CLOCK_PAL)) - timer.elapsed();
    if (wait_time > 0) QThread::msleep(wait_time);
}

//SDSC data
void EmuThread::sdscData(unsigned int b){
    Q_UNUSED(b)
}

//SDSC Control
void EmuThread::sdscControl(unsigned int b){
    Q_UNUSED(b)
}
