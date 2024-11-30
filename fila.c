// TAD fila de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Implementação do TAD - a completar
//
// Implementação com fila encadeada dupla não-circular
#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

struct fila_t* fila_cria (){
    struct fila_t *f;

    if (!(f = malloc(sizeof(struct fila_t)))){

        return NULL;
    }
    f->prim = NULL;
    f->ult = NULL;
    f->tamanho = 0;

    return f;
}

struct fila_t *fila_destroi (struct fila_t *lst){
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

int fila_insere (struct fila_t *lst, int item){
    struct item_t* novo;

    if (!lst || !(novo = malloc(sizeof(struct item_t)))){ //fila nula ou malloc deu errado

        return -1;
    }

    novo->valor = item;
    if (lst->tamanho == 0){ //insere como unico item na fila
        novo->prox = NULL;
        novo->ant = NULL;
        lst->prim = novo;
        lst->ult = novo;
        lst->tamanho++;

        return lst->tamanho;
    }
    
    
    novo->prox = NULL;
    novo->ant = lst->ult;
    lst->ult->prox = novo;
    lst->ult = novo;
    lst->tamanho++;
    return lst->tamanho;
    
}

int fila_retira (struct fila_t *lst, int *item){
    struct item_t* aux;
    int pos = 0;

    if (!lst || !item || lst->tamanho == 0 || pos >= lst->tamanho){

        return -1;
    }

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
    
    

int fila_consulta (struct fila_t *lst, int *item, int pos){
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

int fila_procura (struct fila_t *lst, int valor){
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

int fila_tamanho (struct fila_t *lst){
    if (!lst){

        return -1;
    }

    return lst->tamanho;
}

void fila_imprime (struct fila_t *lst){
    struct item_t* aux;

    if (!lst || lst->tamanho == 0){

        return;
    }

    aux = lst->prim;
    for (int i = 0; i < lst->tamanho; i++){
        if (aux == lst->ult){
            printf("%d", lst->ult->valor); // se for o ultimo item, nao imprime espaço depois do número
            return;
        }
        printf("%d ", aux->valor); // imprime na forma item item item ...
        aux = aux->prox;
    }
}