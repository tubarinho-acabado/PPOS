// PingPongOS - PingPong Operating System

// Este arquivo PODE/DEVE ser alterado.

// Dispatcher: gerencia os estados das tarefas.

/* 
+------------------------------------+
| Nome: Gustavo de Oliveira Ferreira |
| GRR:  20232327                     |
|                                    |
| Nome: Luka Monteiro                |
| GRR:  20232356                     |
+------------------------------------+
*/

#include <stdlib.h>
#include "task.h"
extern void user_main(void *arg);
void dispatcher_init()
{
}

void dispatcher_term()
{
}

void dispatcher()
{
    void *arg = NULL;
    struct task_t *task_user = task_create("user_main", user_main, arg);
    task_switch(task_user);
    task_destroy(task_user);
}

