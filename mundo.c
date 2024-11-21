
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
        world->heroes[i].power = cjto_aleat(rand()%3, 3);
        printf("ID Hero %d ", world->heroes[i].ID_hero);
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
        printf("ID BASE %d ", world->bases[i].ID_Base);
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
    for (int i =0; i < N_HEROIS; i++){
        hb->base = rand()%N_BASES;
        hb->hero = i;
        time = rand()%4320;
        fprio_insere(world->lef, hb, CHEGA, time);
    }
    time = 525600;
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
    while (world->lef->prim != NULL){
        data = (struct hero_base *) world->lef->prim->item;
        event_trigger(world, data);
        free(data);
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
        printf("EVENTO ENTRA NAO ESTA PRONTO");

        break;

    case SAI:
        printf("EVENTO SAI NAO ESTA PRONTO");

        break;

    case VIAJA:
        //printf("EVENTO NAO ESTA PRONTO");
        viaja_ev(world, world->lef->prim->prio, data);

        break;
    
    case MORRE:
        printf("EVENTO MORRE NAO ESTA PRONTO");

        break;

    case FIM:
        destroi_mundo(world);

        break;
    default:
        break;
    }
}

void chega_ev(struct world *world, int time, struct hero_base *hb){
    bool espera = true;

    world->heroes[hb->hero].Base = hb->base;

    printf("%6d: CHEGA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d/%2d) ", time, hb->hero, hb->base, world->bases[hb->base].present->num, world->bases[hb->base].Lotação);

    //se há vagas em B e a fila de espera em B está vazia:
    if (world->bases[hb->base].present->num < world->bases[hb->base].Lotação && !world->bases[hb->base].espera->tamanho){ 
        espera = true;
    }
    else{
        espera = world->heroes[hb->hero].Patience > 10 * world->bases[hb->base].espera->tamanho;
    }
   

    if (espera){
        fprio_insere (world->lef, &hb, ESPERA, time);
        printf("\033[32mESPERA\033[0m\n");

        return;
    }
    fprio_insere (world->lef, &hb, DESISTE, time);
    printf("\033[31mDESISTE\033[0m\n");

    return;  
}

void espera_ev(struct world *world, int time, struct hero_base *hb){
    printf("%6d: ESPERA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d)\n", time, hb->hero, hb->base, world->bases[hb->base].espera->tamanho);
    lista_insere(world->bases[hb->base].espera, world->heroes[hb->hero].ID_hero);
    fprio_insere (world->lef, &hb, AVISA, time);

    return;  
}

void desiste_ev(struct world *world, int time, struct hero_base *hb){
    printf("%6d: DESIST \033[36mHEROI %2d \033[31mBASE %d\033[0m\n", time, hb->hero, hb->base);
    hb->base_n = (rand()%10)-1;
    fprio_insere (world->lef, &hb, VIAJA, time);

    return;  
}

void avisa_ev(struct world *world, int time, struct hero_base *hb){
    int heroi;

    printf("%6d: AVISA \033[36mPORTEIRO \033[31mBASE %d \033[0m(%2d/%2d) FILA [ ",time, hb->base, world->bases[hb->base].espera->tamanho, world->bases[hb->base].Lotação);
    lista_imprime(world->bases[hb->base].espera);
    printf(" ]\n");
    while(world->bases[hb->base].present->num < world->bases[hb->base].Lotação && world->bases[hb->base].espera->tamanho){
        lista_retira(world->bases[hb->base].espera, &heroi);
        cjto_insere(world->bases[hb->base].present, heroi);
        printf("%6d: AVISA \033[36mPORTEIRO \033[31mBASE %d \033[32m ADMITE %2d\033[0m\n", time, hb->base, heroi);

    }
    fprio_insere (world->lef, &hb, ENTRA, time);
}

void viaja_ev(struct world *world, int time, struct hero_base *hb){
    int duracao;
    int dist;

    dist = sqrt(((world->bases[hb->base_n].Local.x - world->bases[hb->base].Local.x)*(world->bases[hb->base_n].Local.x - world->bases[hb->base].Local.x))+((world->bases[hb->base_n].Local.y - world->bases[hb->base].Local.y)*(world->bases[hb->base_n].Local.y - world->bases[hb->base].Local.y)));
    duracao = dist/world->heroes[hb->hero].Speed;
    fprio_insere (world->lef, &hb, CHEGA, time + duracao);

    printf("%6d: VIAJA \033[36mHEROI %2d \033[31mBASE %d -> BASE %d\033[0m DIST %d VEL %d \033[32mCHEGA %d\033[0m\n",time, hb->hero, hb->base, hb->base_n, dist, world->heroes[hb->hero].Speed, time + duracao);
}

void fim (struct world *world){
    destroi_mundo(world);
}
