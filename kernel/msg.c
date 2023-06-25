#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"

#include "msg.h"

KernelCirQueue_t messageQueue[KernelMsgQueue_Num];

void Kernel_MsgQueue_init(void)
{
	for (uint32_t i = 0; i < KernelMsgQueue_Num; ++i) {
		messageQueue[i].front = 0;
		messageQueue[i].rear = 0;
		memclr(messageQueue[i].queue, MSG_QUEUE_SIZE_BYTE);
	}
}

bool Kernel_MsgQueue_is_empty(KernelMsgQueue_t q)
{
	if (q >= KernelMsgQueue_Num) return false;
	return (messageQueue[q].rear == messageQueue[q].front);
}

bool Kernel_MsgQueue_is_full(KernelMsgQueue_t q)
{
	if (q >= KernelMsgQueue_Num) return false;
	return ((messageQueue[q].rear + 1) % MSG_QUEUE_SIZE_BYTE == messageQueue[q].front);
}

bool kernel_MsgQueue_enqueue(KernelMsgQueue_t q, uint8_t data)
{
	if (q >= KernelMsgQueue_Num) return false;
	if (Kernel_MsgQueue_is_full(q)) return false;
	messageQueue[q].rear = (messageQueue[q].rear + 1) % MSG_QUEUE_SIZE_BYTE;
	messageQueue[q].queue[messageQueue[q].rear] = data;
	return true;
}

bool kernel_MsgQueue_dequeue(KernelMsgQueue_t q, uint8_t* data)
{
	if (q >= KernelMsgQueue_Num) return false;
	if (Kernel_MsgQueue_is_empty(q)) return false;
	messageQueue[q].front = (messageQueue[q].front + 1) % MSG_QUEUE_SIZE_BYTE;
	*data = messageQueue[q].queue[messageQueue[q].front];
	return true;
}