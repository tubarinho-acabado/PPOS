// PingPongOS - PingPong Operating System

// Este arquivo PODE/DEVE ser alterado.

// Escalonador de tarefas prontas.

// GRR20232327 Gustavo de Oliveira Ferreira
// GRR20232356 Luka Monteiro

#include "task.h"
#include "scheduler.h"
extern struct task_t *task_atual;

void sched_init()
{
}

void sched_term()
{
}

struct task_t *menor_dim_prio(struct queue_t *ready_queue) {
    struct task_t *menor = queue_head(ready_queue);
    if(menor == NULL)
        return NULL;

    struct task_t *item = queue_next(ready_queue);
    while(item != NULL) {
        if(item->dim_prio < menor->dim_prio)
            menor = item;
        item = queue_next(ready_queue);
    }
    return menor;
}

struct task_t *scheduler(struct queue_t *ready_queue) {
    // if(ready_queue == NULL)
    //     return NULL;
    // return queue_head(ready_queue);

    struct task_t *proxima = menor_dim_prio(ready_queue);
    if(proxima == NULL)
        return NULL;

    struct task_t *item = queue_head(ready_queue);
    while(item != NULL) {
        item->dim_prio -= 1;
        item = queue_next(ready_queue);
    }
    proxima->dim_prio = proxima->prio;
    return proxima;
}

void sched_setprio(struct task_t *task, int prio) {
    if(task == NULL) {
        task_atual->prio = prio;
        task_atual->dim_prio = prio;
    }
    else  {
        task->prio = prio;
        task->dim_prio = prio;
    }  
        
}

int sched_getprio(struct task_t *task) {
    if(task == NULL)
        return task_atual->prio;
    return task->prio;
}