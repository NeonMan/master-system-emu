#include "peripherial.h"

namespace input{

    namespace bus_default{
        uint8_t  data;
        uint16_t addr;
        uint8_t  n_wr;
        uint8_t  n_rd;
        uint8_t  n_ioreq;
    }

    namespace bus{
        uint8_t*  data    = &input::bus_default::data;
        uint16_t* addr    = &input::bus_default::addr;
        uint8_t*  n_wr    = &input::bus_default::n_wr;
        uint8_t*  n_rd    = &input::bus_default::n_rd;
        uint8_t*  n_ioreq = &input::bus_default::n_ioreq;
    }

}