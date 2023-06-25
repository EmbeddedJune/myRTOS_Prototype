#include "stdint.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "stdio.h"
#include "HalTimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "kernel.h"

static void Hw_init(void);
static void Printf_test(void);
static void Timer_test(void);
static void Kernel_init(void);

void main(void)
{
    Hw_init();

    uint32_t i = 50;
    while (i--) Hal_uart_put_char('N');
    Hal_uart_put_char('\n');
    putstr("Hello from EmbeddedJune!\n");

    Printf_test();
    Timer_test();
    Kernel_init();
}

static void Hw_init(void)
{
    Hal_interrupt_init();
    Hal_uart_init();
    Hal_timer_init();
}

static void Printf_test(void)
{
    char* str = "Printf test string";
    char* nullstr = 0;
    uint32_t i = 50;

    debug_printf("%s\n", "Embeddedjune, printf testing");
    debug_printf("Output string: %s\n", str);
    debug_printf("%s is null pointer, %u is number\n", nullstr, i);
    debug_printf("dec = %u, hex = %x\n", 0xff, 0xff);
    debug_printf("printing zero = %u\n", 0);
}

static void Timer_test(void)
{
    int i = 5;
    while (i--) {
        debug_printf("Current timer count = %u\n", Hal_timer_get_1ms_counter());
        delay(1000);
    }
}

// void User_task0(void)
// {
//     uint32_t local_var = 0;
//     while (true) {
//         debug_printf("User Task #0 --- SP: 0x%x\n", &local_var);
//         while (true) {
//             KernelEventFlag_t evt = Kernel_wait_events(
//                 KernelEventFlag_UartIn | KernelEventFlag_CmdOut);
//             switch (evt) {
//             case KernelEventFlag_UartIn:
//                 debug_printf("\nUART Event Handled by Task 0!\n");
//                 Kernel_send_events(KernelEventFlag_CmdIn);
//                 break;
//             case KernelEventFlag_CmdOut:
//                 debug_printf("\nCMD_OUT Event Handled by Task 0!\n");
//                 break;
//             }
//             delay(1000);
//             Kernel_yield();
//         }
//     }
// }

static uint32_t shared_value = 0;

static void test_critical_section(uint32_t var, uint32_t taskID)
{
    Kernel_lock_sem();

    debug_printf("User Task #%u sending value=%u\n", taskID, var);
    shared_value = var;
    Kernel_yield();
    delay(1000);
    debug_printf("User Task #%u Shared value=%u\n", taskID, shared_value);

    Kernel_unlock_sem();
}

void User_task0(void)
{
    uint32_t local_var = 0;
    debug_printf("User Task #0 --- SP: 0x%x\n", &local_var);

    uint8_t cmd_buf[16];
    uint8_t uart_ch = 0;
    uint32_t cmd_buf_idx = 0;

    while (true) {
        KernelEventFlag_t evt = Kernel_wait_events(
            KernelEventFlag_UartIn | KernelEventFlag_CmdOut);
        switch (evt) {
        case KernelEventFlag_UartIn:
            Kernel_recv_msg(KernelMsgQueue_task0, &uart_ch, 1);
            if (uart_ch == '\r') {
                cmd_buf[cmd_buf_idx] = '\0';
                while (true) {
                    Kernel_send_events(KernelEventFlag_CmdIn);
                    if (Kernel_send_msg(KernelMsgQueue_task1, &cmd_buf_idx, 1) == false) Kernel_yield();
                    else if (Kernel_send_msg(KernelMsgQueue_task1, cmd_buf, cmd_buf_idx) == false) {
                        uint8_t rollback_ch;
                        Kernel_recv_msg(KernelMsgQueue_task1, &rollback_ch, 1);
                        Kernel_yield();
                    }
                    else break;
                }
                cmd_buf_idx = 0;
            }
            else {
                cmd_buf[cmd_buf_idx++] = uart_ch;
                cmd_buf_idx %= 16;
            }
            break;
        case KernelEventFlag_CmdOut:
            test_critical_section(5, 0);
            break;
        }
        Kernel_yield();
    }
}

// void User_task1(void)
// {
//     uint32_t local_var = 0;
//     while (true) {
//         debug_printf("User Task #1 --- SP: 0x%x\n", &local_var);
//         while (true) {
//             KernelEventFlag_t evt = Kernel_wait_events(KernelEventFlag_CmdIn);
//             switch (evt) {
//             case KernelEventFlag_CmdIn:
//                 debug_printf("\nCMD Handled by Task 1!\n");
//                 break;
//             }
//             delay(1000);
//             Kernel_yield();
//         }
//     }
// }

void User_task1(void)
{
    uint32_t local_var = 0;
    uint8_t cmd[16] = {0, };
    uint32_t cmd_len = 0;
    debug_printf("User Task #1 --- SP: 0x%x\n", &local_var);

    while (true) {
        KernelEventFlag_t evt = Kernel_wait_events(KernelEventFlag_CmdIn);
        switch (evt) {
        case KernelEventFlag_CmdIn:
            memclr(cmd, 16);
            Kernel_recv_msg(KernelMsgQueue_task1, &cmd_len, 1);
            Kernel_recv_msg(KernelMsgQueue_task1, cmd, cmd_len);
            debug_printf("\n(Task 1) Received cmd: %s\n", cmd);
            break;
        }
        Kernel_yield();
    }
}

void User_task2(void)
{
    uint32_t local_var = 0;
    debug_printf("User Task #2 --- SP: 0x%x\n", &local_var);
    while (true) {
        test_critical_section(3, 2);
        Kernel_yield();
    }
}

static void Kernel_init(void)
{
    uint32_t taskID;
    
    Kernel_task_init();
    Kernel_event_flag_init();
    Kernel_MsgQueue_init();
    Kernel_sem_init(1);

    taskID = Kernel_task_create(User_task0, 0);
    if (taskID == NOT_ENOUGH_TASK_NUM) putstr("Task0 creation FAIL!\n");

    taskID = Kernel_task_create(User_task1, 1);
    if (taskID == NOT_ENOUGH_TASK_NUM) putstr("Task1 creation FAIL!\n");
    
    taskID = Kernel_task_create(User_task2, 2);
    if (taskID == NOT_ENOUGH_TASK_NUM) putstr("Task2 creation FAIL!\n");

    Kernel_start();
}