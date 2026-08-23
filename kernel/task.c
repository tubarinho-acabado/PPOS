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
#include "task.h"

#define INI_STACK_SIZE 10000
struct task_t task_kernel;
struct task_t *task_atual;
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

    task->name = name;
    task->id = ID;
    ID++;
    task->parent = task_atual;
    void *stack = calloc(INI_STACK_SIZE, sizeof(void));
    if(ctx_create(&(task->context), entry, arg, stack, INI_STACK_SIZE) == ERROR)
        return NULL;
    return task;
}

int task_destroy(struct task_t *task) {
    //if(task->status != TERMINATED)
    //    return ERROR;
    free(task->context.stack);
    free(task);
    return NOERROR;
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