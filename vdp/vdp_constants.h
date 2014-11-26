#ifndef __VDP_CONSTANTS_H
#define __VDP_CONSTANTS_H

#include <stdint.h>

// --- VDP Colors ---
struct vdp_color_s{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a; //255 is solid, 0 is transparent
};


#define VDP_COLOR_NONE { 0, 0, 0, 0 }
#define VDP_COLOR_BLACK { 0, 0, 0, 255 }
#define VDP_COLOR_MGREEN { 33, 200, 66, 255 }
#define VDP_COLOR_LGREEN { 94, 220, 120, 255 }

#define VDP_COLOR_DBLUE { 84, 85, 237, 255 }
#define VDP_COLOR_LBLUE { 128, 128, 128, 255 }
#define VDP_COLOR_DRED { 128, 128, 128, 255 }
#define VDP_COLOR_CYAN { 128, 128, 128, 255 }

#define VDP_COLOR_MRED { 128, 128, 128, 255 }
#define VDP_COLOR_LRED { 128, 128, 128, 255 }
#define VDP_COLOR_DYELLOW { 128, 128, 128, 255 }
#define VDP_COLOR_LYELLOW { 128, 128, 128, 255 }

#define VDP_COLOR_DGREEN { 128, 128, 128, 255 }
#define VDP_COLOR_MAGENTA { 128, 128, 128, 255 }
#define VDP_COLOR_GRAY { 128, 128, 128, 255 }
#define VDP_COLOR_WHITE { 128, 128, 128, 255 }

const struct vdp_color_s vdp_tmscolors[16] = {
    VDP_COLOR_NONE, VDP_COLOR_BLACK, VDP_COLOR_MGREEN, VDP_COLOR_LGREEN,
    VDP_COLOR_DBLUE, VDP_COLOR_LBLUE, VDP_COLOR_DRED, VDP_COLOR_CYAN,
    VDP_COLOR_MRED, VDP_COLOR_LRED, VDP_COLOR_DYELLOW, VDP_COLOR_LYELLOW,
    VDP_COLOR_DGREEN, VDP_COLOR_MAGENTA, VDP_COLOR_GRAY, VDP_COLOR_WHITE
};

#endif