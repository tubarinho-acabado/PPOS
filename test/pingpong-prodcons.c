#include "lib/pplibc.h"
#include "lib/queue.h"
#include "syscall.h"
#include "kernel/time.h"
#include "kernel/semaphore.h"

#define QUANT_PRODUTORES   9
#define QUANT_CONSUMIDORES 2
#define TAM_BUFFER         5
#define SEGUNDOS           3
#define SONO_MAX         300

struct queue_t *buffer;
int s_vaga, s_item, s_buffer, 
    s_cont_prods, s_cont_cons,
    cont_cons, cont_prods;

unsigned int sono() {
    return (unsigned int) randnum() % SONO_MAX;
}

void body_produtor(void *arg) {
    unsigned int parada = time() + SEGUNDOS * 1000;
    while(cont_cons > 0 && parada > time()) {
        task_sleep(sono());
        int *item = mem_alloc(sizeof(int));
        *item = randnum() % 100;
        sem_down(s_vaga);
        sem_down(s_buffer);

        queue_add(buffer, item);
        printk("p%d produziu %d\n", (int) arg, *item);

        sem_up(s_buffer);
        sem_up(s_item);
    }
    sem_down(s_cont_prods);
    cont_prods--;
    sem_up(s_cont_prods);
    task_exit(NOERROR);
}

void body_consumidor(void *arg) {
    unsigned int parada = time() + SEGUNDOS * 1000;
    while(cont_prods > 0) {
        sem_down(s_item);
        sem_down(s_buffer);

        int *item = queue_head(buffer);
        queue_del(buffer, queue_head(buffer));
        printk("\t\t\t\t\tc%d consumiu %d\n", (int) arg, *item);
        mem_free(item);

        sem_up(s_buffer);
        sem_up(s_vaga);
        task_sleep(sono());
    }
    sem_down(s_cont_cons);
    cont_cons--;
    sem_up(s_cont_cons);
    task_exit(NOERROR);
}

void user_main(void *arg) {
    buffer = queue_create();
    struct task_t *prods[QUANT_PRODUTORES];
    struct task_t *cons[QUANT_CONSUMIDORES];

    s_vaga = sem_create(TAM_BUFFER);
    s_item = sem_create(0);
    s_buffer = sem_create(1);
    s_cont_prods = sem_create(1);
    s_cont_cons = sem_create(1);

    cont_cons = QUANT_CONSUMIDORES;
    cont_prods = QUANT_PRODUTORES;

    for(int i = 0; i < QUANT_CONSUMIDORES; i++)
        cons[i] = task_create(NULL, body_consumidor, (void *) (i + 1));

    for(int i = 0; i < QUANT_PRODUTORES; i++)
        prods[i] = task_create(NULL, body_produtor, (void *) (i + 1));

    for(int i = 0; i < QUANT_PRODUTORES; i++)
        task_wait(prods[i]);
    
    for(int i = 0; i < QUANT_CONSUMIDORES; i++)
        task_wait(cons[i]);

    for(int i = 0; i < QUANT_PRODUTORES; i++)
        task_destroy(prods[i]);
    
    for(int i = 0; i < QUANT_CONSUMIDORES; i++)
        task_destroy(cons[i]);

    sem_destroy(s_buffer);
    sem_destroy(s_item);
    sem_destroy(s_vaga);
    sem_destroy(s_cont_prods);
    sem_destroy(s_cont_cons);

    void *item = queue_head(buffer);
    while(item != NULL) {
        queue_del(buffer, item);
        mem_free(item);
        item = queue_item(buffer);
    }
    queue_destroy(buffer);
    task_exit(NOERROR);
}