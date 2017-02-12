#include "WidgetVdp.h"
#include <FL/fl_draw.H>

#include "vdp/vdp.h"
#include "vdp/vdp_render.h"

WidgetVdp::WidgetVdp(int x, int y, int w, int h, const char* label) : Fl_Box(x, y, w, h, label)
{
    this->vdp_cram = (uint8_t*) vdp_get_cram();
    this->vdp_vram = (uint8_t*) vdp_get_vram();
    this->vdp = vdp_get_state();
}

WidgetVdp::~WidgetVdp()
{

}

void WidgetVdp::draw()
{
    //fl_draw_box(Fl_Boxtype::FL_ENGRAVED_BOX, x()+1, y()+1, 100, 100, 0);
    {
        //Clear background
        fl_draw_box(Fl_Boxtype::FL_FLAT_BOX, x(), y(), w(), h(), 49);

        //Label
        fl_color(0);
        fl_font(FL_HELVETICA, 14);
        fl_draw("CRAM", x(), y() + 16);

        //Draw CRAM colours
        const uint8_t color_lut[4] = {0, 85, 170, 255};
        const int x_offset = 48;
        const int y_offset = 0;

        //Background
        for (int i = 0; i < 16; i++) {
            uint32_t color;
            const uint8_t color_sms = this->vdp_cram[i];

            color  = ((color_lut[(color_sms >> 0) & 0x3] ) << 24);
            color |= ((color_lut[(color_sms >> 2) & 0x3] ) << 16);
            color |= ((color_lut[(color_sms >> 4) & 0x3] ) << 8);

            fl_draw_box(
                Fl_Boxtype::FL_FLAT_BOX,
                x_offset + x() + (9*i),
                y_offset + y(),
                8,
                24,
                color
            );
        }

        //Sprite
        for (int i = 16; i < 32; i++) {
            uint32_t color;
            const uint8_t color_sms = this->vdp_cram[i];

            color = ((color_lut[(color_sms >> 0) & 0x3]) << 24);
            color |= ((color_lut[(color_sms >> 2) & 0x3]) << 16);
            color |= ((color_lut[(color_sms >> 4) & 0x3]) << 8);

            fl_draw_box(
                Fl_Boxtype::FL_FLAT_BOX,
                x_offset + x() + (9 * i) + 4,
                y_offset + y(),
                8,
                24,
                color
            );
        }
    }
}
