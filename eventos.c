
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fprio.h"
#include "fila.h"
#include "conjunto.h"
#include "mundo.h"
#include "eventos.h"

int dist_cartesiana(struct local_cart a, struct local_cart b){
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
void chega_ev(struct mundo *mundo, int tempo, struct hero_base *hb){
    if(!(mundo->heroes[hb->heroi].vida)){
        free(hb);
        hb = NULL;
        return;
    }

    struct hero_base *dados_chega;
    int espera;

    if (!(dados_chega = malloc(sizeof(struct hero_base)))){
        free(hb);
        hb = NULL;
        return;
    }

    hb->base = hb->base_n;
    mundo->heroes[hb->heroi].base = hb->base;

    printf("%6d: CHEGA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d/%2d) ", tempo, hb->heroi, hb->base, mundo->bases[hb->base].present->num, mundo->bases[hb->base].lotacao);
    
    //se há vagas em B e a fila de espera em B está vazia;
    if ((cjto_card(mundo->bases[hb->base].present) < mundo->bases[hb->base].lotacao && !mundo->bases[hb->base].espera->tamanho) || mundo->heroes[hb->heroi].paciencia > (10 * fila_tamanho(mundo->bases[hb->base].espera))){ 
        espera = 1;
    }
    else{
        espera = 0;
    }
    
    dados_chega->base = hb->base;
    dados_chega->base_n = hb->base_n;
    dados_chega->heroi = hb->heroi;
    if (espera){
        fprio_insere (mundo->lef, dados_chega, ESPERA, tempo);
        printf("\033[32mESPERA\033[0m\n");

        free(hb);
        hb = NULL;
        return;
    }
    fprio_insere (mundo->lef, dados_chega, DESISTE, tempo);
    printf("\033[31mDESISTE\033[0m\n");

    free(hb);
    hb = NULL;
    return;  
}

void espera_ev(struct mundo *mundo, int tempo, struct hero_base *hb){
    if(!(mundo->heroes[hb->heroi].vida)){
        free(hb);
        hb = NULL;
        return;
    }

    struct hero_base *dados_espera;

    if (!(dados_espera = malloc(sizeof(struct hero_base)))){
        free(hb);
        hb = NULL;
        return;
    }
    
    fila_insere(mundo->bases[hb->base].espera, mundo->heroes[hb->heroi].ID_hero);
    if (fila_tamanho(mundo->bases[hb->base].espera) > mundo->bases[hb->base].fila_max){
        mundo->bases[hb->base].fila_max = fila_tamanho(mundo->bases[hb->base].espera);
    }
    printf("%6d: ESPERA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d)\n", tempo, hb->heroi, hb->base, fila_tamanho(mundo->bases[hb->base].espera));
    
    dados_espera->base = hb->base;
    dados_espera->base_n = hb->base_n;
    dados_espera->heroi = hb->heroi;
    fprio_insere (mundo->lef, dados_espera, AVISA, tempo);
    free(hb);
    hb = NULL;

    return;  
}

void desiste_ev(struct mundo *mundo, int tempo, struct hero_base *hb){
    if(!(mundo->heroes[hb->heroi].vida)){
        free(hb);
        hb = NULL;
        return;
    }

    struct hero_base *dados_desiste;

    if (!(dados_desiste = malloc(sizeof(struct hero_base)))){
        free(hb);
        hb = NULL;

        return;
    }

    printf("%6d: DESIST \033[36mHEROI %2d \033[31mBASE %d\033[0m\n", tempo, hb->heroi, hb->base);

    dados_desiste->base = hb->base;
    dados_desiste->base_n = (rand()%10);
    dados_desiste->heroi = hb->heroi;
    fprio_insere (mundo->lef, dados_desiste, VIAJA, tempo);
    free(hb);
    hb = NULL;

    return;  
}

void avisa_ev(struct mundo *mundo, int tempo, struct hero_base *hb){
    printf("%6d: AVISA \033[36mPORTEIRO \033[31mBASE %d \033[0m(%2d/%2d) FILA [ ",tempo, hb->base, cjto_card(mundo->bases[hb->base].present), mundo->bases[hb->base].lotacao);
    fila_imprime(mundo->bases[hb->base].espera);
    printf(" ]\n");

    while(fila_tamanho(mundo->bases[hb->base].espera) && cjto_card(mundo->bases[hb->base].present) < mundo->bases[hb->base].lotacao){
        struct hero_base *dados_avisa;
        int *heroi;

        if (!(dados_avisa = malloc(sizeof(struct hero_base))) || !(heroi = malloc(sizeof(int)))){
            free(hb);
            hb = NULL;
            return;
        }

        fila_retira(mundo->bases[hb->base].espera, heroi);
        cjto_insere(mundo->bases[hb->base].present, *heroi);
        printf("%6d: AVISA \033[36mPORTEIRO \033[31mBASE %d \033[32m ADMITE %2d\033[0m\n", tempo, hb->base, *heroi);
        dados_avisa->base = hb->base;
        dados_avisa->base_n = hb->base_n;
        dados_avisa->heroi = *heroi;
        dados_avisa->missao = hb->missao;
        fprio_insere (mundo->lef, dados_avisa, ENTRA, tempo);
        free(heroi);
        heroi = NULL;

    }
    free(hb);
    hb = NULL;

}

void entra_ev(struct mundo *mundo, int tempo, struct hero_base *hb){
    if(!(mundo->heroes[hb->heroi].vida)){
        free(hb);
        hb = NULL;
        return;
    }

    struct hero_base *dados_entra;
    int tpd, rnd;

    if (!(dados_entra = malloc(sizeof(struct hero_base)))){
        free(hb);
        hb = NULL;
        return;
    }

    rnd = (rand()%20)+1;
    tpd = 15 + (mundo->heroes[hb->heroi].paciencia) * rnd;
    printf("%6d: ENTRA \033[36mHEROI %2d \033[31mBASE %d \033[0m(%2d/%2d) SAI %d\n",tempo, hb->heroi, hb->base, cjto_card(mundo->bases[hb->base].present), mundo->bases[hb->base].lotacao, tempo + tpd);

    dados_entra->base = hb->base;
    dados_entra->base_n = hb->base_n;
    dados_entra->heroi = hb->heroi;
    //printf("TIME + TPD = %d\n", tempo + tpd);
    fprio_insere (mundo->lef, dados_entra, SAI, tempo + tpd);
    free(hb);
    hb = NULL;
}

void sai_ev(struct mundo *mundo, int tempo, struct hero_base *hb){
    if(!(mundo->heroes[hb->heroi].vida)){
        free(hb);
        hb = NULL;
        return;
    }

    struct hero_base *dados_sai;
    struct hero_base *dados_aux;
    int heroi;

    if (!(dados_sai = malloc(sizeof(struct hero_base))) || !(dados_aux = malloc(sizeof(struct hero_base)))){
        free(hb);
        hb = NULL;

        return;
    }
    //printf("%6d:HEROI %d SAI\n", tempo, hb->heroi);
    heroi = cjto_retira(mundo->bases[hb->base].present, hb->heroi);
    printf("%6d: SAI \033[36mHEROI %2d \033[31mBASE %d \033[0m(%2d/%2d)\n",tempo, hb->heroi, hb->base, heroi, mundo->bases[hb->base].lotacao);
    
    dados_sai->base = hb->base;
    dados_sai->base_n = (rand()%10);
    dados_sai->heroi = hb->heroi;
    fprio_insere (mundo->lef, dados_sai, VIAJA, tempo);
    dados_aux->base = hb->base;
    dados_aux->base_n = dados_sai->base_n;
    dados_aux->heroi = hb->heroi;
    fprio_insere (mundo->lef, dados_aux, AVISA, tempo);
    free(hb);
    hb = NULL;
}

void viaja_ev(struct mundo *mundo, int tempo, struct hero_base *hb){
    if(!(mundo->heroes[hb->heroi].vida)){
        free(hb);
        hb = NULL;
        return;
    }

    struct hero_base *dados_viaja;
    int duracao;
    int dist;

    if (!(dados_viaja = malloc(sizeof(struct hero_base)))){
        free(hb);
        hb = NULL;
        return;
    }

    dist = dist_cartesiana(mundo->bases[hb->base].local, mundo->bases[hb->base_n].local);
    duracao = dist/mundo->heroes[hb->heroi].velocidade;
    dados_viaja->base = hb->base;
    dados_viaja->base_n = hb->base_n;
    dados_viaja->heroi = hb->heroi;
    fprio_insere (mundo->lef, dados_viaja, CHEGA, tempo + duracao);

    printf("%6d: VIAJA \033[36mHEROI %2d \033[31mBASE %d -> BASE %d\033[0m DIST %d VEL %d \033[32mCHEGA %d\033[0m\n",tempo, hb->heroi, hb->base, hb->base_n, dist, mundo->heroes[hb->heroi].velocidade, tempo + duracao);
    free(hb);
    hb = NULL;
}

void morre_ev(struct mundo *mundo, int tempo, struct hero_base *hb){
    if(!(mundo->heroes[hb->heroi].vida)){
        free(hb);
        hb = NULL;
        return;
    }

    struct hero_base *dados_morre;

    if (!(dados_morre = malloc(sizeof(struct hero_base)))){
        free(hb);
        hb = NULL;
        return;
    }
    printf("%6d: \033[35mMORRE \033[36mHEROI %2d \033[33mMISSAO %d\033[0m\n", tempo, hb->heroi, hb->missao);
    cjto_retira(mundo->bases[hb->base].present, hb->heroi);
    mundo->heroes[hb->heroi].vida = 0;
    dados_morre->base = hb->base;
    dados_morre->base_n = hb->base_n;
    dados_morre->heroi = hb->heroi;
    dados_morre->missao = hb->missao;
    fprio_insere(mundo->lef, dados_morre, AVISA, tempo);
    free(hb);
    hb = NULL;
}

//MISSAO----------------------------------------------------------------
void missao_ev(struct mundo *mundo, int tempo, struct hero_base *hb){
    struct cjto_t *uniao_hab;
    struct base_dist vetor[N_BASES];
    int risco;
    
    mundo->missoes[hb->missao].tentativas++;
    printf("%6d: \033[33mMISSAO %4d\033[0m TENT %d HAB REQ: [ ", tempo, hb->missao, mundo->missoes[hb->missao].tentativas);
    cjto_imprime(mundo->missoes[hb->missao].HAB_nec);
    printf(" ]\n");
    
    for (int i = 0; i < N_BASES; i++){
        vetor[i].base = i;
        vetor[i].dist = dist_cartesiana(mundo->bases[i].local, mundo->missoes[hb->missao].local);
    }
    ordena_vt(vetor);

    for (int i = 0; i < N_BASES; i++){
        uniao_hab = cjto_cria(N_HABILIDADES);
        for (int j = 0; j < N_HEROIS ; j++){
            if (cjto_pertence(mundo->bases[vetor[i].base].present, j)){
                struct cjto_t *aux;
                aux = cjto_uniao(uniao_hab, mundo->heroes[j].power);
                uniao_hab = cjto_destroi(uniao_hab);
                uniao_hab = aux;
            }
        }

        if(cjto_contem(uniao_hab, mundo->missoes[hb->missao].HAB_nec)){
            mundo->missoes_cumpridas++;
            mundo->bases[i].missoes++;
            printf("%6d: \033[33mMISSAO %4d \033[32mCUMPRIDA \033[31mBASE %d \033[0mHABS: [ ", tempo, hb->missao, vetor[i].base);
            cjto_imprime(uniao_hab);
            printf(" ]\n");

            for (int j = 0; j < N_HEROIS ; j++){
                if (cjto_pertence(mundo->bases[vetor[i].base].present, j)) {
                    risco = mundo->heroes[j].paciencia + mundo->heroes[j].EXP + 1;
                    risco = mundo->missoes[hb->missao].perigo / risco;
                    if (risco > rand()%31){
                        struct hero_base *dados_missao;
                        if (!(dados_missao = malloc(sizeof(struct hero_base)))){
                            free(hb);
                            hb = NULL;
                            return;
                        }
                        dados_missao->base = vetor[i].base;
                        dados_missao->base_n = hb->base_n;
                        dados_missao->heroi = j;
                        dados_missao->missao = hb->missao;
                        fprio_insere (mundo->lef, dados_missao, MORRE, tempo);
                    }
                    else{
                        mundo->heroes[j].EXP++;
                    }
                }

            }
            uniao_hab = cjto_destroi(uniao_hab);
            free(hb);
            hb = NULL;
            return;
        }
        uniao_hab = cjto_destroi(uniao_hab);
    }
    printf("%6d: \033[33mMISSAO %4d \033[31mIMPOSSIVEL\033[0m\n", tempo, hb->missao);
    struct hero_base *dados_adia;
    if (!(dados_adia = malloc(sizeof(struct hero_base)))){
        free(hb);
        hb = NULL;
        return;
    }
    dados_adia->base = hb->base;
    dados_adia->base_n = hb->base_n;
    dados_adia->heroi = hb->heroi;
    dados_adia->missao = hb->missao;
    fprio_insere (mundo->lef, dados_adia, MISSAO, tempo + 24*60);
    free(hb);
    hb = NULL;
    uniao_hab = cjto_destroi(uniao_hab);
    

}
//MISSAO----------------------------------------------------------------


void fim_ev (struct mundo *mundo, int tempo, int eventos, struct hero_base *hb){
    double media_mi, media_tent, taxa_mo;
    int min_tent, max_tent;
    printf("%6d:\033[31m FIM\033[0m\n", tempo);
    printf("\n--\033[33mHERO STATS\033[0m---------------------------------------------\n");

    for (int i =0; i < N_HEROIS; i++){
        printf("\033[36mHEROI %2d \033[0m", i);
        if (mundo->heroes[i].vida){
            printf("\033[32mVIVO  \033[0m");
        }
        if (!(mundo->heroes[i].vida)){
            printf("\033[31mMORTO \033[0m");
        }
        printf("PAC %3d VEL %4d EXP %4d HABS [ ", mundo->heroes[i].paciencia, mundo->heroes[i].velocidade, mundo->heroes[i].EXP);
        cjto_imprime(mundo->heroes[i].power);
        printf(" ]\n");
    }

    printf("\n--\033[33mBASE STATS\033[0m---------------------------------------------\n");

    for (int i =0; i < N_BASES; i++){
        mundo->bases[i].ID_base = i;
        printf("\033[31mBASE %d \033[0m LOT %2d FILA MAX %2d MISSOES %d\n", i, mundo->bases[i].lotacao, mundo->bases[i].fila_max, mundo->bases[i].missoes);
    }

    printf("\n--\033[33mWORLD STATS\033[0m--------------------------------------------\n");
    printf("EVENTOS TRATADOS: %d\n", eventos);
    media_mi = 100 * (mundo->missoes_cumpridas / (double)N_MISSOES);
    printf("MISSOES CUMPRIDAS: %d/%d  (%.1f%%)\n", mundo->missoes_cumpridas, N_MISSOES, media_mi);

    max_tent = 0;
    for (int i = 0; i < N_MISSOES; i++){
        if (mundo->missoes[i].tentativas > max_tent){
            max_tent = mundo->missoes[i].tentativas;
        }
    }
    min_tent = max_tent;
    for (int i = 0; i < N_MISSOES; i++){
        if (mundo->missoes[i].tentativas < min_tent){
            min_tent = mundo->missoes[i].tentativas;
        }
    }
    media_tent = 0;
    for (int i = 0; i < N_MISSOES; i++){
        media_tent = media_tent + mundo->missoes[i].tentativas;
    }
    media_tent = media_tent / (double)N_MISSOES;
    
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min_tent, max_tent, media_tent);

    taxa_mo = 0;
    for (int i = 0; i < N_HEROIS; i++){
        if (!mundo->heroes[i].vida){
            taxa_mo++;
        }
    }
    taxa_mo = 100 * (taxa_mo / (double)N_HEROIS);
    printf("TAXA MORTALIDADE: %.1f%%\n", taxa_mo);
    printf(" _____ ___ __  __ \n");
    printf("|  ___|_ _|  \\/  |\n");
    printf("| |_   | || |\\/| |\n");
    printf("|  _|  | || |  | |\n");
    printf("|_|   |___|_|  |_|\n");
    printf("\n");

    free(hb);
    hb = NULL;
    return;
}


