#include "WidgetVdp.hxx"
#include <FL/fl_draw.H>
#include <assert.h>

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

void WidgetVdp::draw_cram() {
    //Label
    fl_color(0);
    fl_font(FL_HELVETICA, 14);
    fl_draw("CRAM", x(), y() + 16);

    //Draw CRAM colours
    const uint8_t color_lut[4] = { 0, 85, 170, 255 };
    const int x_offset = 48;
    const int y_offset = 0;

    //CRAM Background colours
    for (int i = 0; i < 16; i++) {
        uint32_t color;
        const uint8_t color_sms = this->vdp_cram[i];

        color = ((color_lut[(color_sms >> 0) & 0x3]) << 24);
        color |= ((color_lut[(color_sms >> 2) & 0x3]) << 16);
        color |= ((color_lut[(color_sms >> 4) & 0x3]) << 8);

        fl_draw_box(
            Fl_Boxtype::FL_FLAT_BOX,
            x_offset + x() + (9 * i),
            y_offset + y(),
            8,
            24,
            color
        );
    }

    //CRAM Sprite colours
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

///@brief Converts 8 VRAM pixels into a 8x8 pixel image to be drwan by FLTK.
///@param vram     VDP VRAM pointer.
///@param pix_buff bitmap buffer; 64Bytes.
///
///vram must contain at least 8 bytes and pix_buff must be at least 64bytes in size.
static void vram_to_dump_pixmap(const uint8_t* vram, uint8_t* pix_buff, uint8_t c_one, uint8_t c_zero) {
    for (int i = 0; i < 8; i++) { //For each byte
        uint8_t b = vram[i];
        for (int j = 0; j < 8; j++) { //For each bit in the byte
            pix_buff[(i * 8) + j] = (b & (1 << 7)) ? c_one : c_zero;
            b = b << 1;
        }
    }
}

void WidgetVdp::draw_vram() {
    const int x_offset = 48;
    const int y_offset = -16;

    //Label
    fl_color(0);
    fl_font(FL_HELVETICA, 14);
    fl_draw("VRAM", x(), y() + 48);

    //VRAM bit dump 
    {
        int vram_index;
        static uint8_t vram_bitmap[8 * 8 * 32 * 64]; //<-- Static to free some stack space.
        
        //#pragma omp parallel for private(vram_index)
        for (vram_index = 0; vram_index < (1024 * 16); vram_index += 8) { //For each 8-byte block
            //Draw those pixels int the vdp dialog
            int block_row;
            int block_col;

            //One column for every 8 bytes, 64 columns.
            block_col = (vram_index / 8) % 64; 

            //One row per every 8 * 64 bytes. 32 columns.
            block_row = (vram_index / 8) / 64;

            //Convert VRAM bytes into pixels
            uint8_t block_bitmap[8 * 8]; //<-- Buffer for the 8x8 bitmap
            vram_to_dump_pixmap(this->vdp_vram + vram_index, block_bitmap, 0xFF, (block_row&1) ? 0 : (1<<4));

            //Copy bytes to the full bitmap
            for (int block_line = 0; block_line < 8; block_line++) {
                int bitmap_offset =
                    (block_row * 8 * 64 * 8) +
                    (block_col * 8) +
                    (block_line * 8 * 64)
                    ;
                for (int line_pix = 0; line_pix < 8; line_pix++) {
                    vram_bitmap[bitmap_offset + line_pix] = block_bitmap[(block_line * 8) + line_pix];
                }
            }
        }

        //Draw it
        fl_draw_image_mono(
            (const uchar*)vram_bitmap,
            x_offset + x(),
            y_offset + y() + 48,
            64*8,
            32*8
        );
    }
}

void WidgetVdp::draw()
{
    //fl_draw_box(Fl_Boxtype::FL_ENGRAVED_BOX, x()+1, y()+1, 100, 100, 0);
    {
        //Clear background
        fl_draw_box(Fl_Boxtype::FL_FLAT_BOX, x(), y(), w(), h(), 49);

        draw_cram();
        draw_vram();
    }
}
