#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"
#include "HalInterrupt.h"

#include "kernel.h"

extern volatile PL011_t* Uart;

static void interrupt_handler(void);

void Hal_uart_init(void)
{
    Uart->uartcr.bits.UARTEN = 0;
    Uart->uartcr.bits.TXE = 1;
    Uart->uartcr.bits.RXE = 1;
    Uart->uartcr.bits.UARTEN = 1;

    // Enable Input Interrupt
    Uart->uartimsc.bits.RXIM = 1;
    // Register UART Interrupt Handler
    Hal_interrupt_enable(UART_INTERRUPT0);
    Hal_interrupt_register_handler(interrupt_handler, UART_INTERRUPT0);
}

void Hal_uart_put_char(uint8_t ch)
{
    // Waiting while TX is full
    while (Uart->uartfr.bits.TXFF);
    Uart->uartdr.all = (ch & 0xFF);
}

uint8_t Hal_uart_get_char(void)
{
    uint32_t data;
    // Waiting while RX is empty
    while (Uart->uartfr.bits.RXFE);
    data = Uart->uartdr.all;
    // If there error flag is on
    if (data & 0xFFFFFF00) {
        Uart->uartrsr.all = 0xFF;
        return 0;
    }
    return (uint8_t)(data & 0xFF);
}

// static void interrupt_handler(void)
// {
//     uint8_t ch = Hal_uart_get_char();
//     Hal_uart_put_char(ch);

//     // Kernel_send_events(KernelEventFlag_UartIn | KernelEventFlag_CmdIn);
//     // if (ch == 'X') Kernel_send_events(KernelEventFlag_CmdOut);

//     Kernel_send_msg(KernelMsgQueue_task0, &ch, 1);
//     Kernel_send_events(KernelEventFlag_UartIn);
// }

static void interrupt_handler(void)
{
    uint8_t ch = Hal_uart_get_char();
    if (ch == 'X') {
        Kernel_send_events(KernelEventFlag_CmdOut);
    }
    else {
        Hal_uart_put_char(ch);
        Kernel_send_msg(KernelMsgQueue_task0, &ch, 1);
        Kernel_send_events(KernelEventFlag_UartIn);
    }
}