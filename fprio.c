// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Implementação com fila encadeada simples

// A COMPLETAR

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

//------------------------------------------------------------------------------

//verifica se itens sendo inseridos sao copias
//retorna 0 se for uma copia e 1 se não. -1 em erro
int verifica_copia(struct fprio_t *f, struct fpnodo_t *novo) {
    struct fpnodo_t *atual = f->prim;

    while (atual){
        if (novo->item == atual->item){

            return 0;
        }

        atual = atual->prox;
    }

    return 1;
}

//busca nodo anterior a posição onde novo deve ser inserido
//usado para casos genericos (nao lida com casos especiais de fprio_insere)
//retorna o nodo "atual"(nodo anterior a posição onde novo deve ser inserido), ou NULL(falha)
struct fpnodo_t *busca_pos(struct fprio_t *f, struct fpnodo_t *novo) {
    struct fpnodo_t *atual = f->prim;

    while (atual){
        if (!atual->prox || atual->prox->prio > novo->prio){

            return atual;
        }

        atual = atual->prox;
    }

    return NULL;
}

//------------------------------------------------------------------------------

struct fprio_t *fprio_cria (){
    struct fprio_t *f;

    if (!(f = malloc(sizeof(struct fprio_t)))){

        return NULL;
    }

    f->prim = NULL;
    f->num = 0;

    return f;
}

struct fprio_t *fprio_destroi (struct fprio_t *f){
    if (! f){

        return NULL;
    }

    struct fpnodo_t *aux;
    struct fpnodo_t *atual = f->prim;

    while (atual){
        aux = atual->prox;
        free(atual);
        atual = aux;
    }

    free(f);
    return NULL;
}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio){
    struct fpnodo_t* novo;
    struct fpnodo_t* pos;

    if (!f || !item || !(novo = malloc(sizeof(struct fpnodo_t)))){

        return -1;
    }

    novo->item = item;
    novo->prio = prio;
    novo->tipo = tipo;

    if (verifica_copia(f, novo) == 0){
        free(novo->item);
        novo->item = NULL;
        free(novo);
        novo = NULL;

        return -1;
    }

    if (f->num == 0){ //insere como unico item na fila
        novo->prox = NULL;
        f->prim = novo;

        return f->num++;
    }

    if (prio < f->prim->prio){ //insere antes de f->prim
        novo->prox = f->prim;
        f->prim = novo;

        return f->num++;
    }

    pos = busca_pos(f, novo); //nodo anterior a posição onde novo deve ser inserido
    novo->prox = pos->prox;
    pos->prox = novo;

    return f->num++;
}

void *fprio_retira (struct fprio_t *f){
    struct fpnodo_t* aux;
    void *item;

    if (!f || !f->num){ 

        return NULL;
    }

    aux = f->prim;
    f->prim = aux->prox;
    f->num--;
    //-----------------------
    item = aux->item;

    free(aux);
    aux = NULL;
    return item;
}

int fprio_tamanho (struct fprio_t *f){
    if (!f){

        return -1;
    }

    return f->num;
}

void fprio_imprime (struct fprio_t *f){
    struct fpnodo_t* aux;

    if (!f || f->num == 0){

        return;
    }

    aux = f->prim;
    while (aux){
        if (!aux->prox){
            printf("(%d %d)", aux->tipo, aux->prio); // se for o ultimo item, nao imprime espaço depois

            return;
        }
        
        printf("(%d %d) ", aux->tipo, aux->prio); // imprime na forma (tipo prio) (tipo prio)...
        aux = aux->prox;
    }
}