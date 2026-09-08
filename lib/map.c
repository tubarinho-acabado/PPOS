// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 2.1 -- 07/2026

// Este arquivo PODE/DEVE ser alterado.

// Implementação do TAD Mapa de objetos

// GRR20232327 Gustavo de Oliveira Ferreira
// GRR20232356 Luka Monteiro

#include <stdlib.h>
#include <stdio.h>
#include "map.h"

struct map_t {
    int size, h;
    void **v;
};

struct map_t *map_create(int size) {
    struct map_t *m = malloc(sizeof(struct map_t));
    if(m == NULL) return NULL;
    m->v = malloc(sizeof(void *) * size);
    for(int i = 0; i < size; i++)
        m->v[i] = NULL;
    m->size = size;
    m->h = 0;
    return m;
}

int map_destroy(struct map_t *map) {
    if(map == NULL) return NOERROR;
    free(map->v);
    free(map);
    return NOERROR;
}

int next_empt(struct map_t *map) {
    if(map->v[map->h] == NULL) 
        return map->h;
    int ini = map->h; 
    map->h++;
    while((map->v[map->h] != NULL) && (map->h != ini)) {
        map->h++;
        map->h = map->h % map->size;
    }
    if(map->h == ini)
        return ERROR;
    return map->h;
}

int map_put(struct map_t *map, void *object) {
    int id = next_empt(map);
    if(id == ERROR) return ERROR;
    map->v[id] = object;
    return id;
}

void *map_get(struct map_t *map, int id) {
    if(map == NULL || map->v == NULL || id >= map->size)
        return NULL;
    
    return map->v[id];
}

void *map_del(struct map_t *map, int id) {
    void *object = map_get(map, id);
    if(object == NULL)
        return NULL;
    map->v[id] = NULL;
    return object;
}

int map_items(struct map_t *map) {
    if(map == NULL) return ERROR;
    int items = 0;
    for(int i = 0; i < map->size; i++)
        if(map->v[i] != NULL)
            items++;
    return items;
}

int map_size(struct map_t *map) {
    if(map == NULL) return ERROR;
    return map->size;
}

void map_print(char *name, struct map_t *map) {
    printf("%s: ", name);
    if(map == NULL || map->v == NULL) {
        printf("undefined\n");
        return;
    }
    printf("[ ");
    for(int i = 0; i < map->size; i++) 
        if(map->v[i] == NULL) printf("- ");
        else printf("* ");
    printf("] (%d/%d)\n", map_items(map), map->size);
}