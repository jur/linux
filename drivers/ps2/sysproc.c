/*
 *  PlayStation 2 ps2sysproc
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

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>

/* sysvers */
#include <asm/mach-ps2/bootinfo.h>

/* sysconf */
#include <asm/mach-ps2/sysconf.h>
#include <asm/mach-ps2/ps2.h>

#define INIT_PROC	(1<< 3)
static int init_flags;

static int proc_calc_metrics(char *page, char **start, off_t off,
				 int count, int *eof, int len)
{
	if (len <= off+count) *eof = 1;
	*start = page + off;
	len -= off;
	if (len>count) len = count;
	if (len<0) len = 0;
	return len;
}


int get_ps2sysvers(char *page, char **start, off_t off,
				 int count, int *eof, void *data)
{
        int len;

	sprintf(page,
		"MODELNAME=\"%s\"\n"
		"PS1DRVROMVERSION=\"%s\"\n"
		"PS1DRVHDDVERSION=\"%s\"\n"
		"PS1DRVPATH=\"%s\"\n"
		"VM=\"%s\"\n"
		"RB=\"%s\"\n"
		"DVDIDCHAR=\"%s\"\n"
		"DVDROMVERSION=\"%s\"\n"
		"DVDHDDVERSION=\"%s\"\n"
		"DVDPATH=\"%s\"\n",
		ps2_bootinfo->ver_model,
		ps2_bootinfo->ver_ps1drv_rom,
		ps2_bootinfo->ver_ps1drv_hdd,
		ps2_bootinfo->ver_ps1drv_path,
		ps2_bootinfo->ver_vm,
		ps2_bootinfo->ver_rb,
		ps2_bootinfo->ver_dvd_id,
		ps2_bootinfo->ver_dvd_rom,
		ps2_bootinfo->ver_dvd_hdd,
		ps2_bootinfo->ver_dvd_path);
	
	len = strlen(page);
	return proc_calc_metrics(page, start, off, count, eof, len);
}


int get_ps2sysconf(char *page, char **start, off_t off,
				 int count, int *eof, void *data)
{
        int len;

	sprintf(page,
		"EXDEVICE=0x%04x\n"
		"RGBYC=%d\n"
		"SPDIF=%d\n"
		"ASPECT=%d\n"
		"LANGUAGE=%d\n"
		"TIMEZONE=%d\n"
		"SUMMERTIME=%d\n"
		"DATENOTATION=%d\n"
		"TIMENOTATION=%d\n",
		ps2_pccard_present,
		ps2_sysconf->video,
		ps2_sysconf->spdif,
		ps2_sysconf->aspect,
		ps2_sysconf->language,
		ps2_sysconf->timezone,
		ps2_sysconf->summertime,
		ps2_sysconf->datenotation,
		ps2_sysconf->timenotation);

	len = strlen(page);
	return proc_calc_metrics(page, start, off, count, eof, len);
}


int __init ps2sysproc_init(void)
{

#ifdef CONFIG_PROC_FS
	create_proc_read_entry("ps2sysvers", 0, 0, get_ps2sysvers, NULL);
	create_proc_read_entry("ps2sysconf", 0, 0, get_ps2sysconf, NULL);
	init_flags |= INIT_PROC;
#endif
	return 0;
}

void
ps2sysproc_cleanup(void)
{
#ifdef CONFIG_PROC_FS
	if (init_flags & INIT_PROC)
		remove_proc_entry("ps2sysvers", NULL);
		remove_proc_entry("ps2sysconf", NULL);
	init_flags &= ~INIT_PROC;
#endif
}

module_init(ps2sysproc_init);
module_exit(ps2sysproc_cleanup);

MODULE_AUTHOR("Sony Computer Entertainment Inc.");
MODULE_DESCRIPTION("PlayStation 2 System proc");
MODULE_LICENSE("GPL");

