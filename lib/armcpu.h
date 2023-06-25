#ifndef LIB_ARMCPU_H
#define LIB_ARMCPU_H

// Turn On/Off IRQ by modifying CPSR
void enable_irq(void);
void disable_irq(void);

// Turn On/Off FIQ by modifying CPSR
void enable_fiq(void);
void disable_fiq(void);

#endif