#include "peripherial.h"

namespace input{

    namespace bus_default{
        uint_fast8_t  data;
        uint_fast16_t addr;
        uint_fast8_t  n_wr;
        uint_fast8_t  n_rd;
        uint_fast8_t  n_ioreq;
    }

    namespace bus{
        uint_fast8_t*  data    = &input::bus_default::data;
        uint_fast16_t* addr    = &input::bus_default::addr;
        uint_fast8_t*  n_wr    = &input::bus_default::n_wr;
        uint_fast8_t*  n_rd    = &input::bus_default::n_rd;
        uint_fast8_t*  n_ioreq = &input::bus_default::n_ioreq;
    }

}