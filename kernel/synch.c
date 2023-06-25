#include "stdint.h"
#include "stdbool.h"
#include "synch.h"

#define DEF_SEM_MAX 8

static int32_t semaphore;
static KernelMutex_t mutex;

void Kernel_sem_init(int32_t init_value)
{
    if (init_value <= 0 || init_value > DEF_SEM_MAX)
        semaphore = DEF_SEM_MAX;
	else
        semaphore = init_value;
}

bool Kernel_sem_test(void)
{
    if (semaphore <= 0) return false;
    semaphore--;
    return true;
	//return (semaphore-- > 0);
}

void Kernel_sem_release(void)
{
    semaphore++;
    if (semaphore >= DEF_SEM_MAX) semaphore = DEF_SEM_MAX;
	//if (++semaphore > DEF_SEM_MAX) semaphore = DEF_SEM_MAX;
}

void Kernel_mutex_init(void)
{
    mutex.owner = 0;
    mutex.lock = false;
}

bool Kernel_mutex_lock(uint32_t owner)
{
    // 이미 잠겨있다면, 이중 lock은 오류입니다.
    if (mutex.lock) return false;

    mutex.owner = owner;
    mutex.lock = true;
    return true;
}

bool Kernel_mutex_unlock(uint32_t owner)
{
    // 소유주가 다르다면 오류입니다.
    if (mutex.owner != owner) return false;
    
    mutex.lock = false;
    return true;
}
