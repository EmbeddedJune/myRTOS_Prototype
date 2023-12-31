#ifndef KERNEL_KERNEL_H_
#define KERNEL_KERNEL_H_

#include "stdint.h"
#include "stdbool.h"
#include "task.h"
#include "event.h"
#include "msg.h"
#include "synch.h"

void                Kernel_start(void);
void                Kernel_yield(void);
void                Kernel_send_events(uint32_t event_list);
KernelEventFlag_t   Kernel_wait_events(uint32_t waiting_list);
bool                Kernel_send_msg(KernelMsgQueue_t q, void* data, uint32_t count);
uint32_t            Kernel_recv_msg(KernelMsgQueue_t q, void* data, uint32_t count);
void                Kernel_lock_sem(void);
void                Kernel_unlock_sem(void);

#endif
