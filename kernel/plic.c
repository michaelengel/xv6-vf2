
#include "include/types.h"
#include "include/param.h"
#include "include/memlayout.h"
#include "include/riscv.h"
#include "include/sbi.h"
#include "include/plic.h"
#include "include/proc.h"
#include "include/printf.h"

//
// the riscv Platform Level Interrupt Controller (PLIC).
//

void plicinit(void) {
//	writed(1, PLIC_V + DISK_IRQ * sizeof(uint32));
	writed(1, PLIC_V + UART_IRQ * sizeof(uint32));

	#ifdef DEBUG 
	printf("plicinit\n");
	#endif 
}

void
plicinithart(int hart)
{
  printf("plicinithart 1\n");
  // int hart = cpuid();
  // set uart's enable bit for this hart's S-mode. 
  // *(uint32*)PLIC_SENABLE(hart)=0;
  *(uint32*)(PLIC_SENABLE(hart)+4)= (1 << (UART_IRQ-32));

  printf("plicinithart 2\n");

  // set this hart's S-mode priority threshold to 0.
  *(uint32*)PLIC_SPRIORITY(hart) = 0;
  printf("plicinithart 3\n");
}

// ask the PLIC what interrupt we should serve.
int
plic_claim(void)
{
  int hart = cpuid();
  int irq;
  irq = *(uint32*)PLIC_SCLAIM(hart);
  return irq;
}

// tell the PLIC we've served this IRQ.
void
plic_complete(int irq)
{
  int hart = cpuid();
  *(uint32*)PLIC_SCLAIM(hart) = irq;
}

