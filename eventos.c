
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "mundo.h"
#include "eventos.h"

int dist_cartesiana(struct jusho a, struct jusho b){
    return sqrt(((b.x-a.x)*(b.x-a.x))+((b.y-a.y)*(b.y-a.y)));
}

void ordena_vt(struct base_dist v[]){
    long i, j;
    struct base_dist aux;

    for (i = 1; i < N_BASES; i++){
        aux = v[i];
        j = i - 1;

        while (j >= 0 && aux.dist < v[j].dist){
            v[j+1] = v[j];
            j = j - 1;
        }
        v[j + 1] = aux;
    }

}

// EVENTOS ----------------------
void chega_ev(struct world *world, int time, struct hero_base *hb){
    if(!(world->heroes[hb->hero].vida)){
        return;
    }

    struct hero_base *dados_chega;
    int espera;

    if (!(dados_chega = malloc(sizeof(struct hero_base)))){
        return;
    }

    hb->base = hb->base_n;
    world->heroes[hb->hero].Base = hb->base;

    printf("%6d: CHEGA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d/%2d) ", time, hb->hero, hb->base, world->bases[hb->base].present->num, world->bases[hb->base].Lotação);
    
    //se há vagas em B e a fila de espera em B está vazia;
    if ((cjto_card(world->bases[hb->base].present) < world->bases[hb->base].Lotação && !world->bases[hb->base].espera->tamanho) || world->heroes[hb->hero].Patience > (10 * lista_tamanho(world->bases[hb->base].espera))){ 
        espera = 1;
    }
    else{
        espera = 0;
    }
    
    dados_chega->base = hb->base;
    dados_chega->base_n = hb->base_n;
    dados_chega->hero = hb->hero;
    if (espera){
        fprio_insere (world->lef, dados_chega, ESPERA, time);
        printf("\033[32mESPERA\033[0m\n");

        free(hb);
        hb = NULL;
        return;
    }
    fprio_insere (world->lef, dados_chega, DESISTE, time);
    printf("\033[31mDESISTE\033[0m\n");

    free(hb);
    hb = NULL;
    return;  
}

void espera_ev(struct world *world, int time, struct hero_base *hb){
    if(!(world->heroes[hb->hero].vida)){
        return;
    }

    struct hero_base *dados_espera;

    if (!(dados_espera = malloc(sizeof(struct hero_base)))){

        return;
    }
    
    lista_insere(world->bases[hb->base].espera, world->heroes[hb->hero].ID_hero);
    if (lista_tamanho(world->bases[hb->base].espera) > world->bases[hb->base].fila_max){
        world->bases[hb->base].fila_max = lista_tamanho(world->bases[hb->base].espera);
    }
    printf("%6d: ESPERA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d)\n", time, hb->hero, hb->base, lista_tamanho(world->bases[hb->base].espera));
    
    dados_espera->base = hb->base;
    dados_espera->base_n = hb->base_n;
    dados_espera->hero = hb->hero;
    fprio_insere (world->lef, dados_espera, AVISA, time);
    free(hb);
    hb = NULL;

    return;  
}

void desiste_ev(struct world *world, int time, struct hero_base *hb){
    if(!(world->heroes[hb->hero].vida)){
        return;
    }

    struct hero_base *dados_desiste;

    if (!(dados_desiste = malloc(sizeof(struct hero_base)))){

        return;
    }

    printf("%6d: DESIST \033[36mHEROI %2d \033[31mBASE %d\033[0m\n", time, hb->hero, hb->base);

    dados_desiste->base = hb->base;
    dados_desiste->base_n = (rand()%10);
    dados_desiste->hero = hb->hero;
    fprio_insere (world->lef, dados_desiste, VIAJA, time);
    free(hb);
    hb = NULL;

    return;  
}

void avisa_ev(struct world *world, int time, struct hero_base *hb){
    printf("%6d: AVISA \033[36mPORTEIRO \033[31mBASE %d \033[0m(%2d/%2d) FILA [ ",time, hb->base, cjto_card(world->bases[hb->base].present), world->bases[hb->base].Lotação);
    lista_imprime(world->bases[hb->base].espera);
    printf(" ]\n");

    while(lista_tamanho(world->bases[hb->base].espera) && cjto_card(world->bases[hb->base].present) < world->bases[hb->base].Lotação){
        struct hero_base *dados_avisa;
        int *heroi;

        if (!(dados_avisa = malloc(sizeof(struct hero_base))) || !(heroi = malloc(sizeof(int)))){

            return;
        }

        lista_retira(world->bases[hb->base].espera, heroi);
        cjto_insere(world->bases[hb->base].present, *heroi);
        printf("%6d: AVISA \033[36mPORTEIRO \033[31mBASE %d \033[32m ADMITE %2d\033[0m\n", time, hb->base, *heroi);
        dados_avisa->base = hb->base;
        dados_avisa->base_n = hb->base_n;
        dados_avisa->hero = *heroi;
        dados_avisa->missao = hb->missao;
        fprio_insere (world->lef, dados_avisa, ENTRA, time);
        free(heroi);
        heroi = NULL;

    }
    free(hb);
    hb = NULL;

}

void entra_ev(struct world *world, int time, struct hero_base *hb){
    if(!(world->heroes[hb->hero].vida)){
        return;
    }

    struct hero_base *dados_entra;
    int tpd, rnd;

    if (!(dados_entra = malloc(sizeof(struct hero_base)))){

        return;
    }

    rnd = (rand()%20)+1;
    tpd = 15 + (world->heroes[hb->hero].Patience) * rnd;
    printf("%6d: ENTRA \033[36mHEROI %2d \033[31mBASE %d \033[0m(%2d/%2d) SAI %d\n",time, hb->hero, hb->base, cjto_card(world->bases[hb->base].present), world->bases[hb->base].Lotação, time + tpd);

    dados_entra->base = hb->base;
    dados_entra->base_n = hb->base_n;
    dados_entra->hero = hb->hero;
    //printf("TIME + TPD = %d\n", time + tpd);
    fprio_insere (world->lef, dados_entra, SAI, time + tpd);
    free(hb);
    hb = NULL;
}

void sai_ev(struct world *world, int time, struct hero_base *hb){
    if(!(world->heroes[hb->hero].vida)){
        return;
    }

    struct hero_base *dados_sai;
    struct hero_base *dados_aux;
    int heroi;

    if (!(dados_sai = malloc(sizeof(struct hero_base))) || !(dados_aux = malloc(sizeof(struct hero_base)))){

        return;
    }
    //printf("%6d:HEROI %d SAI\n", time, hb->hero);
    heroi = cjto_retira(world->bases[hb->base].present, hb->hero);
    printf("%6d: SAI \033[36mHEROI %2d \033[31mBASE %d \033[0m(%2d/%2d)\n",time, hb->hero, hb->base, heroi, world->bases[hb->base].Lotação);
    
    dados_sai->base = hb->base;
    dados_sai->base_n = (rand()%10);
    dados_sai->hero = hb->hero;
    fprio_insere (world->lef, dados_sai, VIAJA, time);
    dados_aux->base = hb->base;
    dados_aux->base_n = dados_sai->base_n;
    dados_aux->hero = hb->hero;
    fprio_insere (world->lef, dados_aux, AVISA, time);
    free(hb);
    hb = NULL;
}

void viaja_ev(struct world *world, int time, struct hero_base *hb){
    if(!(world->heroes[hb->hero].vida)){
        return;
    }

    struct hero_base *dados_viaja;
    int duracao;
    int dist;

    if (!(dados_viaja = malloc(sizeof(struct hero_base)))){

        return;
    }

    dist = dist_cartesiana(world->bases[hb->base].Local, world->bases[hb->base_n].Local);
    duracao = dist/world->heroes[hb->hero].Speed;
    dados_viaja->base = hb->base;
    dados_viaja->base_n = hb->base_n;
    dados_viaja->hero = hb->hero;
    fprio_insere (world->lef, dados_viaja, CHEGA, time + duracao);

    printf("%6d: VIAJA \033[36mHEROI %2d \033[31mBASE %d -> BASE %d\033[0m DIST %d VEL %d \033[32mCHEGA %d\033[0m\n",time, hb->hero, hb->base, hb->base_n, dist, world->heroes[hb->hero].Speed, time + duracao);
    free(hb);
    hb = NULL;
}

void morre_ev(struct world *world, int time, struct hero_base *hb){
    if(!(world->heroes[hb->hero].vida)){
        return;
    }

    struct hero_base *dados_morre;

    if (!(dados_morre = malloc(sizeof(struct hero_base)))){

        return;
    }
    printf("%6d: \033[35mMORRE \033[36mHEROI %2d \033[33mMISSAO %d\033[0m\n", time, hb->hero, hb->missao);
    cjto_retira(world->bases[hb->base].present, hb->hero);
    world->heroes[hb->hero].vida = 0;
    dados_morre->base = hb->base;
    dados_morre->base_n = hb->base_n;
    dados_morre->hero = hb->hero;
    dados_morre->missao = hb->missao;
    fprio_insere(world->lef, dados_morre, AVISA, time);
    free(hb);
    hb = NULL;
}

//MISSAO----------------------------------------------------------------
void missao_ev(struct world *world, int time, struct hero_base *hb){
    struct cjto_t *uniao_hab;
    struct base_dist vetor[N_BASES];
    int risco;

    

    uniao_hab = cjto_cria(N_HABILIDADES);
    
    world->missoes[hb->missao].tentativas++;
    printf("%6d: \033[33mMISSAO %4d\033[0m TENT %d HAB REQ: [ ", time, hb->missao, world->missoes[hb->missao].tentativas);
    cjto_imprime(world->missoes[hb->missao].HAB_nec);
    printf(" ]\n");
    
    for (int i = 0; i < N_BASES; i++){
        vetor[i].base = i;
        vetor[i].dist = dist_cartesiana(world->bases[i].Local, world->missoes[hb->missao].local);
    }
    ordena_vt(vetor);

    for (int i = 0; i < N_BASES; i++){
        printf(" MISSAO BASE %d: \n", vetor[i].base);
        for (int j = 0; j < N_HEROIS ; j++){
            if (cjto_pertence(world->bases[vetor[i].base].present, j)){
                printf(" MISSAO HEROI %d HAB: { ", j);
                cjto_imprime(world->heroes[j].power);
                printf(" }\n");
                struct cjto_t *aux = cjto_uniao(uniao_hab, world->heroes[j].power);
                uniao_hab = cjto_destroi(uniao_hab);
                uniao_hab = aux;
            }
        }
        
        printf(" MISSAO HAB UNIAO: [ ");
        cjto_imprime(uniao_hab);
        printf(" ]\n");

        if(cjto_contem(uniao_hab, world->missoes[hb->missao].HAB_nec)){
            world->missoes_cumpridas++;
            world->bases[i].missoes++;
            printf("%6d: \033[33mMISSAO %4d \033[32mCUMPRIDA \033[31mBASE %d \033[0mHABS: [ ", time, hb->missao, vetor[i].base);
            cjto_imprime(uniao_hab);
            printf(" ]\n");

            for (int j = 0; j < N_HEROIS ; j++){
                if (cjto_pertence(world->bases[vetor[i].base].present, j)) {
                    risco = world->heroes[j].Patience + world->heroes[j].EXP + 1;
                    risco = world->missoes[hb->missao].perigo / risco;
                    if (risco > rand()%31){
                        struct hero_base *dados_missao;
                        if (!(dados_missao = malloc(sizeof(struct hero_base)))){

                            return;
                        }
                        dados_missao->base = vetor[i].base;
                        dados_missao->base_n = hb->base_n;
                        dados_missao->hero = j;
                        dados_missao->missao = hb->missao;
                        fprio_insere (world->lef, dados_missao, MORRE, time);
                    }
                    else{
                        world->heroes[j].EXP++;
                    }
                }

            }
            uniao_hab = cjto_destroi(uniao_hab);
            free(hb);
            hb = NULL;
            return;
        }
        
    }
    printf("%6d: \033[33mMISSAO %4d \033[31mIMPOSSIVEL\033[0m\n", time, hb->missao);
    struct hero_base *dados_adia;
    if (!(dados_adia = malloc(sizeof(struct hero_base)))){

        return;
    }
    dados_adia->base = hb->base;
    dados_adia->base_n = hb->base_n;
    dados_adia->hero = hb->hero;
    dados_adia->missao = hb->missao;
    fprio_insere (world->lef, dados_adia, MISSAO, time + 24*60);
    free(hb);
    hb = NULL;
    uniao_hab = cjto_destroi(uniao_hab);
    

}
//MISSAO----------------------------------------------------------------


void fim_ev (struct world *world, int time, int eventos, struct hero_base *hb){
    printf("%6d:\033[31m FIM\033[0m\n", time);
    printf("\n--\033[33mHERO STATS\033[0m---------------------------------------------\n");

    for (int i =0; i < N_HEROIS; i++){
        printf("\033[36mHEROI %2d \033[0m", i);
        if (world->heroes[i].vida){
            printf("\033[32mVIVO  \033[0m");
        }
        if (!(world->heroes[i].vida)){
            printf("\033[31mMORTO \033[0m");
        }
        printf("PAC %3d VEL %4d EXP %4d HABS [ ", world->heroes[i].Patience, world->heroes[i].Speed, world->heroes[i].EXP);
        cjto_imprime(world->heroes[i].power);
        printf(" ]\n");
    }

    printf("\n--\033[33mBASE STATS\033[0m---------------------------------------------\n");

    for (int i =0; i < N_BASES; i++){
        world->bases[i].ID_Base = i;
        printf("\033[31mBASE %d \033[0m LOT %2d FILA MAX %2d MISSOES %d\n", i, world->bases[i].Lotação, world->bases[i].fila_max, world->bases[i].missoes);
    }

    printf("\n--\033[33mWORLD STATS\033[0m--------------------------------------------\n");
    printf("EVENTOS TRATADOS: %d\n", eventos);
    printf("MISSOES CUMPRIDAS: %d/%d\n", world->missoes_cumpridas, N_MISSOES);

    free(hb);
    hb = NULL;
    return;
}


