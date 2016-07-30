#ifndef __INTV_H
#define __INTV_H
/**
 * @file  intv.h
 * @brief Declaration of interrupt vector handlers.
 *
 * Interrupt vector entry points are coded on crt0 and will look for
 * a number of global functions. This functions _must_ be defined for
 * a successful build, even if they are an empty function.
 */

/*RST vectors*/
void rst_08h();
void rst_10h();
void rst_18h();
void rst_20h();
void rst_28h();
void rst_30h();
void rst_38h();

/*NMI vector*/
void nmi();

/*Maskable interrupt vector (mode 1)*/
void int1();

#endif