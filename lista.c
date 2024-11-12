// TAD lista de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com lista encadeada dupla não-circular
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

struct lista_t* lista_cria (){
    struct lista_t *f;

    if (!(f = malloc(sizeof(struct lista_t)))){

        return NULL;
    }
    f->prim = NULL;
    f->ult = NULL;
    f->tamanho = 0;

    return f;
}

struct lista_t *lista_destroi (struct lista_t *lst){
    if (! lst){

        return NULL;
    }

    struct item_t *aux;
    struct item_t *atual = lst->prim;

    while (atual != NULL){
        aux = atual->prox;
        free(atual);
        atual = aux;
    }
    free(lst);

    return NULL;
}

int lista_insere (struct lista_t *lst, int item, int pos){
    struct item_t* novo;
    struct item_t* aux;

    if (!lst || !(novo = malloc(sizeof(struct item_t)))){ //lista nula ou malloc deu errado

        return -1;
    }

    novo->valor = item;
    if (lst->tamanho == 0){ //insere como unico item na lista
        novo->prox = NULL;
        novo->ant = NULL;
        lst->prim = novo;
        lst->ult = novo;
        lst->tamanho++;

        return lst->tamanho;
    }
    
    if (pos >= lst->tamanho || pos == -1){
        novo->prox = NULL;
        novo->ant = lst->ult;
        lst->ult->prox = novo;
        lst->ult = novo;
        lst->tamanho++;

        return lst->tamanho;
    }

    if (pos == 0){ //caso especifico para primeiro item da lista (anterior nulo)
        novo->ant = NULL;
        novo->prox = lst->prim;
        lst->prim->ant = novo;
        lst->prim = novo;
        lst->tamanho++;

        return lst->tamanho;
    }

    aux = lst->prim;
    for (int i = 0; i < pos; i++){
        aux = aux->prox;
    }
    novo->prox = aux;
    aux = aux->ant;
    novo->ant = aux;
    aux->prox = novo;
    aux->prox->prox->ant = novo;
    lst->tamanho++;

    return lst->tamanho;
}

int lista_retira (struct lista_t *lst, int *item, int pos){
    struct item_t* aux;

    if (!lst || !item || lst->tamanho == 0 || pos >= lst->tamanho){

        return -1;
    }

    if (pos == 0){
        aux = lst->prim;
        if (lst->tamanho == 1){
            lst->prim = NULL;
            lst->ult = NULL;
        }
        if (lst->tamanho > 1){
            aux->prox->ant = NULL;
            lst->prim = aux->prox;
        }
        *item = aux->valor;
        free(aux);
        aux = NULL;
        lst->tamanho--;

        return lst->tamanho;
    }
    
    if (pos == -1 || pos == lst->tamanho){ //insere no final (prox nulo)
        aux = lst->ult;
        lst->ult = lst->ult->ant;
        lst->ult->prox = NULL;
        *item = aux->valor;
        free(aux);
        aux = NULL;
        lst->tamanho--;

        return lst->tamanho;
    }

    aux = lst->prim;
    for (int i = 0; i < pos; i++){
        aux = aux->prox;
    }
    aux->prox->ant = aux->ant;
    aux->ant->prox = aux->prox;
    *item = aux->valor;
    free(aux);
    aux = NULL;
    lst->tamanho--;

    return lst->tamanho;
}

int lista_consulta (struct lista_t *lst, int *item, int pos){
    struct item_t* aux;

    if (!lst || !item || lst->tamanho == 0 || pos >= lst->tamanho){

        return -1;
    }

    if (pos == -1 || pos == lst->tamanho){
        *item = lst->ult->valor;

        return lst->tamanho;
    }

    aux = lst->prim;
    for (long i = 0; i < pos; i++){
        aux = aux->prox;
    }
    *item = aux->valor;

    return lst->tamanho;
}

int lista_procura (struct lista_t *lst, int valor){
    struct item_t* aux;

    if (!lst || lst->tamanho == 0){

        return -1;
    }

    aux = lst->prim;
    for (int i = 0; i < lst->tamanho; i++){
        if (aux->valor == valor){

            return i;
        }
        aux = aux->prox;
    }

    return -1;
}

int lista_tamanho (struct lista_t *lst){
    if (!lst){

        return -1;
    }

    return lst->tamanho;
}

void lista_imprime (struct lista_t *lst){
    struct item_t* aux;

    if (!lst || lst->tamanho == 0){

        return;
    }

    aux = lst->prim;
    for (int i = 0; i < lst->tamanho; i++){
        if(aux == lst->ult){
            printf("%d", lst->ult->valor); // se for o ultimo item, nao imprime espaço depois do número
            return;
        }
        printf("%d ", aux->valor); // imprime na forma item item item ...
        aux = aux->prox;
    }
}