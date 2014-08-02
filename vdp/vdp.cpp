#include "vdp.h"
#include <stdint.h>

namespace vdp{
    unsigned char vram[1024 * 64]; ///<-- 64 K of VRAM
    unsigned char cram[32]; ///<-- 32 Bytes of color RAM
}
