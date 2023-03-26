// Copyright (c) 2006-2019 Frans Kaashoek, Robert Morris, Russ Cox,
//                         Massachusetts Institute of Technology

#include "include/types.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/riscv.h"
#include "include/sbi.h"
#include "include/console.h"
#include "include/printf.h"
#include "include/kalloc.h"
#include "include/timer.h"
#include "include/trap.h"
#include "include/proc.h"
#include "include/plic.h"
#include "include/vm.h"
#include "include/disk.h"
#include "include/buf.h"
#ifndef QEMU
#include "include/sdcard.h"
#include "include/fpioa.h"
#include "include/dmac.h"
#endif

volatile int boothartid = -1;
extern int sbi_console;
void uart_init(void);
void uart_putchar(int);

static inline void inithartid(unsigned long hartid) {
  asm volatile("mv tp, %0" : : "r" (hartid & 0x1));
}

volatile static int started = 0;

void
main(unsigned long hartid, unsigned long dtb_pa)
{
  sbi_console = 1;
  inithartid(hartid);
  
  printf("xv6 starting...\n");
  if (boothartid == -1) {
    // start first hartid
    boothartid = hartid;

    printf("hartid %d is the boot core...\n", hartid);
    consoleinit();
    printfinit();   // init a lock for printf 
    print_logo();

    kinit();         // physical page allocator
    printf("hartid %d after kinit...\n", hartid);
    kvminit();       // create kernel page table
    printf("hartid %d after kvminit...\n", hartid);
    kvminithart();   // turn on paging
    printf("hartid %d after kvminithart...\n", hartid);
    timerinit();     // init a lock for timer
    printf("hartid %d after timerinit...\n", hartid);
    trapinithart();  // install kernel trap vector, including interrupt handler
    printf("hartid %d after trapinithart...\n", hartid);
    uart_init();
    printf("hartid %d after uart_init...\n", hartid);
    procinit();
    printf("hartid %d after procinit...\n", hartid);
    plicinit();
    printf("hartid %d after plicinit...\n", hartid);
    plicinithart(hartid);
    printf("hartid %d after plicinithart...\n", hartid);
    // disk_init();
    binit();         // buffer cache
    fileinit();      // file table
    userinit();      // first user process
    printf("hart %d init done\n", hartid);


    // printf("switching from SBI to UART console\n");
    // sbi_console = 0;
    
    // bring up the remaining harts - note that hart 0 is the S7 (no MMU)
#if 0
    for(int i = 1; i < 4; i++) {
      if (i == hartid) continue; // the boot hart is already started...
        printf("Starting hart %d\n", i);
        sbi_hart_start(i, 0x80400000);
//        while ((r = sbi_hart_get_status(i) != 0))
//           printf("hart %d status %x\n", i, r);
    }
    printf("All harts started\n");
#endif

    __sync_synchronize();
    started = 1;
  }
  else
  {
    printf("more hart!\n");

    while (started == 0)
      ;
    kvminithart();
    __sync_synchronize();

    printf("hartid %d continues\n", hartid);
    trapinithart();
    plicinithart(hartid);  // ask PLIC for device interrupts
    printf("hart 1 init done\n");
  }
  printf("scheduler on hart %d\n", hartid);
  scheduler();
}
