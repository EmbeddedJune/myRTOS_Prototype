#ifndef HAL_RVPB_TIMER_H_
#define HAL_RVPB_TIMER_H_

typedef union TimerXControl_t {
    uint32_t all;
    struct {
        uint32_t OneShot:1;     // 0, 타이머 인터럽트 1회 발생 후 타이머 종료
        uint32_t TimerSize:1;   // 1, 타이머 크기, 16-bit, 32-bit
        uint32_t TimerPre:2;    // 3:2, Prescaler, 1, 16, 256
        uint32_t Reserved0:1;
        uint32_t IntEnable:1;   // 5, 타이머 인터럽트 활성화 여부
        uint32_t TimerMode:1;   // 6, TimerXLoad 레지스터 활용 여부
        uint32_t TimerEn:1;     // 7, 타이머 활성화 여부
        uint32_t Reserved1:24;  
    } bits;
} TimerXControl_t;

typedef union TimerXRIS_t {
    uint32_t all;
    struct {
        uint32_t TimerXRIS:1;
        uint32_t Reserved:31;
    } bits;
} TimerXRIS_t;

typedef union TimerXMIS_t {
    uint32_t all;
    struct {
        uint32_t TimerXMIS:1;
        uint32_t Reserved:31;
    } bits;
} TimerXMIS_t;

typedef struct Timer_t {
    uint32_t        timerxload;
    uint32_t        timerxvalue;
    TimerXControl_t timerxcontrol;
    uint32_t        timerxintclr;
    TimerXRIS_t     timerxris;
    TimerXMIS_t     timerxmis;      
    uint32_t        timerxbgload;   
} Timer_t;


#define TIMER_CPU_BASE      0x10011000
#define TIMER_INTERRUPT_NUM 36
#define TIMER_FREERUNNING   0
#define TIMER_PERIODIC      1
#define TIMER_16BIT_COUNTER 0
#define TIMER_32BIT_COUNTER 1
#define TIMER_1MHZ_INTERVAL (1024 * 1024)

#endif