/*
 *  linux/drivers/ps2/ps2dev.c
 *
 *	Copyright (C) 2000-2002  Sony Computer Entertainment Inc.
 *	Copyright (C) 2010       Mega Man
 *
 *  This file is subject to the terms and conditions of the GNU General
 *  Public License Version 2. See the file "COPYING" in the main
 *  directory of this archive for more details.
 *
 *  $Id$
 */
/* TBD: Unfinished state. Rework code. */

#ifndef __PS2DEV_H
#define __PS2DEV_H

#include <linux/sched.h>
#include <linux/param.h>
#include <asm/types.h>
#include <asm/io.h>
#include <linux/ps2/dev.h>
#include <asm/mach-ps2/dma.h>

#define BUFTYPE_MEM	0	/* DMA buffer is allocated by ps2mem */
#define BUFTYPE_SPR	1	/* DMA buffer is scratchpad RAM */
#define BUFTYPE_USER	2	/* copy from/to user address space */

#define DMA_QUEUE_LIMIT_MAX	16
#define DMA_USER_LIMIT	(1 * 1024 * 1024)

/* structure defines */

struct page_list {
    int pages;
    struct page *page[0];
};

struct dma_devch {
    struct dma_channel *channel;
    struct dma_device *device;
    volatile int qct;
    volatile int qsize;
    int qlimit;
    wait_queue_head_t done;
};

struct dma_device {
    struct dma_devch devch[2];
    wait_queue_head_t empty;
    u32 intr_flag;
    u32 intr_mask;
    struct task_struct *ts;
    int sig;
    void *data;
    spinlock_t lock;
};

struct dma_dev_request {
    struct dma_request r;
    struct dma_devch *devch;		/* request device */
    int qsize;				/* request data size */
    void (*free)(struct dma_request *, struct dma_channel *);
};

#define init_dma_dev_request(_req, _ops, _devch, _qsize, _free)	\
    do { init_dma_request(&(_req)->r, (_ops)); \
	 (_req)->devch = (_devch); (_req)->qsize = (_qsize); \
	 (_req)->free = (_free); } while (0)

/* user mode DMA request */

struct udma_request {
    struct dma_dev_request r;
    struct dma_tag *tag;		/* DMA tag */
    unsigned long vaddr;		/* start virtual addr */
    unsigned long saddr;		/* scratchpad RAM addr */
    struct page_list *mem;		/* allocated buffer */
    volatile int *done;			/* pointer to DMA done flag */
};

struct udma_chain_request {
    struct dma_dev_request r;
    unsigned long taddr;		/* DMA tag addr */
    int tte;				/* tag transfer enable flag */
};

struct udma_sendl_request {
    struct dma_dev_request r;
    struct dma_tag *tag_head, *tag_tail;
    struct dma_tag *tag;
    void *mem_head, *mem_tail;
    unsigned long saddr;
};

struct udma_request_list {
    struct dma_dev_request r;
    int reqs, index;
    struct udma_request *ureq[0];
};

/* function prototypes */

/* ps2mem.c */
extern struct vm_area_struct *ps2mem_vma_cache;
extern struct file_operations ps2mem_fops;

/* ps2event.c */
extern struct file_operations ps2ev_fops;
void ps2ev_init(void);
void ps2ev_cleanup(void);

/* ps2image.c */
int ps2gs_loadimage(struct ps2_image *img, struct dma_device *dev, int async);
int ps2gs_storeimage(struct ps2_image *img, struct dma_device *dev);

/* ps2dma.c */
struct page_list *ps2pl_alloc(int pages);
struct page_list *ps2pl_realloc(struct page_list *list, int newpages);
void ps2pl_free(struct page_list *list);
int ps2pl_copy_from_user(struct page_list *list, void *from, long len);
int ps2pl_copy_to_user(void *to, struct page_list *list, long len);

int ps2dma_make_tag(unsigned long start, int len, struct dma_tag **tagp, struct dma_tag **lastp, struct page_list **memp);
void ps2dma_dev_end(struct dma_request *req, struct dma_channel *ch);
int ps2dma_check_and_add_queue(struct dma_dev_request *req, int nonblock);

int ps2dma_write(struct dma_device *dev, struct ps2_packet *pkt, int nonblock);
int ps2dma_send(struct dma_device *dev, struct ps2_packet *pkt, int async);
int ps2dma_send_list(struct dma_device *dev, int num, struct ps2_packet *pkts);
int ps2dma_send_chain(struct dma_device *dev, struct ps2_pchain *pchain);

int ps2dma_recv(struct dma_device *dev, struct ps2_packet *pkt, int async);
int ps2dma_recv_list(struct dma_device *dev, int num, struct ps2_packet *pkts);

int ps2dma_stop(struct dma_device *dev, int dir, struct ps2_pstop *pstop);
int ps2dma_get_qct(struct dma_device *dev, int dir, int param);
int ps2dma_set_qlimit(struct dma_device *dev, int dir, int param);
struct dma_device *ps2dma_dev_init(int send, int recv);
int ps2dma_finish(struct dma_device *dev);

#endif /* __PS2DEV_H */
