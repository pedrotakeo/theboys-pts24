
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "mundo.h"

/*
EVENTOS:
    -Chega - Tipo 1      -Entra - Tipo 5
    -Espera - Tipo 2     -Sai - Tipo 6
    -Desiste - Tipo 3    -Viaja - Tipo 7
    -Avisa - Tipo 4      -Morre - Tipo 8
*/
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
    srand(0);

    for (int i =0; i < N_HEROIS; i++){
        world->heroes[i].ID_hero = i;
        world->heroes[i].EXP = 0;
        world->heroes[i].Patience = rand()%101;
        world->heroes[i].Speed = (rand()% 4951)+50;
        world->heroes[i].power = cjto_aleat(rand()%3, 3);
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
    }
}

void chega_ev(struct world *world, int time, struct hero_base hb){
    bool espera = true;

    world->heroes[hb.hero].Base = hb.base;

    printf("%6d: CHEGA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d/%2d) ", time, hb.hero, hb.base, world->bases[hb.base].present->num, world->bases[hb.base].Lotação);

    //se há vagas em B e a fila de espera em B está vazia:
    if (world->bases[hb.base].present->num < world->bases[hb.base].Lotação && !world->bases[hb.base].espera->tamanho){ 
        espera = true;
    }
    else{
        espera = world->heroes[hb.hero].Patience > 10 * world->bases[hb.base].espera->tamanho;
    }
   

    if (espera){
        fprio_insere (world->lef, &hb, 2, time);
        printf("\033[32mESPERA\033[0m\n");

        return;
    }
    fprio_insere (world->lef, &hb, 3, time);
    printf("\033[31mDESISTE\033[0m\n");

    return;  
}

void espera_ev(struct world *world, int time, struct hero_base hb){
    printf("%6d: ESPERA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d)\n", time, hb.hero, hb.base, world->bases[hb.base].espera->tamanho);
    lista_insere(world->bases[hb.base].espera, world->heroes[hb.hero].ID_hero, -1);
    fprio_insere (world->lef, &hb, 4, time);

    return;  
}

void desiste_ev(struct world *world, int time, struct hero_base hb){
    printf("%6d: DESIST \033[36mHEROI %2d \033[31mBASE %d\033[0m\n", time, hb.hero, hb.base);
    hb.base_n = (rand()%10)-1;
    fprio_insere (world->lef, &hb, 7, time);

    return;  
}

void viaja_ev(struct world *world, int time, struct hero_base hb){
    int duracao;
    int dist;

    dist = sqrt(((world->bases[hb.base_n].Local.x - world->bases[hb.base].Local.x)*(world->bases[hb.base_n].Local.x - world->bases[hb.base].Local.x))+((world->bases[hb.base_n].Local.y - world->bases[hb.base].Local.y)*(world->bases[hb.base_n].Local.y - world->bases[hb.base].Local.y)));
    duracao = dist/world->heroes[hb.hero].Speed;
    fprio_insere (world->lef, &hb, 1, (time + duracao));

    printf("%6d: VIAJA \033[36mHEROI %2d \033[31mBASE %d -> BASE %d\033[0m DIST %d VEL %d CHEGA %d\n",time, hb.hero, hb.base, hb.base_n, dist, world->heroes[hb.hero].Speed, time + duracao);
}
