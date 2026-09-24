// PingPongOS - PingPong Operating System
// © Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.1 -- 06/2026

// Este arquivo PODE/DEVE ser alterado.

// Descritor de tarefas (TCB - Task Control Block).

// GRR20232327 Gustavo de Oliveira Ferreira
// GRR20232356 Luka Monteiro

#ifndef __PPOS_TCB__
#define __PPOS_TCB__

#include "ctx.h"
#include "lib/queue.h"

// Task Control Block (TCB), infos sobre uma tarefa

#define INI_PRIO 0
#define STACKSIZE 10000

enum status_t {NEW, READY, RUNNING, SUSPENDED, TERMINATED};

struct task_t
{
    int id;                       // identificador da tarefa
    char *name;                   // nome da tarefa
    struct ctx_t context;         // contexto da tarefa
    enum status_t status;         // pronta, executando, ...
    //...                         // demais informações, a completar
    int exit_code;                // codigo de encerramento
    void *stack;                  // pilha da task
    struct task_t *parent;        // task que criou esta task
    struct queue_t *queue;        // fila onde ela esta, ou NULL
    int vg_id;		              // ID da pilha da tarefa no Valgrind

    int prio;                     // prioridade estatica
    int dim_prio;                 // prioridade dinamica

    int quantum;                  // quantum da preepcao por tempo
    bool task_user;               // verdadeiro quando eh uma task de usuario

    unsigned int birth_time;      // tempo do relogio quando a funcao foi criada
    unsigned int cpu_time;        // quantidade total de tempo que esteve na cpu
    unsigned int last_cpu_init;   // ultimo time quando ganhou a cpu
    int cpu_acts;                 // quantas vezes recebeu a cpu
};

int task_switch(struct task_t *task);

#endif
