// PingPongOS - PingPong Operating System

// Este arquivo PODE/DEVE ser alterado.

// Escalonador de tarefas prontas.

#include "scheduler.h"

void sched_init()
{
}

void sched_term()
{
}

struct task_t *scheduler(struct queue_t *ready_queue) {
    if(ready_queue == NULL)
        return NULL;
    return queue_head(ready_queue);
}