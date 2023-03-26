#ifndef __PLIC_H
#define __PLIC_H 

#include "memlayout.h"

#if defined(VF2)
#define UART_IRQ    32
#else
#error "unknown soc"
#endif 

void plicinit(void);

// enable PLIC for each hart 
void plicinithart(int);

// ask PLIC what interrupt we should serve 
int plic_claim(void);

// tell PLIC that we've served this IRQ 
void plic_complete(int irq);

#endif 
