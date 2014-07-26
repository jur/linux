/* SCEI_SYM_OWNER */
/*
 * linux/include/asm-mips/ps2/sound.h
 *
 *        Copyright (C) 2001  Sony Computer Entertainment Inc.
 *        Copyright (C) 2012  Mega Man
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License Version 2. See the file "COPYING" in the main
 * directory of this archive for more details.
 *
 * $Id$
 */

#ifndef __ASM_PS2_SOUND_H
#define __ASM_PS2_SOUND_H

/*
 * voice transfer
 */
typedef struct ps2sd_voice_data {
    int addr;
    int len;
    unsigned char *data;
} ps2sd_voice_data;
#if 0 /* This function isn't implemented yet */
#define PS2SDCTL_VOICE_GET		_IOWR('V', 0, ps2sd_voice_data)
#endif
#define PS2SDCTL_VOICE_PUT		_IOW ('V', 1, ps2sd_voice_data)

/*
 * SPU2 native format mode
 */
#define PS2SDCTL_SET_INTMODE		_IOW ('V', 2, int)
#define PS2SD_INTMODE_NORMAL		0
#define PS2SD_INTMODE_512		1

/*
 * SPDIF out mode
 */
#define PS2SDCTL_SET_SPDIFMODE		_IOW ('V', 3, int)
#define SD_SPDIF_OUT_PCM	0
#define SD_SPDIF_OUT_BITSTREAM  1
#define SD_SPDIF_OUT_OFF	2
#define SD_SPDIF_COPY_NORMAL    0x00
#define SD_SPDIF_COPY_PROHIBIT  0x80
#define SD_SPDIF_MEDIA_CD       0x000
#define SD_SPDIF_MEDIA_DVD      0x800
#define SD_BLOCK_MEM_DRY        0  /* no use */

/*
 * IOP memory
 */
#define PS2SDCTL_IOP_ALLOC		_IOWR('V', 4, ps2sd_voice_data)
#define PS2SDCTL_IOP_FREE		_IO  ('V', 5)
#if 0 /* This function isn't implemented yet */
#define PS2SDCTL_IOP_GET		_IOW ('V', 6, ps2sd_voice_data)
#endif
#define PS2SDCTL_IOP_PUT		_IOW ('V', 7, ps2sd_voice_data)

/*
 * command
 */
/*
 * XXX, 
 * struct sbr_sound_remote_arg in asm/mips/ps2/sbcall.h and
 * struct ps2sd_command must have common members.
 * double check if you change this structure.
 */
typedef struct ps2sd_command {
    int result;
    int command;
    int args[126];
} ps2sd_command;
#define PS2SDCTL_COMMAND_INIT		_IO  ('V', 8)
#define PS2SDCTL_COMMAND		_IOWR('V', 9, ps2sd_command)
#define PS2SDCTL_COMMAND_KERNEL22	0xc0005609
#define PS2SDCTL_COMMAND_END		_IO  ('V', 10)

#define PS2SDCTL_CHANGE_THPRI		_IO  ('V', 11)

#endif /* __ASM_PS2_SOUND_H */
