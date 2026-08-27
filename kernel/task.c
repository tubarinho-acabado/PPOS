// PingPongOS - PingPong Operating System

// Este arquivo PODE/DEVE ser alterado.

// Gerência básica de tarefas.

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
#include <valgrind/valgrind.h>

#include "lib/queue.h"
#include "task.h"

struct task_t task_kernel;
struct task_t *task_atual;
struct queue_t *ready_queue;
int ID;

void task_init()
{
    task_kernel.id = 0;
    task_kernel.name = "kernel";
    task_kernel.parent = &task_kernel;
    task_atual = &task_kernel;
    ID = 1;
}

void task_term()
{
}

struct task_t *task_create(char *name, void (*entry)(void *), void *arg) {
    struct task_t *task = malloc(sizeof(struct task_t));
    if(task == NULL) return NULL;

    void *stack = calloc(STACKSIZE, sizeof(void));
    if(ctx_create(&(task->context), entry, arg, stack, STACKSIZE) == ERROR)
        return NULL;

    task->stack = stack;

    // registra a pilha da tarefa no Valgrind
    task->vg_id = VALGRIND_STACK_REGISTER(task->stack, task->stack + STACKSIZE);

    // ajustando os valores iniciais
    task->name = name;
    task->id = ID;
    ID++;
    task->parent = task_atual;
    task->status = READY;

    // prate das filas
    queue_add(ready_queue, task);
    task->queue = ready_queue;

    //parte da prioridade
    task->prio = INI_PRIO;
    task->dim_prio = INI_PRIO;
    return task;
}

int task_destroy(struct task_t *task) {
    //if(task->status != TERMINATED)
    //    return ERROR;
    free(task->stack);

    // dezfaz o registro da pilha no Valgrind
    VALGRIND_STACK_DEREGISTER(task->vg_id);

    free(task);
    return NOERROR;
    VALGRIND_STACK_DEREGISTER(task->vg_id);
}

int task_id(struct task_t *task) {
    if(task == NULL)
        return task_atual->id;
    return task->id;
}

char *task_name(struct task_t *task) {
    if(task == NULL)
        return task_atual->name;
    return task->name;
}

int task_switch(struct task_t *task) {
    if(task == NULL)
        task = task_atual->parent;

    struct ctx_t *cont = &(task_atual->context);
    task_atual = task;
    if(ctx_switch(cont, &(task->context)) == ERROR)
        return ERROR;
    return NOERROR;
}

void task_yield() {
    task_atual->status = READY;
    queue_add(ready_queue, task_atual);
    task_atual->queue = ready_queue;
    task_switch(&task_kernel);
}

void task_exit(int exit_code) {
    task_atual->status = TERMINATED;
    task_switch(&task_kernel);
}