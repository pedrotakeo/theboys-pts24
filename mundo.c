#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fprio.h"
#include "fila.h"
#include "conjunto.h"
#include "eventos.h"
#include "mundo.h"

struct mundo *ini_mundo(){
    struct mundo *mundo;
    if(!(mundo = malloc(sizeof(struct mundo)))){
        fprintf(stderr, "MUNDO NAO ALOCOU");
        return NULL;
    }
    mundo->lef = fprio_cria();
    mundo->N_base = N_BASES;
    mundo->N_heroi = N_HEROIS;
    mundo->N_habilidades = N_HABILIDADES;
    mundo->N_missao = N_MISSOES;
    mundo->missoes_cumpridas = 0;

    return mundo;
}

void ini_heroi(struct mundo *mundo){
    for (int i =0; i < N_HEROIS; i++){
        mundo->heroes[i].ID_hero = i;
        mundo->heroes[i].EXP = 0;
        mundo->heroes[i].paciencia = rand()%101;
        mundo->heroes[i].velocidade = (rand()% 4951)+50;
        mundo->heroes[i].power = cjto_aleat((rand()%3+1), 10);
        mundo->heroes[i].vida = 1;
    }
}

void destroi_heroi(struct mundo *mundo){
    for (int i =0; i < N_HEROIS; i++){
        mundo->heroes[i].power = cjto_destroi(mundo->heroes[i].power);
    }
}

void ini_base(struct mundo *mundo){

    for (int i =0; i < N_BASES; i++){
        mundo->bases[i].ID_base = i;
        mundo->bases[i].local.x = rand()%(N_TAMANHO_MUNDO);
        mundo->bases[i].local.y = rand()%(N_TAMANHO_MUNDO);
        mundo->bases[i].lotacao = (rand()%8)+3;
        mundo->bases[i].present = cjto_cria(N_HEROIS);
        mundo->bases[i].espera = fila_cria();
        mundo->bases[i].fila_max = 0;
        mundo->bases[i].missoes = 0;
    }
}

void destroi_base(struct mundo *mundo){

    for (int i =0; i < N_BASES; i++){
        mundo->bases[i].present = cjto_destroi(mundo->bases[i].present);
        mundo->bases[i].espera = fila_destroi(mundo->bases[i].espera);
    }
}


void ini_missoes(struct mundo *mundo){
    int tempo;
    for (int i =0; i < N_MISSOES; i++){
        struct hero_base *aux;
        if(!(aux = malloc(sizeof(struct hero_base)))){
            fprintf(stderr, "MISSAO NAO ALOCOU");
            return;
        }
        mundo->missoes[i].ID_missao = i;
        mundo->missoes[i].local.x = rand()%(N_TAMANHO_MUNDO);
        mundo->missoes[i].local.y = rand()%(N_TAMANHO_MUNDO);
        mundo->missoes[i].HAB_nec = cjto_aleat((rand()%5+6), 10);
        mundo->missoes[i].perigo = rand()%101;
        mundo->missoes[i].tentativas = 0;
        aux->missao = i;
        aux->base = -1;
        aux->base_n = -1;
        aux->heroi = -1;
        tempo = rand()%T_FIM_DO_MUNDO;
        fprio_insere(mundo->lef, aux, MISSAO, tempo);

    }
}

void destroi_missoes(struct mundo *mundo){
    for (int i =0; i < N_MISSOES; i++){
        mundo->missoes[i].HAB_nec = cjto_destroi(mundo->missoes[i].HAB_nec);
    }
}

void ini_lef (struct mundo *mundo){
    int tempo;
    struct hero_base *hb;
     if(!(hb = malloc(sizeof(struct hero_base)))){
        fprintf(stderr, "HB INILEF NAO ALOCOU");
        return;
    }

    for (int i =0; i < N_HEROIS; i++){
        struct hero_base *aux;
        if(!(aux = malloc(sizeof(struct hero_base)))){
            fprintf(stderr, "AUX INILEF NAO ALOCOU");
            return;
        }
        aux->base = rand()%N_BASES;
        aux->heroi = i;
        aux->base_n = aux->base;
        aux->missao = 0;
        tempo = rand()%4320;
        fprio_insere(mundo->lef, aux, CHEGA, tempo);
    }
    tempo = T_FIM_DO_MUNDO;
    hb->base_n =0;
    hb->base = 0;
    hb->heroi = 0;
    hb->missao = 0;
    fprio_insere(mundo->lef, hb, FIM, tempo);
}

void destroi_lef(struct mundo *mundo){
    struct fpnodo_t *atual = mundo->lef->prim->prox;

    while(atual){
        struct hero_base *aux;
        aux = (struct hero_base *)atual->item;
        free(aux);
        aux = NULL;
        atual = atual->prox;
    }
}

struct mundo *destroi_mundo(struct mundo *mundo){
    destroi_lef(mundo);
    mundo->lef = fprio_destroi(mundo->lef);
    destroi_base(mundo);
    destroi_heroi(mundo);
    destroi_missoes(mundo);
    free(mundo);
    return NULL;
}

void ex_ev(struct mundo *mundo){
    struct hero_base *dados;
    int eventos_tratados = 0;

    if ( !mundo->lef->prim->item) {
        
        return;
    }

    while (mundo->lef && fprio_tamanho(mundo->lef) > 0){
        dados = (struct hero_base *) mundo->lef->prim->item;
        eventos_tratados++;
        switch (mundo->lef->prim->tipo){
        case CHEGA:
            chega_ev(mundo, mundo->lef->prim->prio, dados);
        

            break;

        case ESPERA:
            espera_ev(mundo, mundo->lef->prim->prio, dados);

            break;

        case DESISTE:
            desiste_ev(mundo, mundo->lef->prim->prio, dados);

            break;

        case AVISA:
        
            avisa_ev(mundo, mundo->lef->prim->prio, dados);

            break;

        case ENTRA:
            entra_ev(mundo, mundo->lef->prim->prio, dados);

            break;

        case SAI:
            sai_ev(mundo, mundo->lef->prim->prio, dados);

            break;

        case VIAJA:
            viaja_ev(mundo, mundo->lef->prim->prio, dados);

            break;
    
        case MORRE:
            morre_ev(mundo, mundo->lef->prim->prio, dados);

            break;

        case MISSAO:
            missao_ev(mundo, mundo->lef->prim->prio, dados);

            break;

        case FIM:
            fim_ev (mundo, mundo->lef->prim->prio, eventos_tratados, dados);

            return;
            break;
        default:
            free(dados);
            dados = NULL;

            return;
            break;
        }
        fprio_retira(mundo->lef);

        }

    return;
}

