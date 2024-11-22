
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "mundo.h"

//MUDAR P ARQUIVO SEPARADO DPS
/*int dist_cartesiana(int x, int y, int a, int b){
    return sqrt((a-x));
}*/

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

    return world;
}

void ini_heroi(struct world *world){
    for (int i =0; i < N_HEROIS; i++){
        world->heroes[i].ID_hero = i;
        world->heroes[i].EXP = 0;
        world->heroes[i].Patience = rand()%101;
        world->heroes[i].Speed = (rand()% 4951)+50;
        world->heroes[i].power = cjto_aleat((rand()%3+1), 10);
        
        //printf("ID Hero %d \n", world->heroes[i].ID_hero);
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
        world->bases[i].Local.x = rand()%(N_TAMANHO_MUNDO-1);
        world->bases[i].Local.y = rand()%(N_TAMANHO_MUNDO-1);
        world->bases[i].Lotação = (rand()%8)+3;
        world->bases[i].present = cjto_cria(world->bases[i].Lotação);
        world->bases[i].espera = lista_cria();
        //printf("ID BASE %d ", world->bases[i].ID_Base);
    }
}

void destroi_base(struct world *world){

    for (int i =0; i < N_BASES; i++){
        world->bases[i].present = cjto_destroi(world->bases[i].present);
        world->bases[i].espera = lista_destroi(world->bases[i].espera);
    }
}

struct world *destroi_mundo(struct world *world){
    world->lef = fprio_destroi(world->lef);
    destroi_base(world);
    destroi_heroi(world);
    free(world);
    return NULL;
}

void ini_lef (struct world *world){
    struct hero_base *hb;
     if(!(hb = malloc(sizeof(struct hero_base)))){
        return;
    }
    int time;
    hb->base_n =0;
    hb->base = 0;
    hb->hero = 0;
    for (int i =0; i < N_HEROIS; i++){
        struct hero_base *aux;
        if(!(aux = malloc(sizeof(struct hero_base)))){
            return;
        }
        aux->base = rand()%N_BASES;
        aux->hero = i;
        aux->base_n =aux->base;
        time = rand()%4320;
        fprio_insere(world->lef, aux, CHEGA, time);
    }
    time = T_FIM_DO_MUNDO;
    fprio_insere(world->lef, hb, FIM, time);
}

void ex_ev(struct world *world){
    struct hero_base *data;
    if(!(data = malloc(sizeof(struct hero_base)))){
        return;
    }

    if (world->lef->prim->item == NULL) {
        return;
    }
    while (world->lef->num > 0){
        data = (struct hero_base *) world->lef->prim->item;
        event_trigger(world, data);
        fprio_retira(world->lef);
    }

    return;
}

void event_trigger (struct world *world, struct hero_base *data){
    if (!world || !data || !world->lef || !world->lef->prim){
        return;
    }
    switch (world->lef->prim->tipo)
    {
    case CHEGA:
        //printf("EVENTO NAO ESTA PRONTO");
        chega_ev(world, world->lef->prim->prio, data);
        

        break;

    case ESPERA:
        //printf("EVENTO NAO ESTA PRONTO");
        espera_ev(world, world->lef->prim->prio, data);

        break;

    case DESISTE:
        //printf("EVENTO NAO ESTA PRONTO");
        desiste_ev(world, world->lef->prim->prio, data);

        break;

    case AVISA:
        //printf("EVENTO NAO ESTA PRONTO");
        avisa_ev(world, world->lef->prim->prio, data);

        break;

    case ENTRA:
        entra_ev(world, world->lef->prim->prio, data);
        //printf("%6d: EVENTO ENTRA NAO ESTA PRONTO\n", -1);

        break;

    case SAI:
        sai_ev(world, world->lef->prim->prio, data);
        //printf("%6d: EVENTO SAI NAO ESTA PRONTO\n", -1);

        break;

    case VIAJA:
        //printf("EVENTO NAO ESTA PRONTO");
        viaja_ev(world, world->lef->prim->prio, data);

        break;
    
    case MORRE:
        printf("%6d: EVENTO MORRE NAO ESTA PRONTO\n", -1);

        break;

    case FIM:
        fim(world);
        break;
    default:
        break;
    }
}

void chega_ev(struct world *world, int time, struct hero_base *hb){
    struct hero_base *dados_chega;
    int espera;

    if (!(dados_chega = malloc(sizeof(struct hero_base)))){
        return;
    }

    world->heroes[hb->hero].Base = hb->base_n;

    printf("%6d: CHEGA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d/%2d) ", time, hb->hero, world->heroes[hb->hero].Base, world->bases[hb->base].present->num, world->bases[hb->base].Lotação);
    
    dados_chega->base = hb->base;
    dados_chega->base_n = hb->base_n;
    dados_chega->hero = hb->hero;
    //se há vagas em B e a fila de espera em B está vazia;
    if ((world->bases[hb->base].present->num < world->bases[hb->base].Lotação && !world->bases[hb->base].espera->tamanho) || world->heroes[hb->hero].Patience > 10 * world->bases[hb->base].espera->tamanho){ 
        espera = 1;
    }
    else{
        espera = 0;
    }
    

    if (espera){
        fprio_insere (world->lef, dados_chega, ESPERA, time);
        printf("\033[32mESPERA\033[0m\n");
        return;
    }
    fprio_insere (world->lef, dados_chega, DESISTE, time);
    printf("\033[31mDESISTE\033[0m\n");

    return;  
}

void espera_ev(struct world *world, int time, struct hero_base *hb){
    struct hero_base *dados_espera;

    if (!(dados_espera = malloc(sizeof(struct hero_base)))){

        return;
    }
    
    lista_insere(world->bases[hb->base].espera, world->heroes[hb->hero].ID_hero);
    printf("%6d: ESPERA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d)\n", time, hb->hero, world->heroes[hb->hero].Base, world->bases[hb->base].espera->tamanho);
    
    dados_espera->base = hb->base;
    dados_espera->base_n = hb->base_n;
    dados_espera->hero = hb->hero;
    fprio_insere (world->lef, dados_espera, AVISA, time);

    return;  
}

void desiste_ev(struct world *world, int time, struct hero_base *hb){
    struct hero_base *dados_desiste;

    if (!(dados_desiste = malloc(sizeof(struct hero_base)))){

        return;
    }

    printf("%6d: DESIST \033[36mHEROI %2d \033[31mBASE %d\033[0m\n", time, hb->hero, hb->base);
    hb->base_n = (rand()%10);
    dados_desiste->base = hb->base;
    dados_desiste->base_n = hb->base_n;
    dados_desiste->hero = hb->hero;
    fprio_insere (world->lef, dados_desiste, VIAJA, time);

    return;  
}

void avisa_ev(struct world *world, int time, struct hero_base *hb){
    struct hero_base *dados_avisa;
    int *heroi;

    if (!(dados_avisa = malloc(sizeof(struct hero_base))) || !(heroi = malloc(sizeof(int)))){

        return;
    }

    printf("%6d: AVISA \033[36mPORTEIRO \033[31mBASE %d \033[0m(%2d/%2d) FILA [ ",time, hb->base, world->bases[hb->base].present->num, world->bases[hb->base].Lotação);
    lista_imprime(world->bases[hb->base].espera);
    printf(" ]\n");
    while(world->bases[hb->base].present->num < world->bases[hb->base].Lotação && world->bases[hb->base].espera->tamanho){
        lista_retira(world->bases[hb->base].espera, heroi);
        cjto_insere(world->bases[hb->base].present, *heroi);
        printf("%6d: AVISA \033[36mPORTEIRO \033[31mBASE %d \033[32m ADMITE %2d\033[0m\n", time, hb->base, *heroi);

    }
    dados_avisa->base = hb->base;
    dados_avisa->base_n = hb->base_n;
    dados_avisa->hero = hb->hero;
    fprio_insere (world->lef, dados_avisa, ENTRA, time);
}

void entra_ev(struct world *world, int time, struct hero_base *hb){
    struct hero_base *dados_entra;
    int tpd, rnd;

    if (!(dados_entra = malloc(sizeof(struct hero_base)))){

        return;
    }

    rnd = (rand()%20)+1;
    tpd = 15 + (world->heroes[hb->hero].Patience) * rnd;
    printf("%6d: ENTRA \033[36mHEROI %2d \033[31mBASE %d \033[0m(%2d/%2d) SAI %d\n",time, hb->hero, hb->base, world->bases[hb->base].present->num, world->bases[hb->base].Lotação, time + tpd);

    dados_entra->base = hb->base;
    dados_entra->base_n = hb->base_n;
    dados_entra->hero = hb->hero;
    fprio_insere (world->lef, dados_entra, SAI, (time + tpd));
}

void sai_ev(struct world *world, int time, struct hero_base *hb){
    struct hero_base *dados_sai;
    struct hero_base *dados_aux;

    if (!(dados_sai = malloc(sizeof(struct hero_base))) || !(dados_aux = malloc(sizeof(struct hero_base)))){

        return;
    }
    
    printf("%6d: SAI \033[36mHEROI %2d \033[31mBASE %d \033[0m(%2d/%2d)\n",time, hb->hero, hb->base, cjto_retira(world->bases[hb->base].present, hb->hero), world->bases[hb->base].Lotação);
    
    hb->base_n = (rand()%10);
    dados_sai->base = hb->base;
    dados_sai->base_n = hb->base_n;
    dados_sai->hero = hb->hero;
    fprio_insere (world->lef, dados_sai, VIAJA, time);
    dados_aux->base = hb->base;
    dados_aux->base_n = hb->base_n;
    dados_aux->hero = hb->hero;
    fprio_insere (world->lef, dados_aux, AVISA, time);
}

void viaja_ev(struct world *world, int time, struct hero_base *hb){
    struct hero_base *dados_viaja;
    int duracao;
    int dist;

    if (!(dados_viaja = malloc(sizeof(struct hero_base)))){

        return;
    }

    dist = sqrt(((world->bases[hb->base_n].Local.x - world->bases[hb->base].Local.x)*(world->bases[hb->base_n].Local.x - world->bases[hb->base].Local.x))+((world->bases[hb->base_n].Local.y - world->bases[hb->base].Local.y)*(world->bases[hb->base_n].Local.y - world->bases[hb->base].Local.y)));
    duracao = dist/world->heroes[hb->hero].Speed;
    dados_viaja->base = hb->base;
    dados_viaja->base_n = hb->base_n;
    dados_viaja->hero = hb->hero;
    fprio_insere (world->lef, dados_viaja, CHEGA, time + duracao);

    printf("%6d: VIAJA \033[36mHEROI %2d \033[31mBASE %d -> BASE %d\033[0m DIST %d VEL %d \033[32mCHEGA %d\033[0m\n",time, hb->hero, hb->base, hb->base_n, dist, world->heroes[hb->hero].Speed, time + duracao);
}

void fim (struct world *world){
    printf("BYEEEE");
    destroi_mundo(world);
}
