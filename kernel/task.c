// PingPongOS - PingPong Operating System

// Este arquivo PODE/DEVE ser alterado.

// Gerência básica de tarefas.

// GRR20232327 Gustavo de Oliveira Ferreira
// GRR20232356 Luka Monteiro

#include <stdlib.h>
#include <valgrind/valgrind.h>

#include "lib/pplibc.h"
#include "lib/queue.h"
#include "time.h"
#include "task.h"

struct task_t task_kernel;
struct task_t *task_atual;
struct queue_t *ready_queue;
int ID;

void task_init()
{
    task_kernel.id = 0;
    task_kernel.name = "kernel";
    task_kernel.task_user = 0;
    task_kernel.parent = &task_kernel;
    task_atual = &task_kernel;
    ID = 1;
}

void task_term()
{
    task_exit(NOERROR);
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
    task->task_user = 1;

    // prate das filas
    queue_add(ready_queue, task);
    task->queue = ready_queue;

    //parte da prioridade
    task->prio = INI_PRIO;
    task->dim_prio = INI_PRIO;
    task->birth_time = time();
    task->cpu_time = 0;
    task->cpu_acts = 0;
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
    task->cpu_acts++;
    task->last_cpu_init = time();
    if(ctx_switch(cont, &(task->context)) == ERROR)
        return ERROR;
    return NOERROR;
}

void task_yield() {
    task_atual->status = READY;
    queue_add(ready_queue, task_atual);
    task_atual->queue = ready_queue;
    task_atual->cpu_time += time() - task_atual->last_cpu_init;
    task_switch(&task_kernel);
}

void print_task_contab(struct task_t *task) {
    unsigned fim = time();

    printk("PPOS: task %d (%s), %d ms run, %d ms cpu, %d acts, exit code %d\n",
           task->id,
           task->name,
           fim - task->birth_time,
           task->cpu_time + fim - task->last_cpu_init,
           task->cpu_acts,
           task->exit_code);
}

void task_exit(int exit_code) {
    task_atual->status = TERMINATED;
    task_atual->exit_code = exit_code;
    print_task_contab(task_atual);
    task_switch(&task_kernel);
}
