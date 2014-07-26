/*
 *  Playstation 2 IOP memory heap management.
 *
 *  Copyright (C) 2000-2002 Sony Computer Entertainment Inc.
 *  Copyright (C) 2010-2013 Juergen Urban
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

#include <asm/mach-ps2/ps2.h>
#include <asm/mach-ps2/sifdefs.h>
#include <asm/mach-ps2/sbios.h>

static DECLARE_MUTEX(iopheap_sem);

EXPORT_SYMBOL(ps2sif_allociopheap);
EXPORT_SYMBOL(ps2sif_freeiopheap);
EXPORT_SYMBOL(ps2sif_phystobus);
EXPORT_SYMBOL(ps2sif_bustophys);

int __init ps2sif_initiopheap(void)
{
    volatile int i;
    int result;
    int err;

    while (1) {
	down(&iopheap_sem);
	err = sbios_rpc(SBR_IOPH_INIT, NULL, &result);
	up(&iopheap_sem);

	if (err < 0)
	    return -1;
	if (result == 0)
	    break;
	i = 0x100000;
	while (i--)
	    ;
    }
    return 0;
}

dma_addr_t ps2sif_allociopheap(int size)
{
    struct sbr_ioph_alloc_arg arg;
    int result;
    int err;
    
    arg.size = size;

    down(&iopheap_sem);
    err = sbios_rpc(SBR_IOPH_ALLOC, &arg, &result);
    up(&iopheap_sem);

    if (err < 0)
		return 0;
    return result;
}

int ps2sif_freeiopheap(dma_addr_t addr)
{
    struct sbr_ioph_free_arg arg;
    int result;
    int err;
    
    arg.addr = addr;

    down(&iopheap_sem);
    err = sbios_rpc(SBR_IOPH_FREE, &arg, &result);
    up(&iopheap_sem);

    if (err < 0)
		return -1;
    return result;
}

dma_addr_t ps2sif_phystobus(phys_addr_t a)
{
	return((unsigned int)a - PS2_IOP_HEAP_BASE);
}

phys_addr_t ps2sif_bustophys(dma_addr_t a)
{
	return(a + PS2_IOP_HEAP_BASE);
}
