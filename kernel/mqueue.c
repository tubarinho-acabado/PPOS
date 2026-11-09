// PingPongOS - PingPong Operating System

// Este arquivo PODE/DEVE ser alterado.

// Gerência de filas de mensagens

// GRR20232327 Gustavo de Oliveira Ferreira
// GRR20232356 Luka Monteiro

#include <string.h>
#include "lib/pplibc.h"
#include "syscall.h"
#include "task.h"
#include "lib/map.h"
#include "lib/queue.h"
#include "semaphore.h"
#include "mqueue.h"

#define MAX_FILAS 100

struct mqueue_t {
    void *buffer;
    int ini; // indice de onde inserir no buffer

    int sem_buffer, sem_item, sem_vaga;
    int max_msgs, msg_size, msgs;
};

struct map_t *correios; // mapa das mensagens :D

void mqueue_init() {
    correios = map_create(MAX_FILAS);
}

void mqueue_term() {
    map_destroy(correios);
}

int mqueue_create(int max_msgs, int msg_size) {
    struct mqueue_t *fila = mem_alloc(sizeof(struct mqueue_t));
    if(fila == NULL) return ERROR;

    fila->buffer = mem_alloc(max_msgs * msg_size);
    if(fila->buffer == NULL) return ERROR;

    fila->sem_buffer = sem_create(1);
    fila->sem_item = sem_create(0);
    fila->sem_vaga = sem_create(max_msgs);
    if(fila->sem_buffer == ERROR ||
       fila->sem_item   == ERROR ||
       fila->sem_vaga   == ERROR) 
    return ERROR;

    fila->max_msgs = max_msgs;
    fila->msg_size = msg_size;
    fila->msgs = 0;
    fila->ini = 0;

    return map_put(correios, (void *) fila);
}

int mqueue_destroy(int mqueue_id) {
    struct mqueue_t *fila = map_del(correios, mqueue_id);
    if(fila == NULL) return ERROR;

    sem_destroy(fila->sem_buffer);
    sem_destroy(fila->sem_item);
    sem_destroy(fila->sem_vaga);
    mem_free(fila->buffer); 
    mem_free(fila);
    
    return NOERROR;
}

void *ini_da_fila(struct mqueue_t *fila) {
    return fila->buffer + fila->ini * fila->msg_size;
}

void *fim_da_fila(struct mqueue_t *fila) {
    int fim = fila->msgs + fila->ini;
    if(fim >= fila->max_msgs)
        fim = fila->msgs - fila->max_msgs + fila->ini + 1;
    return fila->buffer + fim * fila->msg_size;
}

void atualiza_ini_da_fila(struct mqueue_t *fila) {
    fila->ini++;
    if(fila->ini >= fila->max_msgs)
        fila->ini = 0;
}

int mqueue_send(int mqueue_id, void *msg) {
    struct mqueue_t *fila = map_get(correios, mqueue_id);
    if(fila == NULL) return ERROR;

    if(sem_down(fila->sem_vaga) == ERROR) return ERROR;
    if(sem_down(fila->sem_buffer) == ERROR) return ERROR;

    memcpy(fim_da_fila(fila), msg, fila->msg_size);
    fila->msgs++;

    sem_up(fila->sem_buffer);
    sem_up(fila->sem_item);
    return NOERROR;
}

int mqueue_recv(int mqueue_id, void *msg) {
    struct mqueue_t *fila = map_get(correios, mqueue_id);
    if(fila == NULL) return ERROR;

    if(sem_down(fila->sem_item) == ERROR) return ERROR;
    if(sem_down(fila->sem_buffer) == ERROR) return ERROR;

    memcpy(msg, ini_da_fila(fila), fila->msg_size);
    atualiza_ini_da_fila(fila);
    fila->msgs--;

    sem_up(fila->sem_buffer);
    sem_up(fila->sem_vaga);
    return NOERROR;
}

int mqueue_msgs(int mqueue_id) {
    struct mqueue_t *fila = map_get(correios, mqueue_id);
    if(fila == NULL)
        return ERROR;
    return fila->msgs;
}