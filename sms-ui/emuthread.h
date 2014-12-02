#ifndef EMUTHREAD_H
#define EMUTHREAD_H

#include <QThread>
#include <stdint.h>

enum run_mode {RUN_STOPPED, RUN_PAUSED, RUN_CYCLE, RUN_STEP, RUN_RUNNING};

/**
 * @brief This class is the emulator worker thread.
 * The worker thread allows the emulator to run without the GUI
 * freezing up.
 */
class EmuThread : public QThread
{
public:
    static EmuThread* instance();
    static void drop();
    void run();
    void tick();

    //Emulation methods
    void sdscData(unsigned int b);
    void sdscControl(unsigned int b);
    void setRunMode(run_mode r){running = r;}
    run_mode getRunMode(){return running;}

private:
    volatile run_mode running;

    EmuThread();
    EmuThread(const EmuThread &); //<-- Prevent use of copy constructor
    EmuThread& operator=(const EmuThread &); //<-- Prevent assignment
    static EmuThread* m_instance;
    //SDSC callbacks
    static void sdsc_data_callback(uint8_t b);
    static void sdsc_control_callback(uint8_t b);

};

#endif // EMUTHREAD_H
