/*
 *  PlayStation 2 Game Controller driver
 *
 *        Copyright (C) 2000, 2001  Sony Computer Entertainment Inc.
 *        Copyright (C) 2011        Mega Man
 *
 *  This file is subject to the terms and conditions of the GNU General
 *  Public License Version 2. See the file "COPYING" in the main
 *  directory of this archive for more details.
 *
 * $Id$
 */

#include <asm/mach-ps2/sbios.h>
#include <asm/mach-ps2/sifdefs.h>
#include <linux/ps2/pad.h>

#define PadStateDiscon		(0)
#define PadStateFindPad		(1)
#define PadStateFindCTP1	(2)
#define PadStateExecCmd		(5)
#define PadStateStable		(6)
#define PadStateError		(7)

#define PadReqStateComplete	(0)
#define PadReqStateFaild	(1)
#define PadReqStateFailed	(1)
#define PadReqStateBusy		(2)

#define PS2PAD_NPORTS            2
#define PS2PAD_NSLOTS            1 /* currently, we doesn't support multitap */
#define PS2PAD_MAXNPADS          8

struct ps2pad_libctx {
	int port, slot;
	void *dmabuf;
};

extern struct ps2pad_libctx ps2pad_pads[];
extern int ps2pad_npads;

void ps2pad_js_init(void);
void ps2pad_js_quit(void);

int ps2pad_stat_conv(int stat);
