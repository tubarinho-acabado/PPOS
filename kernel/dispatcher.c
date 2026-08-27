// PingPongOS - PingPong Operating System

// Este arquivo PODE/DEVE ser alterado.

// Dispatcher: gerencia os estados das tarefas.

// GRR20232327 Gustavo de Oliveira Ferreira
// GRR20232356 Luka Monteiro

#include <stdlib.h>
#include "scheduler.h"
#include "task.h"
#include "dispatcher.h"
extern void user_main(void *arg);
extern struct queue_t *ready_queue;
extern struct task_t *task_atual;

void dispatcher_init()
{
    ready_queue = queue_create();
}

void dispatcher_term()
{
    queue_destroy(ready_queue);
}

void dispatcher()
{
    void *arg = NULL;
    
    // cria a tarefa inicial de usuário, que executará user_main()
    struct task_t *task_user = task_create("user_main", user_main, arg);
    task_run(task_user);
    
    // enquanto houver tarefas de usuário
    while(queue_size(ready_queue) > 0) {
        
        // escolhe a próxima tarefa a executar
        struct task_t *proxima = scheduler(ready_queue);
        
        // escalonador escolheu uma tarefa?
        if(proxima != NULL) {
            
            // transfere controle para a próxima tarefa
            task_run(proxima);

            // ao voltar ao dispatcher, trata a tarefa de acordo com seu estado
            // caso o estado da tarefa "próxima" seja
            switch (proxima->status) {
            case TERMINATED:
                queue_del(ready_queue, proxima);
                proxima->queue = NULL;
                task_destroy(proxima);
                break;
            default:
                break;
            }
        }
    }

    // destrói a tarefa inicial do usuário
    task_destroy(task_user);
}

void task_run(struct task_t *task) {
    queue_del(ready_queue, (void *) task);
    task->status = RUNNING;
    task->queue = NULL;
    task_switch(task);
}

void task_suspend(struct queue_t *queue) {
    queue_del(ready_queue, (void *) task_atual);
    task_atual->queue = NULL;
    task_atual->status = SUSPENDED;
    if(queue != NULL) 
        queue_add(queue, (void *) task_atual);
    task_atual->queue = queue;
}

void task_awake(struct task_t *task) {
    if(task->status == SUSPENDED)
        queue_del(task->queue, (void *) task);
    queue_add(ready_queue, task);
    task->queue = ready_queue;
}