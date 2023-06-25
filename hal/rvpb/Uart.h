#ifndef HAL_RVPB_UART_H_
#define HAL_RVPB_UART_H_

typedef union UARTDR_t
{
    uint32_t all;
    struct {
        uint32_t DATA:8;        // 7:0
        uint32_t FE:1;          // 8
        uint32_t PE:1;          // 9
        uint32_t BE:1;          // 10
        uint32_t OE:1;          // 11
        uint32_t reserved:20;
    } bits;
} UARTDR_t;     // Data Register

typedef union UARTRSR_t
{
    uint32_t all;
    struct {
        uint32_t FE:1;          // 0
        uint32_t PE:1;          // 1
        uint32_t BE:1;          // 2
        uint32_t OE:1;          // 3
        uint32_t reserved:28;
    } bits;
} UARTRSR_t;     // Receive Status Register 

typedef union UARTFR_t
{
    uint32_t all;
    struct {
        uint32_t CTS:1;     // 0 Clear to send.
        uint32_t DSR:1;     // 1 Data set ready.
        uint32_t DCD:1;     // 2 Data carrier detect.
        uint32_t BUSY:1;    // 3 UART busy flag.
        uint32_t RXFE:1;    // 4 Receive FIFO empty flag.
        uint32_t TXFF:1;    // 5 Transmit FIFO full flag.
        uint32_t RXFF:1;    // 6 Receive FIFO full flag.
        uint32_t TXFE:1;    // 7 Transmit FIFO empty flag.
        uint32_t RE:1;      // 8 
    } bits;
} UARTFR_t;     // Flag Register

typedef union UARTILPR_t
{
    uint32_t all;
    struct {
        uint32_t ILPDVSR:8; // 7:0, 8-bit low-power divisor value.
        uint32_t reserved:24;
    } bits;
} UARTILPR_t;   // Low-Power Counter Register

// Baud rate는 표 3-9 참고하기
typedef union UARTIBRD_t
{
    uint32_t all;
    struct {
        uint32_t BAUDDIVINT:16; // 15:0
        uint32_t reserved:16;
    } bits;
} UARTIBRD_t;   // Integer Baud Rate Register

typedef union UARTFBRD_t
{
    uint32_t all;
    struct {
        uint32_t BAUDDIVFRAC:6; // 5:0
        uint32_t reserved:26;
    } bits;
} UARTFBRD_t;   // Fractional Baud Rate Register

typedef union UARTLCR_H_t
{
    uint32_t all;
    struct {
        uint32_t BRK:1;     // 0, Send break
        uint32_t PEN:1;     // 1, Parity enable
        uint32_t EPS:1;     // 2, Even parity select
        uint32_t STP2:1;    // 3, 2-step bits select, 2개의 stop bit가 마지막에 전송됨.
        uint32_t FEN:1;     // 4, Enable FIFOs
        uint32_t WLEN:2;    // 6:5, WORD length (00(5-bit) ~ 11(8-bits))
        uint32_t SPS:1;     // 7, Stick parity select
        uint32_t reserved:24;
    } bits;
} UARTLCR_H_t;  // Line Control Register

typedef union UARTCR_t
{
    uint32_t all;
    struct {
        uint32_t UARTEN:1;      // 0, UART enable
        uint32_t SIREN:1;       // 1, SIR enable
        uint32_t SIRLP:1;       // 2, SIR low-power IrDA mode
        uint32_t Reserved1:4;   // 6:3
        uint32_t LBE:1;         // 7. Loopback enable
        uint32_t TXE:1;         // 8, Transmit enable
        uint32_t RXE:1;         // 9, Receive enable
        uint32_t DTR:1;         // 10, Data transmit enable
        uint32_t RTS:1;         // 11, Request to send
        uint32_t Out1:1;        // 12
        uint32_t Out2:1;        // 13
        uint32_t RTSEn:1;       // 14, RTS HW flow control enable
        uint32_t CTSEn:1;       // 15, CTS HW flow control enable
        uint32_t reserved2:16;
    } bits;
} UARTCR_t;     // Control Register

typedef union UARTIFLS_t
{
    uint32_t all;
    struct {
        uint32_t TXIFLSEL:3;    // 2:0
        uint32_t RXIFLSEL:3;    // 5:3
        uint32_t reserved:26;
    } bits;
} UARTIFLS_t;   // Interrupt FIFO Level Select Register

typedef union UARTIMSC_t
{
    uint32_t all;
    struct {
        uint32_t RIMIM:1;   // 0, nUARTRI modem interrupt
        uint32_t CTSMIM:1;  // 1, nUARTCTS modem interrupt
        uint32_t DCDMIM:1;  // 2, nUARTDCD modem interrupt
        uint32_t DSRMIM:1;  // 3, nUARTDSR modem interrupt
        uint32_t RXIM:1;    // 4, Receive interrupt
        uint32_t TXIM:1;    // 5, Transmit interrupt
        uint32_t RTIM:1;    // 6, Receive timeout interrupt
        uint32_t FEIM:1;    // 7, Framing error interrupt
        uint32_t PEIM:1;    // 8, Parity error interrupt
        uint32_t BEIM:1;    // 9, Break error interrupt
        uint32_t OEIM:1;    // 10, Overrun error interrupt
        uint32_t reserved:21;
    } bits;
} UARTIMSC_t;   // Interrupt Mask Set/Clear Register

typedef union UARTRIS_t
{
    uint32_t all;
    struct {
        uint32_t RIRMIS:1;  // 0, nUARTRI modem interrupt
        uint32_t CTSRMIS:1; // 1, nUARTCTS modem interrupt
        uint32_t DCDRMIS:1; // 2, nUARTDCD modem interrupt
        uint32_t DSRRMIS:1; // 3, nUARTDSR modem interrupt
        uint32_t RXRIS:1;   // 4, Receive interrupt
        uint32_t TXRIS:1;   // 5, Transmit interrupt
        uint32_t RTRIS:1;   // 6, Receive timeout interrupt
        uint32_t FERIS:1;   // 7, Framing error interrupt
        uint32_t PERIS:1;   // 8, Parity error interrupt
        uint32_t BERIS:1;   // 9, Break error interrupt
        uint32_t OERIS:1;   // 10, Overrun error interrupt
        uint32_t reserved:21;
    } bits;
} UARTRIS_t;    // Raw Interrupt Status Register

typedef union UARTMIS_t
{
    uint32_t all;
    struct {
        uint32_t RIMMIS:1;  // 0
        uint32_t CTSMMIS:1; // 1
        uint32_t DCDMMIS:1; // 2
        uint32_t DSRMMIS:1; // 3
        uint32_t RXMIS:1;   // 4
        uint32_t TXMIS:1;   // 5
        uint32_t RTMIS:1;   // 6
        uint32_t FEMIS:1;   // 7
        uint32_t PEMIS:1;   // 8
        uint32_t BEMIS:1;   // 9
        uint32_t OEMIS:1;   // 10
        uint32_t reserved:21;
    } bits;
} UARTMIS_t;    // Masked Interrupt Status Register

typedef union UARTICR_t
{
    uint32_t all;
    struct {
        uint32_t RIMIC:1;   // 0
        uint32_t CTSMIC:1;  // 1
        uint32_t DCDMIC:1;  // 2
        uint32_t DSRMIC:1;  // 3
        uint32_t RXIC:1;    // 4
        uint32_t TXIC:1;    // 5
        uint32_t RTIC:1;    // 6
        uint32_t FEIC:1;    // 7
        uint32_t PEIC:1;    // 8
        uint32_t BEIC:1;    // 9
        uint32_t OEIC:1;    // 10
        uint32_t reserved:21;
    } bits;
} UARTICR_t;    // Interrupt Clear Register

typedef union UARTDMACR_t
{
    uint32_t all;
    struct {
        uint32_t RXDMAE:1;      // 0, Receive DMA enable.
        uint32_t TXDMAE:1;      // 1, Transmit DMA enable.
        uint32_t DMAONERR:1;    // 2, DMA on Error
        uint32_t reserved:29;
    } bits;
} UARTDMACR_t;      // DMA Control Register


typedef struct PL011_t
{
    UARTDR_t    uartdr;         //0x000
    UARTRSR_t   uartrsr;        //0x004
    uint32_t    reserved0[4];   //0x008 - 0x014
    UARTFR_t    uartfr;         //0x018
    uint32_t    reserved1;      //0x01C
    UARTILPR_t  uartilpr;       //0x020
    UARTIBRD_t  uartibrd;       //0x024
    UARTFBRD_t  uartfbrd;       //0x028
    UARTLCR_H_t uartlcr_h;      //0x02C
    UARTCR_t    uartcr;         //0x030
    UARTIFLS_t  uartifls;       //0x034
    UARTIMSC_t  uartimsc;       //0x038
    UARTRIS_t   uartris;        //0x03C
    UARTMIS_t   uartmis;        //0x040
    UARTICR_t   uarticr;        //0x044
    UARTDMACR_t uartdmacr;      //0x048
} PL011_t;          // Main structure for UART!


#define UART_BASE_ADDRESS0       0x10009000
#define UART_INTERRUPT0          44

#endif