/* Copyright 2010 Mega Man */
#ifndef __ASM_PS2_PS2_H
#define __ASM_PS2_PS2_H

#include <linux/kernel.h>

/* Device name of PS2 SBIOS serial device. */
#define PS2_SBIOS_SERIAL_DEVICE_NAME "ttyS"

extern int ps2_pccard_present;
extern int ps2_pcic_type;
extern struct ps2_sysconf *ps2_sysconf;

extern void prom_putchar(char);
extern int ps2_printf(const char *fmt, ...);
void ps2_dev_init(void);
extern int ps2sif_initiopheap(void);

#endif
