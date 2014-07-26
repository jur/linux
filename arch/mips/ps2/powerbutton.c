/*
 *  powerbutton.c: PlayStation 2 power button handling
 *
 *        Copyright (C) 2000-2002  Sony Computer Entertainment Inc.
 *        Copyright (C) 2012       Mega Man
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License Version 2. See the file "COPYING" in the main
 * directory of this archive for more details.
 *
 */
#include <linux/init.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

#include <asm/signal.h>
#include <asm/mach-ps2/ps2.h>
#include <asm/mach-ps2/sifdefs.h>
#include <asm/mach-ps2/sbios.h>

#define POWEROFF_SID 0x9090900

#define PWRBTN_MODE_SHUTDOWN             0x01
#define PWRBTN_MODE_ENABLE_AUTO_SHUTOFF  0x02


static ps2sif_clientdata_t cd_poweroff_rpc;
static int rpc_initialized;
DECLARE_MUTEX(poweroff_rpc_sema);

static void ps2_powerbutton_handler(void *);

static void poweroff_rpcend_notify(void *arg)
{
	complete((struct completion *)arg);
	return;
}

/* Install powerhook with RTE (module CDVDFSV).
 * This will not work with TGE.
 */
static int __init rte_powerhook(void)
{
	int res;
	struct sb_cdvd_powerhook_arg arg;

	/*
	 * XXX, you should get the CD/DVD lock.
	 * But it might be OK because this routine will be called
	 * in early stage of boot sequence.
	 */

	/* initialize CD/DVD */
	do {
		if (sbios_rpc(SBR_CDVD_INIT, NULL, &res) < 0)
			return -1;
	} while (res == -1);

	/* install power button hook */
	arg.func = ps2_powerbutton_handler;
	arg.arg = NULL;
	sbios(SB_CDVD_POWERHOOK, &arg);

	return 0;
}

/* Install powerhook with TGE (module poweroff.irx).
 * This will not work with RTE.
 */
static int __init tge_powerhook(void)
{
	int loop;
	struct completion compl;
	int rv;
	volatile int j;

	init_completion(&compl);

	/* bind poweroff.irx module */
	for (loop = 100; loop; loop--) {
		rv = ps2sif_bindrpc(&cd_poweroff_rpc, POWEROFF_SID,
			SIF_RPCM_NOWAIT, poweroff_rpcend_notify, (void *)&compl);
		if (rv < 0) {
			printk("poweroff.irx: bind rv = %d.\n", rv);
			break;
		}
		wait_for_completion(&compl);
		if (cd_poweroff_rpc.serve != 0)
			break;
		j = 0x010000;
		while (j--) ;
	}
	if (cd_poweroff_rpc.serve == 0) {
		printk("poweroff.irx bind error 1, power button will not work.\n");
		return -1;
	}
	rpc_initialized = -1;
	return 0;
}

int ps2_powerbutton_enable_auto_shutoff(int enable_auto_shutoff)
{
	struct completion compl;
	int rv;
	static u32 rpc_data[16] __attribute__ ((aligned(64)));

	if (!rpc_initialized) {
		return -1;
	}

	init_completion(&compl);
	down(&poweroff_rpc_sema);
	rpc_data[0] = enable_auto_shutoff;
	do {
		rv = ps2sif_callrpc(&cd_poweroff_rpc, PWRBTN_MODE_ENABLE_AUTO_SHUTOFF,
			SIF_RPCM_NOWAIT,
			rpc_data, 4,
			rpc_data, 4,
			poweroff_rpcend_notify,
			&compl);
	} while (rv == -E_SIF_PKT_ALLOC);
	if (rv != 0) {
		printk("ps2_powerbutton_enable_auto_shutoff callrpc failed, (%d)\n", rv);
	} else {
		wait_for_completion(&compl);
	}
	up(&poweroff_rpc_sema);
	return rv;
}


int __init ps2_powerbutton_init(void)
{
	int rte_rv;
	int tge_rv;

	rpc_initialized = 0;

#ifdef CONFIG_PS2_SBIOS_VER_CHECK
	if (sbios(SB_GETVER, NULL) < 0x0201)
	    	return (-1);
#endif

	rte_rv = rte_powerhook();
	tge_rv = tge_powerhook();

	if ((rte_rv == 0) || (tge_rv ==0)) {
		return 0;
	} else {
		return -1;
	}
}

static void ps2_powerbutton_handler(void *arg)
{
	/* give a SIGPWR signal to init proc */
	kill_cad_pid(SIGPWR, 0);
}
