// PingPongOS - PingPong Operating System

// Este arquivo PODE/DEVE ser alterado.

// Semáforos e spinlocks

#include <stdlib.h>
#include "task.h"
#include "dispatcher.h"
#include "lib/queue.h"
#include "lib/map.h"

#define MAX_SEMAPHORE 32

struct semaphore_t {
    int id, counter, lock;
    struct queue_t *tasks_queue;
};

// um mapa de semáforos 
// kk eu sou muito engraçado 
// tá parei
struct map_t *detran;

void sem_init()
{
    detran = map_create(MAX_SEMAPHORE);
}

void sem_term()
{
    map_destroy(detran);
}

void spin_lock(int *lock) {
    while (__sync_fetch_and_or (lock, 1))
        task_yield();
}

void spin_unlock(int *lock) {
    (*lock) = 0;
}

int sem_create(int value) {
    struct semaphore_t *sem = malloc(sizeof(struct semaphore_t *));
    sem->counter = value;
    sem->lock = 0;
    sem->tasks_queue = queue_create();
    sem->id = map_put(detran, sem);
    return sem->id;
}

int sem_destroy(int sem_id) {
    struct semaphore_t *sem = map_get(detran, sem_id);
    if(sem == NULL)
        return ERROR;
    map_del(detran, sem_id);
    queue_head(sem->tasks_queue);
    while(queue_item(sem->tasks_queue) != NULL) {
        task_awake(queue_item(sem->tasks_queue));
        queue_next(sem->tasks_queue);
    }
    queue_destroy(sem->tasks_queue);
    free(sem);
    return NOERROR;
}

int sem_down(int sem_id) {
    struct semaphore_t *sem = map_get(detran, sem_id);
    int suspende = 0;
    if(sem == NULL)
        return ERROR;
    spin_lock(&(sem->lock));
        sem->counter--;
        if(sem->counter < 0)
            suspende = 1;
    spin_unlock(&(sem->lock));
    if(suspende)
        task_suspend(sem->tasks_queue);
    
    // Verificando se o semaforo ainda existe
    struct semaphore_t *sem_atual = map_get(detran, sem_id);
    if(sem == sem_atual) return NOERROR;
    return ERROR;
}

int sem_up(int sem_id) {
    struct semaphore_t *sem = map_get(detran, sem_id);
    if(sem == NULL)
        return ERROR;
    spin_lock(&(sem->lock));
        sem->counter++;
        if(queue_size(sem->tasks_queue) > 0)
            task_awake(queue_head(sem->tasks_queue));
    spin_unlock(&(sem->lock));
    return NOERROR;
}