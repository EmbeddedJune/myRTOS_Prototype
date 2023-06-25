#include "stdint.h"
#include "stdbool.h"

#include "event.h"

static uint32_t eventFlag;

void Kernel_event_flag_init(void)
{
	eventFlag = 0;
}

void Kernel_event_flag_set(KernelEventFlag_t event)
{
	eventFlag |= (uint32_t)event;
}

void Kernel_event_flag_clear(KernelEventFlag_t event)
{
	eventFlag &= ~((uint32_t)event);
}

bool Kernel_event_flag_check(KernelEventFlag_t event)
{
	if (eventFlag & (uint32_t)event) {
		Kernel_event_flag_clear(event);
		return true;
	}
	return false;
}