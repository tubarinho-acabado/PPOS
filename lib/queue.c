// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.1 -- 07/2026

// Este arquivo PODE/DEVE ser alterado.

// Implementação do TAD fila genérica
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

struct node_t {
    void *item;
    struct node_t *next;
};

struct queue_t {
    int size;
    struct node_t *head, *iterator;
};

struct queue_t *queue_create() {
    struct queue_t *queue = malloc(sizeof(struct queue_t));
    if(queue == NULL)
        return NULL;
    queue->head = NULL;
    queue->iterator = NULL;
    queue->size = 0;
    return queue;
}

int queue_destroy(struct queue_t *queue) {
    if(queue == NULL)
        return ERROR;
    struct node_t *node = queue->head;
    while(node != NULL) {
        struct node_t *del = node;
        node = node->next;
        free(del);
    }
    free(queue);
    return NOERROR;
}
struct node_t *node_create(void *item) {
    struct node_t *node = malloc(sizeof(struct node_t));
    if(node == NULL)
        return NULL;
    
    node->item = item;
    node->next = NULL;
    return node;
}

int queue_add(struct queue_t *queue, void *item) {
    struct node_t *new = node_create(item);
    if(queue == NULL || new == NULL)
        return ERROR;

    struct node_t *node = queue->head;
    if(node == NULL) {
        queue->head = new;
        queue->iterator = new;
    }
    else {
        while(node->next != NULL)
            node = node->next;
        node->next = new;
    }
     queue->size++;   
    return NOERROR;
}

int queue_del(struct queue_t *queue, void *item) {
    if(queue == NULL)
        return ERROR;
    
    struct node_t *parent = queue->head;
    if(parent == NULL) return ERROR;

    struct node_t *node = parent->next;

    if(parent->item != item) {
        while(node != NULL && node->item != item) {
            parent = parent->next;
            node = node->next;
        }
            
        if(node == NULL)
            return ERROR;
        parent->next = node->next;
    }
    else {
        node = parent;
        queue->head = node->next;
    }

    queue->size--;
    if(queue->iterator == node)
        queue->iterator = node->next;
    
    free(node);
    return NOERROR;
}

bool queue_has(struct queue_t *queue, void *item) {
    if(queue == NULL) return false;

    struct node_t *node = queue->head;
    if(node == NULL) return false;

    while(node != NULL && node->item != item)
        node = node->next;
    
    if(node == NULL) return false;
    return true;
}

int queue_size(struct queue_t *queue) {
    if(queue == NULL) return ERROR;
    return queue->size;
}

void *queue_head(struct queue_t *queue) {
    if(queue == NULL || queue->head == NULL) return NULL;
    queue->iterator = queue->head;
    return queue->head->item;
}

void *queue_next(struct queue_t *queue) {
    if(queue == NULL || queue->iterator == NULL) return NULL;
    queue->iterator = queue->iterator->next; 
    if(queue->iterator == NULL) return NULL;
    return queue->iterator->item;
}

void *queue_item(struct queue_t *queue) {
    if(queue == NULL || queue->iterator == NULL) return NULL;
    return queue->iterator->item;
}

void print_undef(void *arg) {
    printf("undef");
}

void queue_print(char *name, struct queue_t *queue, void(func)(void *)) {
    if(func == NULL)
        func = print_undef;
    if(queue == NULL) {
        printf("%s: undef\n", name);
        return;
    }
    printf("%s: [ ", name);
    struct node_t *node = queue->head;
    while(node != NULL) {
        func(node->item);
        printf(" ");
        node = node->next;
    }
    printf("] (%d items)\n", queue->size);
}