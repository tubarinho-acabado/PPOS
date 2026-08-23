// PingPongOS - PingPong Operating System
// © Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.1 -- 06/2026

// Este arquivo PODE/DEVE ser alterado.

// Descritor de tarefas (TCB - Task Control Block).

/* 
+------------------------------------+
| Nome: Gustavo de Oliveira Ferreira |
| GRR:  20232327                     |
|                                    |
| Nome: Luka Monteiro                |
| GRR:  20232356                     |
+------------------------------------+
*/

#ifndef __PPOS_TCB__
#define __PPOS_TCB__

#include "ctx.h"

// Task Control Block (TCB), infos sobre uma tarefa

enum status_t {NEW, READY, RUNNING, SUSPENDED, TERMINATED};

struct task_t
{
    int id;         // identificador da tarefa
    char *name;     // nome da tarefa
    struct ctx_t context;  // contexto da tarefa
    enum status_t status;     // pronta, executando, ...
    //...             // demais informações, a completar
    struct task_t *parent;
};

int task_switch(struct task_t *task);

#endif
