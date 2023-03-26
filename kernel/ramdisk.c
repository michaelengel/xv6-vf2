//
// ramdisk that uses the disk image loaded by qemu -initrd fs.img
//

#include "include/types.h"
#include "include/riscv.h"
#include "include/defs.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/spinlock.h"
#include "include/sleeplock.h"
#include "include/buf.h"
#include "include/ramdisk.h"

void
ramdiskinit(void)
{
}

// If B_DIRTY is set, write buf to disk, clear B_DIRTY, set B_VALID.
// Else if B_VALID is not set, read buf from disk, set B_VALID.
void
ramdiskrw(struct buf *b, int write)
{
  if(!holdingsleep(&b->lock))
    panic("include/ramdiskrw: buf not locked");

  if(b->sectorno >= FSSIZE)
    panic("include/ramdiskrw: sectorno too big");

  uint64 diskaddr = b->sectorno * BSIZE;
  char *addr = (char *)fs_img + diskaddr;

  if(write /* || (b->flags & B_DIRTY) */ ){
    // write
    memmove(addr, b->data, BSIZE);
  } else {
    // read
    memmove(b->data, addr, BSIZE);
  }
}

