#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fprio.h"
#include "fila.h"
#include "conjunto.h"
#include "eventos.h"
#include "mundo.h"

//aloca espaço necessario para herois e inicializa dados
//RETORNO: 1 em execucao normal, 0 em erro
int ini_heroi(struct mundo *mundo){
    if (!mundo){

        return 0;
    }

    //para cada heroi arruma dados da struct
    for (int i =0; i < N_HEROIS; i++){
        mundo->herois[i].ID_hero = i;
        mundo->herois[i].EXP = 0;
        mundo->herois[i].paciencia = rand()%101;
        mundo->herois[i].velocidade = rand()%4951 + 50;
        mundo->herois[i].habilidades = cjto_aleat((rand()%3 + 1), 10);
        if (!mundo->herois[i].habilidades){

            return 0;
        }

        mundo->herois[i].vida = 1;
    }
    
    return 1;
}

//destroi dados alocados dinamicamente de cada heroi
void destroi_heroi(struct mundo *mundo){
    if (!mundo){
        return;
    }

    for (int i =0; i < N_HEROIS; i++){
        mundo->herois[i].habilidades = cjto_destroi(mundo->herois[i].habilidades);
    }
}

//aloca espaço necessario para bases e inicializa dados
//RETORNO: 1 em execucao normal, 0 em erro
int ini_base(struct mundo *mundo){
    if (!mundo){

        return 0;
    }

    for (int i =0; i < N_BASES; i++){
        mundo->bases[i].ID_base = i;
        mundo->bases[i].local.x = rand()%(N_TAMANHO_MUNDO);
        mundo->bases[i].local.y = rand()%(N_TAMANHO_MUNDO);
        mundo->bases[i].lotacao = rand()%8 + 3;
        mundo->bases[i].present = cjto_cria(N_HEROIS);
        if (!mundo->bases[i].present){

            return 0;
        }

        mundo->bases[i].espera = fila_cria();
        if (!mundo->bases[i].espera){

            return 0;
        }

        mundo->bases[i].fila_max = 0;
        mundo->bases[i].missoes = 0;
    }

    return 1;
}

//destroi dados alocados dinamicamente de cada heroi
void destroi_base(struct mundo *mundo){
    if (!mundo){

        return;
    }

    for (int i =0; i < N_BASES; i++){
        mundo->bases[i].present = cjto_destroi(mundo->bases[i].present);
        mundo->bases[i].espera = fila_destroi(mundo->bases[i].espera);
    }
}

//inicializa eventos missao_ev na LEF
//RETORNO: 1 em execucao normal, 0 em erro
int ini_missoes(struct mundo *mundo){
    int tempo;

    if (!mundo || !mundo->lef){

        return 0;
    }

    for (int i =0; i < N_MISSOES; i++){
        struct dados *aux;

        if (!(aux = malloc(sizeof(struct dados)))){
            
            return 0;
        }

        mundo->missoes[i].ID_missao = i;
        mundo->missoes[i].local.x = rand()%(N_TAMANHO_MUNDO);
        mundo->missoes[i].local.y = rand()%(N_TAMANHO_MUNDO);
        mundo->missoes[i].HAB_nec = cjto_aleat((rand()%5 + 6), 10);
        if (!mundo->missoes[i].HAB_nec){

            return 0;
        }

        mundo->missoes[i].perigo = rand()%101;
        mundo->missoes[i].tentativas = 0;
        aux->missao = i;
        aux->base_n = VALOR_INVALIDO;
        aux->heroi = VALOR_INVALIDO;
        tempo = rand()%T_FIM_DO_MUNDO;
        fprio_insere(mundo->lef, aux, MISSAO, tempo);
    }

    return 1;
}

//destroi dados dinamicamente alocados do vetor de missoes
void destroi_missoes(struct mundo *mundo){
    if (!mundo){

        return;
    }

    for (int i =0; i < N_MISSOES; i++){
        mundo->missoes[i].HAB_nec = cjto_destroi(mundo->missoes[i].HAB_nec);
    }
}

//inicializa eventos chega_ev na LEF
//RETORNO: 1 em execucao normal, 0 em erro
int ini_lef (struct mundo *mundo){
    struct dados *hb;
    int tempo;

    if (!mundo || !mundo->lef || !(hb = malloc(sizeof(struct dados)))){

        return 0;
    }
    
    for (int i =0; i < N_HEROIS; i++){
        struct dados *aux;
        if (!(aux = malloc(sizeof(struct dados)))){

            return 0;
        }

        aux->heroi = i;
        mundo->herois[aux->heroi].base = rand()%N_BASES;
        aux->base_n = mundo->herois[aux->heroi].base;
        aux->missao = 0;
        tempo = rand()%4320;
        fprio_insere(mundo->lef, aux, CHEGA, tempo);

    }
    tempo = T_FIM_DO_MUNDO;
    hb->base_n = VALOR_INVALIDO;
    hb->heroi = VALOR_INVALIDO;
    hb->missao = VALOR_INVALIDO;
    fprio_insere(mundo->lef, hb, FIM, tempo);
    
    return 1;
}

//destroi dados dinamicamente alocados de nodos da lef
void destroi_itens_lef(struct mundo *mundo){
    struct fpnodo_t *atual;

    if (!mundo || !mundo->lef){

        return;
    }

    atual = mundo->lef->prim->prox;
    while (atual){
        struct dados *aux;
        aux = (struct dados *)atual->item;
        free(aux);
        aux = NULL;
        atual = atual->prox;
    }
}

struct mundo *ini_mundo(){
    struct mundo *mundo;

    if (!(mundo = malloc(sizeof(struct mundo)))){
        return NULL;
    }

    mundo->lef = fprio_cria();
    mundo->N_base = N_BASES;
    mundo->N_heroi = N_HEROIS;
    mundo->N_habilidades = N_HABILIDADES;
    mundo->N_missao = N_MISSOES;
    mundo->missoes_cumpridas = 0;
    mundo->herois_mortos = 0;
    mundo->eventos_tratados = 0;
    //inicialização de vetores e fprio
    if (!ini_heroi(mundo) || !ini_base(mundo) || !ini_missoes(mundo) || !ini_lef(mundo)){
        return NULL;
    }

    return mundo;
}

struct mundo *destroi_mundo(struct mundo *mundo){
    if (!mundo){

        return NULL;
    }

    destroi_itens_lef(mundo);
    mundo->lef = fprio_destroi(mundo->lef);
    destroi_base(mundo);
    destroi_heroi(mundo);
    destroi_missoes(mundo);
    free(mundo);

    return NULL;
}

void ex_ev(struct mundo *mundo){
    struct dados *dados;

    if ( !mundo || !mundo->lef || !mundo->lef->prim->item){
        
        return;
    }

    while (mundo->lef && fprio_tamanho(mundo->lef) > 0){
        dados = (struct dados *) mundo->lef->prim->item;
        mundo->eventos_tratados++;

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
            fim_ev (mundo, mundo->lef->prim->prio, dados);

            return;

            break;

        default:

            free(dados);
            dados = NULL;
            return;

            break;
        }

        fprio_retira(mundo->lef); //retira e destroi item usado
        }

    return;
}