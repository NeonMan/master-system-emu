#ifndef __BBC_MICRO_FONT_H
#define __BBC_MICRO_FONT_H
/*------------------------------------------ */
/* In loving memory of Acorn Computers Ltd.  */
/*      Mike G proudly presents the:         */

/*                                           */
/*       BBC Micro Character Set!            */
/*       ========================            */
/*                                           */
/* This source was created automatically     */
/* from character data stored in the BBC     */
/* Microcomputer operating system ROM.       */
/*                                           */
/* The BBC set is a nice, clear font which   */
/* (unlike the classic Namco/Sega font)      */
/* includes lower-case letters as well as    */
/* upper-case.                               */
/*                                           */
/* Being a British micro, there's even a     */
/* Pound Sterling currency symbol, which     */
/* should come in handy when I begin to      */
/* write financial apps on the SMS. (Just    */
/* kidding!)                                 */
/*                                           */
/* Hopefully by using binary representation  */
/* it will be obvious how the character      */
/* data is stored.                           */
/*                                           */
/*------------------------------------------ */

/*Quick conversion into a C header */

#include <stdint.h>

#define BBC_FONT_BASE      0x20
#define BBC_FONT_SIZE      0x60
#define BBC_FONT_CHAR_SIZE 8
const uint8_t bbc_font[BBC_FONT_SIZE * BBC_FONT_CHAR_SIZE] = {

    /* Character 0x20 (space) */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,



    /* Character 0x21 ! */

    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,



    /* Character 0x22 " */

    /* %01101100 */ 0x6C,
    /* %01101100 */ 0x6C,
    /* %01101100 */ 0x6C,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,



    /* Character 0x23 # */

    /* %00110110 */ 0x36,
    /* %00110110 */ 0x36,
    /* %01111111 */ 0x7F,
    /* %00110110 */ 0x36,
    /* %01111111 */ 0x7F,
    /* %00110110 */ 0x36,
    /* %00110110 */ 0x36,
    /* %00000000 */ 0x00,



    /* Character 0x24 $ */

    /* %00001100 */ 0x0C,
    /* %00111111 */ 0x3F,
    /* %01101000 */ 0x68,
    /* %00111110 */ 0x3E,
    /* %00001011 */ 0x0B,
    /* %01111110 */ 0x7E,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,



    /* Character 0x25 % */

    /* %01100000 */ 0x60,
    /* %01100110 */ 0x66,
    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,
    /* %01100110 */ 0x66,
    /* %00000110 */ 0x06,
    /* %00000000 */ 0x00,



    /* Character 0x26 & */

    /* %00111000 */ 0x38,
    /* %01101100 */ 0x6C,
    /* %01101100 */ 0x6C,
    /* %00111000 */ 0x38,
    /* %01101101 */ 0x6D,
    /* %01100110 */ 0x66,
    /* %00111011 */ 0x3B,
    /* %00000000 */ 0x00,



    /* Character 0x27 ' */

    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,



    /* Character 0x28 ( */

    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,
    /* %00110000 */ 0x30,
    /* %00110000 */ 0x30,
    /* %00011000 */ 0x18,
    /* %00001100 */ 0x0C,
    /* %00000000 */ 0x00,



    /* Character 0x29 ) */

    /* %00110000 */ 0x30,
    /* %00011000 */ 0x18,
    /* %00001100 */ 0x0C,
    /* %00001100 */ 0x0C,
    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,
    /* %00000000 */ 0x00,



    /* Character 0x2A * */

    /* %00000000 */ 0x00,
    /* %00011000 */ 0x18,
    /* %01111110 */ 0x7E,
    /* %00111100 */ 0x3C,
    /* %01111110 */ 0x7E,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,



    /* Character 0x2B + */

    /* %00000000 */ 0x00,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %01111110 */ 0x7E,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,



    /* Character 0x2C , */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,



    /* Character 0x2D - */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %01111110 */ 0x7E,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,



    /* Character 0x2E . */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,



    /* Character 0x2F / */

    /* %00000000 */ 0x00,
    /* %00000110 */ 0x06,
    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,
    /* %01100000 */ 0x60,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,



    /* Character 0x30 0 */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01101110 */ 0x6E,
    /* %01111110 */ 0x7E,
    /* %01110110 */ 0x76,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x31 1 */

    /* %00011000 */ 0x18,
    /* %00111000 */ 0x38,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %01111110 */ 0x7E,
    /* %00000000 */ 0x00,



    /* Character 0x32 2 */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %00000110 */ 0x06,
    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,
    /* %01111110 */ 0x7E,
    /* %00000000 */ 0x00,



    /* Character 0x33 3 */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %00000110 */ 0x06,
    /* %00011100 */ 0x1C,
    /* %00000110 */ 0x06,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x34 4 */

    /* %00001100 */ 0x0C,
    /* %00011100 */ 0x1C,
    /* %00111100 */ 0x3C,
    /* %01101100 */ 0x6C,
    /* %01111110 */ 0x7E,
    /* %00001100 */ 0x0C,
    /* %00001100 */ 0x0C,
    /* %00000000 */ 0x00,



    /* Character 0x35 5 */

    /* %01111110 */ 0x7E,
    /* %01100000 */ 0x60,
    /* %01111100 */ 0x7C,
    /* %00000110 */ 0x06,
    /* %00000110 */ 0x06,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x36 6 */

    /* %00011100 */ 0x1C,
    /* %00110000 */ 0x30,
    /* %01100000 */ 0x60,
    /* %01111100 */ 0x7C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x37 7 */

    /* %01111110 */ 0x7E,
    /* %00000110 */ 0x06,
    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,
    /* %00110000 */ 0x30,
    /* %00110000 */ 0x30,
    /* %00000000 */ 0x00,



    /* Character 0x38 8 */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x39 9 */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111110 */ 0x3E,
    /* %00000110 */ 0x06,
    /* %00001100 */ 0x0C,
    /* %00111000 */ 0x38,
    /* %00000000 */ 0x00,



    /* Character 0x3A : */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,



    /* Character 0x3B ; */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,



    /* Character 0x3C < */

    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,
    /* %01100000 */ 0x60,
    /* %00110000 */ 0x30,
    /* %00011000 */ 0x18,
    /* %00001100 */ 0x0C,
    /* %00000000 */ 0x00,



    /* Character 0x3D = */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %01111110 */ 0x7E,
    /* %00000000 */ 0x00,
    /* %01111110 */ 0x7E,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,



    /* Character 0x3E > */

    /* %00110000 */ 0x30,
    /* %00011000 */ 0x18,
    /* %00001100 */ 0x0C,
    /* %00000110 */ 0x06,
    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,
    /* %00000000 */ 0x00,



    /* Character 0x3F ? */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,



    /* Character 0x40 @ */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01101110 */ 0x6E,
    /* %01101010 */ 0x6A,
    /* %01101110 */ 0x6E,
    /* %01100000 */ 0x60,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x41 A */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01111110 */ 0x7E,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00000000 */ 0x00,



    /* Character 0x42 B */

    /* %01111100 */ 0x7C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01111100 */ 0x7C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01111100 */ 0x7C,
    /* %00000000 */ 0x00,



    /* Character 0x43 C */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x44 D */

    /* %01111000 */ 0x78,
    /* %01101100 */ 0x6C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01101100 */ 0x6C,
    /* %01111000 */ 0x78,
    /* %00000000 */ 0x00,



    /* Character 0x45 E */

    /* %01111110 */ 0x7E,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01111100 */ 0x7C,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01111110 */ 0x7E,
    /* %00000000 */ 0x00,



    /* Character 0x46 F */

    /* %01111110 */ 0x7E,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01111100 */ 0x7C,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %00000000 */ 0x00,



    /* Character 0x47 G */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01100000 */ 0x60,
    /* %01101110 */ 0x6E,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x48 H */

    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01111110 */ 0x7E,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00000000 */ 0x00,



    /* Character 0x49 I */

    /* %01111110 */ 0x7E,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %01111110 */ 0x7E,
    /* %00000000 */ 0x00,



    /* Character 0x4A J */

    /* %00111110 */ 0x3E,
    /* %00001100 */ 0x0C,
    /* %00001100 */ 0x0C,
    /* %00001100 */ 0x0C,
    /* %00001100 */ 0x0C,
    /* %01101100 */ 0x6C,
    /* %00111000 */ 0x38,
    /* %00000000 */ 0x00,



    /* Character 0x4B K */

    /* %01100110 */ 0x66,
    /* %01101100 */ 0x6C,
    /* %01111000 */ 0x78,
    /* %01110000 */ 0x70,
    /* %01111000 */ 0x78,
    /* %01101100 */ 0x6C,
    /* %01100110 */ 0x66,
    /* %00000000 */ 0x00,



    /* Character 0x4C L */

    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01111110 */ 0x7E,
    /* %00000000 */ 0x00,



    /* Character 0x4D M */

    /* %01100011 */ 0x63,
    /* %01110111 */ 0x77,
    /* %01111111 */ 0x7F,
    /* %01101011 */ 0x6B,
    /* %01101011 */ 0x6B,
    /* %01100011 */ 0x63,
    /* %01100011 */ 0x63,
    /* %00000000 */ 0x00,



    /* Character 0x4E N */

    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01110110 */ 0x76,
    /* %01111110 */ 0x7E,
    /* %01101110 */ 0x6E,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00000000 */ 0x00,



    /* Character 0x4F O */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x50 P */

    /* %01111100 */ 0x7C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01111100 */ 0x7C,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %00000000 */ 0x00,



    /* Character 0x51 Q */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01101010 */ 0x6A,
    /* %01101100 */ 0x6C,
    /* %00110110 */ 0x36,
    /* %00000000 */ 0x00,



    /* Character 0x52 R */

    /* %01111100 */ 0x7C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01111100 */ 0x7C,
    /* %01101100 */ 0x6C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00000000 */ 0x00,



    /* Character 0x53 S */

    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01100000 */ 0x60,
    /* %00111100 */ 0x3C,
    /* %00000110 */ 0x06,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x54 T */

    /* %01111110 */ 0x7E,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,



    /* Character 0x55 U */

    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x56 V */

    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,



    /* Character 0x57 W */

    /* %01100011 */ 0x63,
    /* %01100011 */ 0x63,
    /* %01101011 */ 0x6B,
    /* %01101011 */ 0x6B,
    /* %01111111 */ 0x7F,
    /* %01110111 */ 0x77,
    /* %01100011 */ 0x63,
    /* %00000000 */ 0x00,



    /* Character 0x58 X */

    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00011000 */ 0x18,
    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00000000 */ 0x00,



    /* Character 0x59 Y */

    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,



    /* Character 0x5A Z */

    /* %01111110 */ 0x7E,
    /* %00000110 */ 0x06,
    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,
    /* %01100000 */ 0x60,
    /* %01111110 */ 0x7E,
    /* %00000000 */ 0x00,



    /* Character 0x5B [ */

    /* %01111100 */ 0x7C,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01111100 */ 0x7C,
    /* %00000000 */ 0x00,



    /* Character 0x5C \ */

    /* %00000000 */ 0x00,
    /* %01100000 */ 0x60,
    /* %00110000 */ 0x30,
    /* %00011000 */ 0x18,
    /* %00001100 */ 0x0C,
    /* %00000110 */ 0x06,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,



    /* Character 0x5D ] */

    /* %00111110 */ 0x3E,
    /* %00000110 */ 0x06,
    /* %00000110 */ 0x06,
    /* %00000110 */ 0x06,
    /* %00000110 */ 0x06,
    /* %00000110 */ 0x06,
    /* %00111110 */ 0x3E,
    /* %00000000 */ 0x00,



    /* Character 0x5E ^ */

    /* %00011000 */ 0x18,
    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01000010 */ 0x42,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,



    /* Character 0x5F (underscore) */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %11111111 */ 0xFF,



    /* Character 0x60 (Pound Sterling) */

    /* %00011100 */ 0x1C,
    /* %00110110 */ 0x36,
    /* %00110000 */ 0x30,
    /* %01111100 */ 0x7C,
    /* %00110000 */ 0x30,
    /* %00110000 */ 0x30,
    /* %01111110 */ 0x7E,
    /* %00000000 */ 0x00,



    /* Character 0x61 a */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00111100 */ 0x3C,
    /* %00000110 */ 0x06,
    /* %00111110 */ 0x3E,
    /* %01100110 */ 0x66,
    /* %00111110 */ 0x3E,
    /* %00000000 */ 0x00,



    /* Character 0x62 b */

    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01111100 */ 0x7C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01111100 */ 0x7C,
    /* %00000000 */ 0x00,



    /* Character 0x63 c */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01100000 */ 0x60,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x64 d */

    /* %00000110 */ 0x06,
    /* %00000110 */ 0x06,
    /* %00111110 */ 0x3E,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111110 */ 0x3E,
    /* %00000000 */ 0x00,



    /* Character 0x65 e */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01111110 */ 0x7E,
    /* %01100000 */ 0x60,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x66 f */

    /* %00011100 */ 0x1C,
    /* %00110000 */ 0x30,
    /* %00110000 */ 0x30,
    /* %01111100 */ 0x7C,
    /* %00110000 */ 0x30,
    /* %00110000 */ 0x30,
    /* %00110000 */ 0x30,
    /* %00000000 */ 0x00,



    /* Character 0x67 g */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00111110 */ 0x3E,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111110 */ 0x3E,
    /* %00000110 */ 0x06,
    /* %00111100 */ 0x3C,



    /* Character 0x68 h */

    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01111100 */ 0x7C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00000000 */ 0x00,



    /* Character 0x69 i */

    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,
    /* %00111000 */ 0x38,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x6A j */

    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,
    /* %00111000 */ 0x38,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %01110000 */ 0x70,



    /* Character 0x6B k */

    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100110 */ 0x66,
    /* %01101100 */ 0x6C,
    /* %01111000 */ 0x78,
    /* %01101100 */ 0x6C,
    /* %01100110 */ 0x66,
    /* %00000000 */ 0x00,



    /* Character 0x6C l */

    /* %00111000 */ 0x38,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x6D m */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00110110 */ 0x36,
    /* %01111111 */ 0x7F,
    /* %01101011 */ 0x6B,
    /* %01101011 */ 0x6B,
    /* %01100011 */ 0x63,
    /* %00000000 */ 0x00,



    /* Character 0x6E n */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %01111100 */ 0x7C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00000000 */ 0x00,



    /* Character 0x6F o */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00000000 */ 0x00,



    /* Character 0x70 p */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %01111100 */ 0x7C,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01111100 */ 0x7C,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,



    /* Character 0x71 q */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00111110 */ 0x3E,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111110 */ 0x3E,
    /* %00000110 */ 0x06,
    /* %00000111 */ 0x07,



    /* Character 0x72 r */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %01101100 */ 0x6C,
    /* %01110110 */ 0x76,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %01100000 */ 0x60,
    /* %00000000 */ 0x00,



    /* Character 0x73 s */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00111110 */ 0x3E,
    /* %01100000 */ 0x60,
    /* %00111100 */ 0x3C,
    /* %00000110 */ 0x06,
    /* %01111100 */ 0x7C,
    /* %00000000 */ 0x00,



    /* Character 0x74 t */

    /* %00110000 */ 0x30,
    /* %00110000 */ 0x30,
    /* %01111100 */ 0x7C,
    /* %00110000 */ 0x30,
    /* %00110000 */ 0x30,
    /* %00110000 */ 0x30,
    /* %00011100 */ 0x1C,
    /* %00000000 */ 0x00,



    /* Character 0x75 u */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111110 */ 0x3E,
    /* %00000000 */ 0x00,



    /* Character 0x76 v */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,



    /* Character 0x77 w */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %01100011 */ 0x63,
    /* %01101011 */ 0x6B,
    /* %01101011 */ 0x6B,
    /* %01111111 */ 0x7F,
    /* %00110110 */ 0x36,
    /* %00000000 */ 0x00,



    /* Character 0x78 x */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %01100110 */ 0x66,
    /* %00111100 */ 0x3C,
    /* %00011000 */ 0x18,
    /* %00111100 */ 0x3C,
    /* %01100110 */ 0x66,
    /* %00000000 */ 0x00,



    /* Character 0x79 y */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %01100110 */ 0x66,
    /* %00111110 */ 0x3E,
    /* %00000110 */ 0x06,
    /* %00111100 */ 0x3C,



    /* Character 0x7A z */

    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %01111110 */ 0x7E,
    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,
    /* %01111110 */ 0x7E,
    /* %00000000 */ 0x00,



    /* Character 0x7B { */

    /* %00001100 */ 0x0C,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %01110000 */ 0x70,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00001100 */ 0x0C,
    /* %00000000 */ 0x00,



    /* Character 0x7C (OR symbol) */

    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00000000 */ 0x00,



    /* Character 0x7D } */

    /* %00110000 */ 0x30,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00001110 */ 0x0E,
    /* %00011000 */ 0x18,
    /* %00011000 */ 0x18,
    /* %00110000 */ 0x30,
    /* %00000000 */ 0x00,



    /* Character 0x7E ~ */

    /* %00110001 */ 0x31,
    /* %01101011 */ 0x6B,
    /* %01000110 */ 0x46,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,
    /* %00000000 */ 0x00,



    /* Character 0x7F (block) */

    /* %11111111 */ 0xFF,
    /* %11111111 */ 0xFF,
    /* %11111111 */ 0xFF,
    /* %11111111 */ 0xFF,
    /* %11111111 */ 0xFF,
    /* %11111111 */ 0xFF,
    /* %11111111 */ 0xFF,
    /* %11111111 */ 0xFF
};

/*------------------------------------------ */
/*                                           */
/* That's all folks! Now that Acorn is no    */
/* longer with us (RIP), it should be safe   */
/* to use this font in any homebrew          */
/* projects.                                 */
/*                                           */
/* Anyone is welcome to distribute and use   */
/* this source file for whatever purpose     */
/* they wish.                                */
/*                                           */
/*                                           */
/* Best wishes,                              */
/*                                           */
/* Mike G. (mike@mikeg2.freeserve.co.uk)     */
/*                                           */
/* 4th March 2001                            */
/*                                           */
/* SMS Power! - www.smspower.org             */
/* SMSReader - www.smspower.org/smsreader/   */
/*                                           */
/* My Home Page: www.mikeg2.freeserve.co.uk  */
/*                                           */
/*------------------------------------------ */

#endif
