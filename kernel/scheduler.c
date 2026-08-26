// PingPongOS - PingPong Operating System

// Este arquivo PODE/DEVE ser alterado.

// Escalonador de tarefas prontas.

// GRR20232327 Gustavo de Oliveira Ferreira
// GRR20232356 Luka Monteiro

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