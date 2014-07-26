/* Copyright 2010 Mega Man */
#ifndef __ASM_PS2_PS2_H
#define __ASM_PS2_PS2_H

extern int ps2_pccard_present;
extern int ps2_pcic_type;
extern struct ps2_sysconf *ps2_sysconf;
/* PS2 console */
extern const struct consw ps2_con;

extern void prom_putchar(char);

#endif
