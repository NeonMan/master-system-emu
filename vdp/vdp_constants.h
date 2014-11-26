#ifndef __VDP_CONSTANTS_H
#define __VDP_CONSTANTS_H

#include <stdint.h>

// --- VDP Colors ---
#define VDP_COLOR_RGBA(R,G,B,A) ((uint32_t)((A<<24)+(R<<16)+(G<<8)+B))
#define VDP_COLOR_RGB(R,G,B) VDP_COLOR_RGBA(R,G,B,255)

#define VDP_COLOR_NONE VDP_COLOR_RGBA(128, 128, 128, 0)
#define VDP_COLOR_BLACK VDP_COLOR_RGB(0, 0, 0)
#define VDP_COLOR_MGREEN VDP_COLOR_RGB(33, 200, 66)
#define VDP_COLOR_LGREEN VDP_COLOR_RGB(94, 220, 120)

#define VDP_COLOR_DBLUE VDP_COLOR_RGB(84, 85, 237)
#define VDP_COLOR_LBLUE VDP_COLOR_RGB(125, 118, 252)
#define VDP_COLOR_DRED VDP_COLOR_RGB(212, 82, 77)
#define VDP_COLOR_CYAN VDP_COLOR_RGB(66, 235, 245)

#define VDP_COLOR_MRED VDP_COLOR_RGB(252, 85, 84)
#define VDP_COLOR_LRED VDP_COLOR_RGB(255, 121, 120)
#define VDP_COLOR_DYELLOW VDP_COLOR_RGB(212, 193, 84)
#define VDP_COLOR_LYELLOW VDP_COLOR_RGB(230, 206, 128)

#define VDP_COLOR_DGREEN VDP_COLOR_RGB(33, 176, 59)
#define VDP_COLOR_MAGENTA VDP_COLOR_RGB(201, 91, 186)
#define VDP_COLOR_GRAY VDP_COLOR_RGB(204, 204, 204)
#define VDP_COLOR_WHITE VDP_COLOR_RGB(255, 255, 255)

uint32_t vdp_tmscolors[16] = {
    VDP_COLOR_NONE, VDP_COLOR_BLACK, VDP_COLOR_MGREEN, VDP_COLOR_LGREEN,
    VDP_COLOR_DBLUE, VDP_COLOR_LBLUE, VDP_COLOR_DRED, VDP_COLOR_CYAN,
    VDP_COLOR_MRED, VDP_COLOR_LRED, VDP_COLOR_DYELLOW, VDP_COLOR_LYELLOW,
    VDP_COLOR_DGREEN, VDP_COLOR_MAGENTA, VDP_COLOR_GRAY, VDP_COLOR_WHITE
};

#endif