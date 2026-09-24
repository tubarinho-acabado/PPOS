// PingPongOS - PingPong Operating System

// Este arquivo PODE/DEVE ser alterado.

// Gerência básica do tempo.

// GRR20232327 Gustavo de Oliveira Ferreira
// GRR20232356 Luka Monteiro

#include "task.h"
#include "time.h"
#include "hardware/cpu.h"

unsigned int clock_time;
extern struct task_t *task_atual;

void time_handle(int time) {
    clock_time++;
    if(task_atual->task_user == 0)
        return;
    task_atual->quantum--;
    if(task_atual->quantum == 0)
        task_yield();
}

void time_init()
{
    clock_time = 0;
    hw_irq_handle(IRQ_TIMER, time_handle);
    hw_timer(TICK, TICK);
}

void time_term()
{
}

unsigned int time()
{
    return clock_time;
}

