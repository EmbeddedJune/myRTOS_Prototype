#include "stdint.h"
#include "Timer.h"
#include "HalTimer.h"
#include "HalInterrupt.h"

extern volatile Timer_t* Timer;

static void interrupt_handler(void);
static uint32_t internal_1ms_counter;

void Hal_timer_init(void)
{
    // 타이머 reset 과정
    Timer->timerxcontrol.bits.TimerEn   = 0;    // 타이머 비활성화
    Timer->timerxcontrol.bits.TimerMode = 0;    // Free-running 모드
    Timer->timerxcontrol.bits.OneShot   = 0;    // Wrapping-mode
    Timer->timerxcontrol.bits.TimerSize = 0;    // 16-bit counter
    Timer->timerxcontrol.bits.TimerPre  = 0;    // prescaler 1:1
    Timer->timerxcontrol.bits.IntEnable = 1;    // 인터럽트 활성화
    Timer->timerxload   = 0;                    // Load값 0으로 초기화
    Timer->timerxvalue  = 0xFFFFFFFF;           // Value값 최댓값으로 초기화

    // 타이머 환경설정 : Periodic mode
    Timer->timerxcontrol.bits.TimerMode = TIMER_PERIODIC;
    Timer->timerxcontrol.bits.TimerSize = TIMER_32BIT_COUNTER;
    Timer->timerxcontrol.bits.OneShot   = 0;
    Timer->timerxcontrol.bits.TimerPre  = 0;
    Timer->timerxcontrol.bits.IntEnable = 1;
 
    // 타이머 인터럽트 주기 설정 : 1ms
    uint32_t interval_1ms = TIMER_1MHZ_INTERVAL / 1000;
    Timer->timerxload = interval_1ms;

    Timer->timerxcontrol.bits.TimerEn = 1;      // 타이머 활성화

    internal_1ms_counter = 0;

    // 타이머 인터럽트 핸들러 할당
    Hal_interrupt_enable(TIMER_INTERRUPT_NUM);
    Hal_interrupt_register_handler(interrupt_handler, TIMER_INTERRUPT_NUM);
}

uint32_t Hal_timer_get_1ms_counter(void)
{
    return internal_1ms_counter;
}

static void interrupt_handler(void)
{
    internal_1ms_counter++;
    Timer->timerxintclr = 1;    // 인터럽트 핸들러 수행 종료 시 인터럽트 클리어!
}