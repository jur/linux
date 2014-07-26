/*
 * linux/include/asm-mips/ps2/eedev.h
 *
 *	Copyright (C) 2000-2002  Sony Computer Entertainment Inc.
 *	Copyright (C) 2010       Mega Man
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License Version 2. See the file "COPYING" in the main
 * directory of this archive for more details.
 *
 * $Id$
 */
/* TBD: Unfinished state. Rework code. */
#ifndef __ASM_PS2_EEDEV_H
#define __ASM_PS2_EEDEV_H

#include <asm/types.h>
#include <asm/io.h>

#define ALIGN16(x)	(((unsigned long)(x) + 15) & ~15)
#define PACK32(x, y)	((x) | ((y) << 16))
#define PACK64(x, y)	((u64)(x) | ((u64)(y) << 32))

#define GSFB_SIZE		(4 * 1024 * 1024)
/* The scratchpad has no physical address, ise the following value to detect
 * the scratchpad in the TLB handlers.
 * The TLB handler expects that the highest bit is set.
 */
#define SPR_PHYS_ADDR		0x80000000
/* EntryLo0 flag to use scratchpad instead or normal RAM. */
#define SCRATCHPAD_RAM		0x80000000
/* Size of scratchpad memory. */
#define SPR_SIZE		16384

/* register defines */

#define IPUREG_CMD		0x10002000
#define IPUREG_CTRL		0x10002010
#define IPUREG_BP		0x10002020
#define IPUREG_TOP		0x10002030

#define GIFREG_BASE		0x10003000
#define GIFREG(x)		(inl(GIFREG_BASE + ((x) << 4)))
#define SET_GIFREG(x, val)	(outl(val, GIFREG_BASE + ((x) << 4)))
#define VIF0REG_BASE		0x10003800
#define VIF0REG(x)		(inl(VIF0REG_BASE + ((x) << 4)))
#define SET_VIF0REG(x, val)		(outl(val, VIF0REG_BASE + ((x) << 4)))
#define VIF1REG_BASE		0x10003c00
#define VIF1REG(x)		(inl(VIF1REG_BASE + ((x) << 4)))
#define SET_VIF1REG(x, val)	(outl(val, VIF1REG_BASE + ((x) << 4)))
#define VIFnREG(n, x)		\
	(inl(VIF0REG_BASE + ((n) * 0x0400) + ((x) << 4)))
#define SET_VIFnREG(n, x, val)		\
	(outl(val, VIF0REG_BASE + ((n) * 0x0400) + ((x) << 4)))

#define VIF0_FIFO		0x10004000
#define VIF1_FIFO		0x10005000
#define GIF_FIFO		0x10006000
#define IPU_O_FIFO		0x10007000
#define IPU_I_FIFO		0x10007010

#define GSSREG_BASE1		0x12000000
#define GSSREG_BASE2		0x12001000
#define GSSREG1(x)		(GSSREG_BASE1 + ((x) << 4))
#define GSSREG2(x)		(GSSREG_BASE2 + (((x) & 0x0f) << 4))

/* inline assembler functions */

union _dword {
        __u64 di;
        struct {
#ifdef CONFIG_CPU_LITTLE_ENDIAN
                __u32   lo, hi;
#else
                __u32   hi, lo;
#endif
        } si;
};

#if 0
static inline void store_double(unsigned long addr, unsigned long long val)
{
    union _dword src;

    src.di=val;
    __asm__ __volatile__(
        "	.set push\n"
        "	.set arch=r5900\n"
        "	pextlw         $8,%1,%0\n"
        "	sd             $8,(%2)\n"
        "	.set   pop"
        : : "r"(src.si.lo), "r"(src.si.hi), "r" (addr) : "$8");
}

static inline unsigned long long load_double(unsigned long addr)
{
    union _dword val;

    __asm__ __volatile__(
	"	.set	push\n"
	"	.set	arch=r5900\n"
	"	ld	$8,(%2)\n"
		/* 63-32th bits must be same as 31th bit */
	"	dsra	%1,$8,32\n" 
	"	dsll	%0,$8,32\n" 
	"	dsra	%0,%0,32\n"
	"	.set	pop"
	: "=r" (val.si.lo), "=r" (val.si.hi) : "r" (addr) : "$8");

    return val.di;
}
#endif

static inline void move_quad(unsigned long dest, unsigned long src)
{
    __asm__ __volatile__(
	"	.set	push\n"
	"	.set	arch=r5900\n"
	"	move	$8,%1\n"
	"	lq     $9,($8)\n"
	"	move	$8,%0\n"
	"	sq     $9,($8)\n"
	"	.set	pop"
	: : "r" (dest), "r" (src) : "$8", "$9" );
}

static inline void dummy_read_quad(unsigned long addr)
{
    __asm__ __volatile__(
	"	.set	push\n"
	"	.set	arch=r5900\n"
	"	lq	$9,(%0)\n"
	"	.set	pop"
	: : "r" (addr) : "$9" );
}

#endif /* __ASM_PS2_EEDEV_H */
