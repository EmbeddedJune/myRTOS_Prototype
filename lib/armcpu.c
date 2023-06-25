#include "armcpu.h"

void enable_irq(void)
{
    __asm__ ("PUSH {R0, R1}");      // Backup R0, R1 to stack
    __asm__ ("MRS R0, CPSR");       // R0 <- CPSR
    __asm__ ("BIC R1, R0, #0x80");  // Clear CPSR I-bit (Enable IRQ)
    __asm__ ("MSR CPSR, R1");       // CPSR <- R1
    __asm__ ("POP {R0, R1}");       // Restore R0, R1 from stack
}

void disable_irq(void)
{
    __asm__ ("PUSH {R0, R1}");      // Backup R0, R1 to stack
    __asm__ ("MRS R0, CPSR");       // R0 <- CPSR
    __asm__ ("ORR R1, R0, #0x80");  // Set CPSR I-bit (Disable IRQ)
    __asm__ ("MSR CPSR, R1");       // CPSR <- R1
    __asm__ ("POP {R0, R1}");       // Restore R0, R1 from stack
}

void enable_fiq(void)
{
    __asm__ ("PUSH {R0, R1}");      // Backup R0, R1 to stack
    __asm__ ("MRS R0, CPSR");       // R0 <- CPSR
    __asm__ ("BIC R1, R0, #0x40");  // Clear CPSR F-bit (Enable FIQ)
    __asm__ ("MSR CPSR, R1");       // CPSR <- R1
    __asm__ ("POP {R0, R1}");       // Restore R0, R1 from stack
}

void disable_fiq(void)
{
    __asm__ ("PUSH {R0, R1}");      // Backup R0, R1 to stack
    __asm__ ("MRS R0, CPSR");       // R0 <- CPSR
    __asm__ ("ORR R1, R0, #0x40");  // Clear CPSR F-bit (Disable FIQ)
    __asm__ ("MSR CPSR, R1");       // CPSR <- R1
    __asm__ ("POP {R0, R1}");       // Restore R0, R1 from stack
}
