
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
void chega_ev(struct mundo *mundo, int tempo, struct dados *hb){
    struct dados *dados_chega;
    struct base *aux_base = &mundo->bases[hb->base_n];
    struct heroi *aux_heroi = &mundo->herois[hb->heroi];
    int espera;

    if (!mundo || !(aux_heroi->vida) || !(dados_chega = malloc(sizeof(struct dados)))){

        free(hb);
        hb = NULL;
        return;
    }

    aux_heroi->base = hb->base_n;

    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ", 
        tempo, 
        hb->heroi, 
        aux_heroi->base, 
        cjto_card(aux_base->present), 
        aux_base->lotacao
    );

    //se há vagas em B e fila de espera em B está vazia;
    if ((cjto_card(aux_base->present) < aux_base->lotacao && !fila_tamanho(aux_base->espera)) || aux_heroi->paciencia > (10 * fila_tamanho(aux_base->espera))){ 
        espera = 1;
    }
    else{
        espera = 0;
    }
    
    dados_chega->base_n = hb->base_n;
    dados_chega->heroi = hb->heroi;
    dados_chega->missao = hb->missao;
    if (espera){
        fprio_insere (mundo->lef, dados_chega, ESPERA, tempo); //adiciona evento ESPERA

        printf("ESPERA\n");

        free(hb);
        hb = NULL;
        return;
    }

    fprio_insere (mundo->lef, dados_chega, DESISTE, tempo); //adiciona evento DESISTE

    printf("DESISTE\n");

    free(hb);
    hb = NULL;
    return;  
}

void espera_ev(struct mundo *mundo, int tempo, struct dados *hb){
    struct dados *dados_espera;
    struct heroi *aux_heroi = &mundo->herois[hb->heroi];
    struct base *aux_base = &mundo->bases[aux_heroi->base];

    if (!mundo || !(aux_heroi->vida) || !(dados_espera = malloc(sizeof(struct dados)))){

        free(hb);
        hb = NULL;
        return;
    }

    fila_insere(aux_base->espera, aux_heroi->ID_hero);
    if (fila_tamanho(aux_base->espera) > aux_base->fila_max){
        aux_base->fila_max = fila_tamanho(aux_base->espera);
    }

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", 
        tempo, 
        hb->heroi, 
        aux_heroi->base, 
        fila_tamanho(aux_base->espera)
    );

    dados_espera->base_n = hb->base_n;
    dados_espera->heroi = hb->heroi;
    dados_espera->missao = hb->missao;
    fprio_insere (mundo->lef, dados_espera, AVISA, tempo); //adiciona evento AVISA

    free(hb);
    hb = NULL;
    return;  
}

void desiste_ev(struct mundo *mundo, int tempo, struct dados *hb){
    struct dados *dados_desiste;
    struct heroi *aux_heroi = &mundo->herois[hb->heroi];

    if (!mundo || !(aux_heroi->vida) || !(dados_desiste = malloc(sizeof(struct dados)))){

        free(hb);
        hb = NULL;
        return;
    }

    printf("%6d: DESIST HEROI %2d BASE %d\n", 
        tempo, 
        hb->heroi, 
        aux_heroi->base
    );

    dados_desiste->base_n = (rand()%10);
    dados_desiste->heroi = hb->heroi;
    dados_desiste->missao = hb->missao;
    fprio_insere (mundo->lef, dados_desiste, VIAJA, tempo); //adiciona evento VIAJA

    free(hb);
    hb = NULL;
    return;  
}

void avisa_ev(struct mundo *mundo, int tempo, struct dados *hb){
    struct heroi *aux_heroi = &mundo->herois[hb->heroi];
    struct base *aux_base = &mundo->bases[aux_heroi->base];

    if (!mundo){

        free(hb);
        hb = NULL;
        return;
    }

    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ",
        tempo, 
        aux_heroi->base, 
        cjto_card(aux_base->present), 
        aux_base->lotacao
    );
    fila_imprime(aux_base->espera);
    printf(" ]\n");

    while (fila_tamanho(aux_base->espera) && cjto_card(aux_base->present) < aux_base->lotacao){
        struct dados *dados_avisa;
        int *heroi;

        if (!(dados_avisa = malloc(sizeof(struct dados))) || !(heroi = malloc(sizeof(int)))){

            free(hb);
            hb = NULL;
            return;
        }

        fila_retira(aux_base->espera, heroi);
        cjto_insere(aux_base->present, *heroi);

        printf("%6d: AVISA PORTEIRO BASE %d  ADMITE %2d\n", 
            tempo, 
            aux_heroi->base, 
            *heroi
        );

        dados_avisa->base_n = hb->base_n;
        dados_avisa->heroi = *heroi;
        dados_avisa->missao = hb->missao;
        fprio_insere (mundo->lef, dados_avisa, ENTRA, tempo); //adiciona evento ENTRA

        free(heroi);
        heroi = NULL;

    }

    free(hb);
    hb = NULL;
}

void entra_ev(struct mundo *mundo, int tempo, struct dados *hb){
    struct dados *dados_entra;
    struct heroi *aux_heroi = &mundo->herois[hb->heroi];
    struct base *aux_base = &mundo->bases[aux_heroi->base];

    int tpd, rnd;

    if (!mundo || !aux_heroi->vida || !(dados_entra = malloc(sizeof(struct dados)))){

        free(hb);
        hb = NULL;
        return;
    }

    rnd = (rand()%20)+1;
    tpd = 15 + (mundo->herois[hb->heroi].paciencia) * rnd;

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n",
        tempo, 
        hb->heroi, 
        aux_heroi->base, 
        cjto_card(aux_base->present), 
        aux_base->lotacao, 
        tempo + tpd
    );

    dados_entra->base_n = hb->base_n;
    dados_entra->heroi = hb->heroi;
    dados_entra->missao = hb->missao;
    fprio_insere (mundo->lef, dados_entra, SAI, tempo + tpd); //adiciona evento SAI

    free(hb);
    hb = NULL;
}

void sai_ev(struct mundo *mundo, int tempo, struct dados *hb){
    struct dados *dados_sai;
    struct dados *dados_aux;
    struct heroi *aux_heroi = &mundo->herois[hb->heroi];
    struct base *aux_base = &mundo->bases[aux_heroi->base];
    int heroi;

    if (!mundo ||!(aux_heroi->vida) || !(dados_sai = malloc(sizeof(struct dados))) || !(dados_aux = malloc(sizeof(struct dados)))){
        
        free(hb);
        hb = NULL;
        return;
    }

    heroi = cjto_retira(aux_base->present, hb->heroi);

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n",
        tempo, 
        hb->heroi, 
        aux_heroi->base, 
        heroi, 
        aux_base->lotacao
    );

    dados_sai->base_n = (rand()%10);
    dados_sai->heroi = hb->heroi;
    dados_sai->missao = hb->missao;
    fprio_insere (mundo->lef, dados_sai, VIAJA, tempo); //adiciona evento VIAJA

    dados_aux->base_n = dados_sai->base_n;
    dados_aux->heroi = hb->heroi;
    dados_aux->missao = hb->missao;
    fprio_insere (mundo->lef, dados_aux, AVISA, tempo); //adiciona evento AVISA

    free(hb);
    hb = NULL;
}

void viaja_ev(struct mundo *mundo, int tempo, struct dados *hb){
    struct dados *dados_viaja;
    struct heroi *aux_heroi = &mundo->herois[hb->heroi];
    struct base *aux_base = &mundo->bases[aux_heroi->base];
    int duracao;
    int dist;

    if (!mundo ||!(aux_heroi->vida) || !(dados_viaja = malloc(sizeof(struct dados)))){

        free(hb);
        hb = NULL;
        return;
    }

    dist = dist_cartesiana(aux_base->local, mundo->bases[hb->base_n].local);
    duracao = dist/aux_heroi->velocidade;
    dados_viaja->base_n = hb->base_n;
    dados_viaja->heroi = hb->heroi;
    dados_viaja->missao = hb->missao;
    fprio_insere (mundo->lef, dados_viaja, CHEGA, tempo + duracao); //adiciona evento CHEGA

    printf("%6d: VIAJA HEROI %2d BASE %d -> BASE %d DIST %d VEL %d CHEGA %d\n",
        tempo, 
        hb->heroi, 
        aux_heroi->base, 
        hb->base_n, 
        dist, 
        aux_heroi->velocidade, 
        tempo + duracao
    );

    free(hb);
    hb = NULL;
}

void morre_ev(struct mundo *mundo, int tempo, struct dados *hb){
    struct dados *dados_morre;
    struct heroi *aux_heroi = &mundo->herois[hb->heroi];
    struct base *aux_base = &mundo->bases[aux_heroi->base];

    if (!mundo || !(aux_heroi->vida) || !(dados_morre = malloc(sizeof(struct dados)))){

        free(hb);
        hb = NULL;
        return;
    }

    printf("%6d: MORRE HEROI %2d MISSAO %d\n", 
        tempo,
        hb->heroi, 
        hb->missao
    );

    cjto_retira(aux_base->present, hb->heroi);
    aux_heroi->vida = 0;
    dados_morre->base_n = hb->base_n;
    dados_morre->heroi = hb->heroi;
    dados_morre->missao = hb->missao;
    fprio_insere(mundo->lef, dados_morre, AVISA, tempo); //adiciona evento AVISA

    free(hb);
    hb = NULL;
}

//MISSAO----------------------------------------------------------------
void missao_ev(struct mundo *mundo, int tempo, struct dados *hb){
    struct cjto_t *uniao_hab;
    struct base_dist vetor[N_BASES];
    struct base *aux_base;
    struct heroi *aux_heroi;
    struct missao *aux_missao = &mundo->missoes[hb->missao];
    int risco;

    if (!mundo){

        free(hb);
        hb = NULL;
        return;
    }
    
    aux_missao->tentativas++;

    printf("%6d: MISSAO %4d TENT %d HAB REQ: [ ", 
        tempo, 
        hb->missao, 
        aux_missao->tentativas
    );
    cjto_imprime(aux_missao->HAB_nec);
    printf(" ]\n");

    for (int i = 0; i < N_BASES; i++){
        vetor[i].base = i;
        vetor[i].dist = dist_cartesiana(mundo->bases[i].local, aux_missao->local);
    }
    ordena_vt(vetor);
    for (int i = 0; i < N_BASES; i++){
        aux_base = &mundo->bases[vetor[i].base];
        uniao_hab = cjto_cria(N_HABILIDADES);
        for (int j = 0; j < N_HEROIS ; j++){
            aux_heroi = &mundo->herois[j];
            if (cjto_pertence(aux_base->present, j)){
                struct cjto_t *aux;
                aux = cjto_uniao(uniao_hab, aux_heroi->habilidades);
                uniao_hab = cjto_destroi(uniao_hab);
                uniao_hab = aux;
            }
        }
        if (cjto_contem(uniao_hab, aux_missao->HAB_nec)){
            mundo->missoes_cumpridas++;
            aux_base->missoes++;

            printf("%6d: MISSAO %4d CUMPRIDA BASE %d HABS: [ ", 
                tempo, 
                hb->missao, 
                vetor[i].base
            );
            cjto_imprime(uniao_hab);
            printf(" ]\n");

            for (int j = 0; j < N_HEROIS ; j++){
                aux_heroi = &mundo->herois[j];
                if (cjto_pertence(aux_base->present, j)){
                    risco = aux_heroi->paciencia + aux_heroi->EXP + 1;
                    risco = aux_missao->perigo / risco;
                    if (risco > rand()%31){
                        struct dados *dados_missao;
                        if (!(dados_missao = malloc(sizeof(struct dados)))){

                            free(hb);
                            hb = NULL;
                            return;
                        }

                        dados_missao->base_n = hb->base_n;
                        dados_missao->heroi = j;
                        dados_missao->missao = hb->missao;
                        fprio_insere (mundo->lef, dados_missao, MORRE, tempo); //adiciona evento MORRE
                    }
                    else{
                        aux_heroi->EXP++;
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

    printf("%6d: MISSAO %4d IMPOSSIVEL\n", 
        tempo, 
        hb->missao
    );

    struct dados *dados_adia;
    if (!(dados_adia = malloc(sizeof(struct dados)))){

        free(hb);
        hb = NULL;
        return;
    }

    dados_adia->base_n = hb->base_n;
    dados_adia->heroi = hb->heroi;
    dados_adia->missao = hb->missao;
    fprio_insere (mundo->lef, dados_adia, MISSAO, tempo + 24*60); //adiciona (adia) evento MISSAO

    free(hb);
    hb = NULL;
    uniao_hab = cjto_destroi(uniao_hab);
}
//MISSAO----------------------------------------------------------------


void fim_ev (struct mundo *mundo, int tempo, int eventos, struct dados *hb){
    double media_mi, media_tent, taxa_mo;
    int min_tent, max_tent;

    printf("%6d: FIM\n", tempo);

    for (int i =0; i < N_HEROIS; i++){

        printf("HEROI %2d ", i);
        if (mundo->herois[i].vida){
            printf("VIVO  ");
        }

        if (!(mundo->herois[i].vida)){
            printf("MORTO ");
        }

        printf("PAC %3d VEL %4d EXP %4d HABS [ ", mundo->herois[i].paciencia, mundo->herois[i].velocidade, mundo->herois[i].EXP);
        cjto_imprime(mundo->herois[i].habilidades);
        printf(" ]\n");

    }
    for (int i =0; i < N_BASES; i++){

        mundo->bases[i].ID_base = i;
        printf("BASE %d  LOT %2d FILA MAX %2d MISSOES %d\n", 
            i, 
            mundo->bases[i].lotacao, 
            mundo->bases[i].fila_max, 
            mundo->bases[i].missoes
        );

    }

    printf("EVENTOS TRATADOS: %d\n", eventos);

    media_mi = 100 * (mundo->missoes_cumpridas / (double)N_MISSOES);
    printf("MISSOES CUMPRIDAS: %d/%d  (%.1f%%)\n", 
        mundo->missoes_cumpridas, 
        N_MISSOES, 
        media_mi
    );

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
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", 
        min_tent,
        max_tent, 
        media_tent
    );

    taxa_mo = 0;
    for (int i = 0; i < N_HEROIS; i++){
        if (!mundo->herois[i].vida){
            taxa_mo++;
        }

    }
    taxa_mo = 100 * (taxa_mo / (double)N_HEROIS);
    printf("TAXA MORTALIDADE: %.1f%%\n", taxa_mo);

    free(hb);
    hb = NULL;
}


