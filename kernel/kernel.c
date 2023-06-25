#include "stdint.h"
#include "stdbool.h"

#include "armcpu.h"
#include "kernel.h"

#define SET_BIT(p,n) ((p) |=  (1 << (n)))
#define CLR_BIT(p,n) ((p) &= ~(1 << (n)))


void Kernel_start(void)
{
	enable_irq();
    Kernel_task_start();
}

void Kernel_yield(void)
{
    Kernel_task_scheduler();
}

void Kernel_send_events(uint32_t event_list)
{
	for (uint32_t i = 0; i < 32; i++) {
		if ((event_list >> i) & 1) {
			KernelEventFlag_t evt = KernelEventFlag_Empty;
			evt = (KernelEventFlag_t)SET_BIT(evt, i);
			Kernel_event_flag_set(evt);
		}
	}
}

KernelEventFlag_t Kernel_wait_events(uint32_t event_list)
{
	for (uint32_t i = 0; i < 32; i++) {
		if ((event_list >> i) & 1) {
			KernelEventFlag_t evt = KernelEventFlag_Empty;
			evt = (KernelEventFlag_t)SET_BIT(evt, i);
			if (Kernel_event_flag_check(evt)) return evt;
		}
	}
	return KernelEventFlag_Empty;
}

bool Kernel_send_msg(KernelMsgQueue_t q, void* data, uint32_t count)
{
	uint8_t* d = (uint8_t*)data;
	for (uint32_t i = 0; i < count; i++, d++) {
		if (kernel_MsgQueue_enqueue(q, *d) == false) {
			for (uint32_t j = 0; j < i; j++) {
				uint8_t rollback;
				kernel_MsgQueue_dequeue(q, &rollback);
			}
			return false;
		}
	}
	return true;
}

uint32_t Kernel_recv_msg(KernelMsgQueue_t q, void* data, uint32_t count)
{
	uint8_t* d = (uint8_t*)data;
	for (uint32_t i = 0; i < count; i++, d++)
		if (kernel_MsgQueue_dequeue(q, d) == false) return i;
	return count;
}

void Kernel_lock_sem(void)
{
	while (Kernel_sem_test() == false) Kernel_yield();
}

void Kernel_unlock_sem(void)
{
	Kernel_sem_release();
}