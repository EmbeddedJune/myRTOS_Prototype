#ifndef KERNEL_MSG_H_
#define KERNEL_MSG_H_

#define MSG_QUEUE_SIZE_BYTE 512

typedef enum KernelMsgQueue_t {
	KernelMsgQueue_task0,
	KernelMsgQueue_task1,
	KernelMsgQueue_task2,

	KernelMsgQueue_Num
} KernelMsgQueue_t;

typedef struct KernelCirQueue_t {
	uint32_t front;
	uint32_t rear;
	uint8_t queue[MSG_QUEUE_SIZE_BYTE];
} KernelCirQueue_t;


void Kernel_MsgQueue_init(void);
bool Kernel_MsgQueue_is_empty(KernelMsgQueue_t q);
bool Kernel_MsgQueue_is_full(KernelMsgQueue_t q);
bool kernel_MsgQueue_enqueue(KernelMsgQueue_t q, uint8_t data);
bool kernel_MsgQueue_dequeue(KernelMsgQueue_t q, uint8_t* data);

#endif