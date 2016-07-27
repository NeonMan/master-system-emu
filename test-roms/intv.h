
#ifndef __INTV_H
#define __INTV_H

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