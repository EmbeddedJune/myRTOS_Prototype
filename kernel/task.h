#ifndef KERNEL_TASK_H_
#define KERNEL_TASK_H_

#include "MemoryMap.h"

#define NOT_ENOUGH_TASK_NUM 0xFFFFFFFF
#define USR_TASK_STACK_SIZE 0x100000    // 1MiB
#define MAX_TASK_NUM (TASK_STACK_SIZE / USR_TASK_STACK_SIZE)    // 64

typedef struct KernelTaskContext_t {
    uint32_t spsr;
    uint32_t r0, r1, r2, r3, r4,
            r5, r6, r7, r8, r9,
            r10, r11, r12;
    uint32_t pc;
} KernelTaskContext_t;

typedef struct KernelTCB_t {
    uint32_t sp;
    uint32_t stack_base;
    uint32_t priority;
} KernelTCB_t;

// RTOS task는 반환값도, 매개변수도 없습니다.
typedef void (*KernelTaskFunc_t)(void);

void        Kernel_task_init(void);
uint32_t    Kernel_task_create(KernelTaskFunc_t taskHandler, uint32_t priority);
void        Kernel_task_scheduler(void);
void        Kernel_task_start(void);

#endif