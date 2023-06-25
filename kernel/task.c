#include "stdint.h"
#include "stdbool.h"
#include "ARMv7AR.h"
#include "task.h"

static uint32_t current_tcb_index;      // 현재 실행중인 task의 TCB index
static uint32_t allocated_tcb_index;    // 현재까지 선점된 task의 TCB index (< MAX_TASK)
static KernelTCB_t task_list[MAX_TASK_NUM];
static KernelTCB_t* cur_TCB;            // 현재 실행중인 task의 TCB를 가리키는 포인터
static KernelTCB_t* next_TCB;           // 다음에 실행할 task의 TCB를 가리키는 포인터

static KernelTCB_t* Scheduler_roundrobin(void);
static KernelTCB_t* Scheduler_priority(void);


static __attribute__ ((naked))
void Save_context(void)
{
    // 현재 task의 스택에 context를 저장합니다.
    __asm__ ("PUSH {lr}");
    __asm__ ("PUSH {R0-R12}");
    __asm__ ("MRS R0, CPSR");
    __asm__ ("PUSH {R0}");
    // 현재 TCB에 현재 sp를 저장합니다.
    __asm__ ("LDR R0, =cur_TCB");
    __asm__ ("LDR R0, [R0]");
    __asm__ ("STMIA R0!, {SP}");
}

static __attribute ((naked))
void Restore_context(void)
{
    // 다음 TCB로부터 sp를 복구합니다.
    __asm__ ("LDR R0, =next_TCB");
    __asm__ ("LDR R0, [R0]");
    __asm__ ("LDMIA R0!, {sp}");
    // 다음 TCB로부터 다음 task의 context를 복구합니다.
    __asm__ ("POP {R0}");
    __asm__ ("MSR CPSR, R0");
    __asm__ ("POP {R0-R12}");
    __asm__ ("POP {PC}");
}


__attribute__ ((naked))
void Kernel_task_context_switching(void)
{
    __asm__ ("B Save_context");
    __asm__ ("B Restore_context");
}

void Kernel_task_init(void)
{
    current_tcb_index = 0;
    allocated_tcb_index = 0;
    for (uint32_t i = 0; i < MAX_TASK_NUM; ++i) {
        task_list[i].stack_base = TASK_STACK_START + (i * USR_TASK_STACK_SIZE);
        task_list[i].sp = task_list[i].stack_base + USR_TASK_STACK_SIZE - 4;
        task_list[i].sp -= sizeof(KernelTaskContext_t);

        KernelTaskContext_t* curTaskContext = (KernelTaskContext_t*)task_list[i].sp;
        curTaskContext->pc = 0;
        curTaskContext->spsr = ARM_MODE_BIT_SYS;
    }
}

uint32_t Kernel_task_create(KernelTaskFunc_t taskHandler, uint32_t priority)
{
    if (allocated_tcb_index >= MAX_TASK_NUM) return NOT_ENOUGH_TASK_NUM;

    KernelTCB_t* new_TCB = &task_list[allocated_tcb_index++];
    new_TCB->priority = priority;
    KernelTaskContext_t* curTaskContext = (KernelTaskContext_t*)new_TCB->sp;
    curTaskContext->pc = (uint32_t)taskHandler;

    return (allocated_tcb_index - 1);
}

void Kernel_task_scheduler(void)
{
    cur_TCB = &task_list[current_tcb_index];
    next_TCB = Scheduler_roundrobin();
    Kernel_task_context_switching();
}

static KernelTCB_t* Scheduler_roundrobin(void)
{
    current_tcb_index = (current_tcb_index + 1) % allocated_tcb_index;
    return &task_list[current_tcb_index];
}

static KernelTCB_t* Scheduler_priority(void)
{
    KernelTCB_t* local_cur_TCB = &task_list[current_tcb_index];
    for (uint32_t i = 0; i < allocated_tcb_index; ++i) {
        KernelTCB_t* local_next_TCB = &task_list[i];
        // 숫자가 높을수록 우선순위는 낮습니다.
        if (current_tcb_index != i && 
            local_next_TCB->priority <= local_cur_TCB->priority) {
            current_tcb_index = i;
            return local_next_TCB;
        }
    }
    return local_cur_TCB;
}

__attribute__ ((naked))
void Arch_start(void)
{
    __asm__ volatile ("B Restore_context");
}

void Kernel_task_start(void)
{
    next_TCB = &task_list[current_tcb_index];
    Arch_start();
}

