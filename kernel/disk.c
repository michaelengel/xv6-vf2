#include "include/types.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/riscv.h"

#include "include/buf.h"

#ifndef QEMU
#include "include/sdcard.h"
#include "include/dmac.h"
#else
#include "include/virtio.h"
#endif 

void ramdiskrw(struct buf *b, int write);

void disk_init(void)
{
}

void disk_read(struct buf *b)
{
  ramdiskrw(b, 0);
}

void disk_write(struct buf *b)
{
  ramdiskrw(b, 1);
}

void disk_intr(void)
{
}
