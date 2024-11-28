#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "eventos.h"
#include "mundo.h"

struct world *ini_mundo(){
    struct world *world;
    if(!(world = malloc(sizeof(struct world)))){
        return NULL;
    }
    world->lef = fprio_cria();
    world->N_Base = N_BASES;
    world->N_Hero = N_HEROIS;
    world->N_habilidades = N_HABILIDADES;
    world->N_Mission = N_MISSOES;
    world->missoes_cumpridas = 0;

    return world;
}

void ini_heroi(struct world *world){
    for (int i =0; i < N_HEROIS; i++){
        world->heroes[i].ID_hero = i;
        world->heroes[i].EXP = 0;
        world->heroes[i].Patience = rand()%101;
        world->heroes[i].Speed = (rand()% 4951)+50;
        world->heroes[i].power = cjto_aleat((rand()%3+1), 10);
        world->heroes[i].vida = 1;
    }
}

void destroi_heroi(struct world *world){
    for (int i =0; i < N_HEROIS; i++){
        world->heroes[i].power = cjto_destroi(world->heroes[i].power);
    }
}

void ini_base(struct world *world){

    for (int i =0; i < N_BASES; i++){
        world->bases[i].ID_Base = i;
        world->bases[i].Local.x = rand()%(N_TAMANHO_MUNDO);
        world->bases[i].Local.y = rand()%(N_TAMANHO_MUNDO);
        world->bases[i].Lotação = (rand()%8)+3;
        world->bases[i].present = cjto_cria(N_HEROIS);
        world->bases[i].espera = lista_cria();
        world->bases[i].fila_max = 0;
        world->bases[i].missoes = 0;
    }
}

void destroi_base(struct world *world){

    for (int i =0; i < N_BASES; i++){
        world->bases[i].present = cjto_destroi(world->bases[i].present);
        world->bases[i].espera = lista_destroi(world->bases[i].espera);
    }
}


void ini_missoes(struct world *world){
    int time;
    for (int i =0; i < N_MISSOES; i++){
        struct hero_base *aux;
        if(!(aux = malloc(sizeof(struct hero_base)))){
            return;
        }
        world->missoes[i].ID_missao = i;
        world->missoes[i].local.x = rand()%(N_TAMANHO_MUNDO);
        world->missoes[i].local.y = rand()%(N_TAMANHO_MUNDO);
        world->missoes[i].HAB_nec = cjto_aleat((rand()%5+6), 10);
        world->missoes[i].perigo = rand()%101;
        world->missoes[i].tentativas = 0;
        aux->missao = i;
        aux->base = -1;
        aux->base_n = -1;
        aux->hero = -1;
        time = rand()%T_FIM_DO_MUNDO;
        fprio_insere(world->lef, aux, MISSAO, time);

    }
}

void destroi_missoes(struct world *world){
    for (int i =0; i < N_MISSOES; i++){
        world->missoes[i].HAB_nec = cjto_destroi(world->missoes[i].HAB_nec);
    }
}

void ini_lef (struct world *world){
    int time;
    struct hero_base *hb;
     if(!(hb = malloc(sizeof(struct hero_base)))){
        return;
    }

    for (int i =0; i < N_HEROIS; i++){
        struct hero_base *aux;
        if(!(aux = malloc(sizeof(struct hero_base)))){
            return;
        }
        aux->base = rand()%N_BASES;
        aux->hero = i;
        aux->base_n = aux->base;
        aux->missao = -1;
        time = rand()%4320;
        fprio_insere(world->lef, aux, CHEGA, time);
    }
    time = T_FIM_DO_MUNDO;
    hb->base_n =0;
    hb->base = 0;
    hb->hero = 0;
    hb->missao = -1;
    fprio_insere(world->lef, hb, FIM, time);
}

void destroi_lef(struct world *world){
    struct fpnodo_t *atual = world->lef->prim->prox;
    struct hero_base *aux;
    
    
    while (atual){
        aux = (struct hero_base *) atual->item;
        free(aux);
        aux = NULL;
        atual = atual->prox;
    }
    

}

struct world *destroi_mundo(struct world *world){
    destroi_lef(world);
    world->lef = fprio_destroi(world->lef);
    destroi_base(world);
    destroi_heroi(world);
    destroi_missoes(world);
    free(world);
    return NULL;
}

void ex_ev(struct world *world){
    struct hero_base *dados;
    int EVENTOS_TRATADOS = 0;
  
    if (!(dados = malloc(sizeof(struct hero_base))) || world->lef->prim->item == NULL) {
        return;
    }

    while (fprio_tamanho(world->lef) > 0){
        dados = (struct hero_base *) world->lef->prim->item;
        EVENTOS_TRATADOS++;
        switch (world->lef->prim->tipo){
        case CHEGA:
            chega_ev(world, world->lef->prim->prio, dados);
        

            break;

        case ESPERA:
            espera_ev(world, world->lef->prim->prio, dados);

            break;

        case DESISTE:
            desiste_ev(world, world->lef->prim->prio, dados);

            break;

        case AVISA:
        
            avisa_ev(world, world->lef->prim->prio, dados);

            break;

        case ENTRA:
            printf("137: EVENTO MISSAO ENTRA NAO ESTA PRONTO\n");
            entra_ev(world, world->lef->prim->prio, dados);

            break;

        case SAI:
            sai_ev(world, world->lef->prim->prio, dados);

            break;

        case VIAJA:
            viaja_ev(world, world->lef->prim->prio, dados);

            break;
    
        case MORRE:
            //printf("%6d: EVENTO MISSAO MORRE NAO ESTA PRONTO\n", -1);
            morre_ev(world, world->lef->prim->prio, dados);

            break;

        case MISSAO:
            //printf("%6d: EVENTO MISSAO NAO ESTA PRONTO\n", -1);
            missao_ev(world, world->lef->prim->prio, dados);

            break;

        case FIM:
            fim_ev (world, world->lef->prim->prio, EVENTOS_TRATADOS, dados);

            return;
            break;
        default:
        
            return;
            break;
        }
        fprio_retira(world->lef);

        }

    return;
}

