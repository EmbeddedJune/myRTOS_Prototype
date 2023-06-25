# RTOS 프로토타입 제작

RTOS 프로토타입 제작 프로젝트입니다. (기간: 23.6.14 ~ 6.20)
Target: QEMU ARMv7 Cortex-A8 Realview-PB

---
## 디렉토리 설명
- boot/ : 타겟보드 부팅과정에 반드시 필요한 소스파일 및 startup 부트로더가 포함돼있습니다.
- hal/ : HW 독립적인 API 위주의 코드가 포함돼있습니다.
- hal/rvpb : HW 종속적인 코드가 포함돼있습니다. GIC, UART, Timer 등의 HW를 정의했습니다.
- include/ :
    - 표준 C 라이브러리를 링킹 할 수 없기 때문에 (-lgcc) 필요한 부분만 일부 가져와 사용했습니다.
    - 또한, 타겟의 메모리 구조(MemoryMap.h)를 구현했습니다.
- Kernel/ : RTOS 동작에 필요한 코드가 포함돼있습니다. Task, Scheduler, Context switching 등이 구현돼있습니다.
- lib/ :
    - 표준 C 라이브러리를 링킹 할 수 없기 때문에 (-lgcc) 필요한 부분을 일부 구현했습니다.
    - include 디렉토리는 표준 C 라이브러리를 그대로(또는 일부를) 가져왔다면,
    - lib 디렉토리는 표준 C 라이브러리의 같은 이름의 함수를 간단한 버전으로 다시 구현했습니다.

---
## 프로젝트에 대한 설명

[① 툴체인 설정 및 Startup 코드 개발](https://blog.naver.com/leeinje66/223128238302)

[② 스택 초기화 및 부팅하기](https://blog.naver.com/leeinje66/223128682590)

[③ UART](https://blog.naver.com/leeinje66/223128963414)

[④ 인터럽트 컨트롤러](https://blog.naver.com/leeinje66/223129200393)

[⑤ 타이머](https://blog.naver.com/leeinje66/223129565869)

[⑥ 태스크](https://blog.naver.com/leeinje66/223130174151)

[⑦ 통신 (Event, Message Queue)](https://blog.naver.com/leeinje66/223133405610)

[⑧ 동기화 (Semaphore, Mutex)](https://blog.naver.com/leeinje66/223133922583)

---
