#ifndef __WIDGET_VDP_H
#define __WIDGET_VDP_H

#include <FL/Fl_Widget.H>
#include <FL/Fl_Box.H>
#include <cstdint>

#include "vdp/vdp.h"

class WidgetVdp : public Fl_Box {
private:
    uint8_t* vdp_vram;
    uint8_t* vdp_cram;
    struct vdp_s* vdp;

public:
    WidgetVdp(int x, int y, int w, int h, const char* label = 0L);
    virtual ~WidgetVdp();
    
    virtual void draw();
};

#endif
