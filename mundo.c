
#include <stdio.h>
#include <stdlib.h>
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

void ini_heroi(struct world *world){
    srand(0);
    struct hero heroi_atual;
    int n;

    for (int i =0; i < N_HEROIS; i++){
        heroi_atual = world->heroes[i];
        heroi_atual.ID_hero = i;
        heroi_atual.EXP = 0;
        heroi_atual.Patience = rand()%101;
        heroi_atual.Speed = (rand()% 4951)+50;
        n = rand()%3; 
        heroi_atual.power = cjto_aleat(n, 3);
    }
}

void chega_ev(struct world *world, int time, struct hero_base hb){
    struct base base_atual;
    bool espera;

    base_atual = world->bases[hb.base]; //deixa comparações mais faceis de ler
    world->heroes[hb.hero].Base = hb.base;

    printf("%6d: CHEGA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d/%2d) ", time, hb.hero, hb.base, base_atual.present->tamanho, base_atual.Lotação);

    //se há vagas em B e a fila de espera em B está vazia:
    if (base_atual.present->tamanho < base_atual.Lotação && !base_atual.espera->tamanho){ 
        espera = true;
    }
    espera = world->heroes[hb.hero].Patience > 10 * base_atual.espera->tamanho;

    if (espera){
        fprio_insere (world, time, 2, &hb);
        printf("\033[32mESPERA\033[0m\n");

        return;
    }
    fprio_insere (world, time, 3, &hb);
    printf("\033[31mDESISTE\033[0m\n");

    return;  
}

void espera_ev(struct world *world, int time, struct hero_base hb){
    struct base base_atual;

    base_atual = world->bases[hb.base]; //deixa comparações mais faceis de ler
    world->heroes[hb.hero].Base = hb.base;

    printf("%6d: ESPERA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d)\n", time, hb.hero, hb.base, base_atual.espera->tamanho);
    lista_insere(base_atual.espera, world->heroes[hb.hero].ID_hero, -1);
    fprio_insere (world, time, 4, &hb);

    return;  
}

void desiste_ev(struct world *world, int time, struct hero_base hb){
    srand(0);
    struct base base_atual;

    base_atual = world->bases[hb.base]; //deixa comparações mais faceis de ler
    world->heroes[hb.hero].Base = hb.base;

    printf("%6d: DESIST \033[36mHEROI %2d \033[31mBASE %d\033[0m\n", time, hb.hero, hb.base);
    hb.base = (rand()%10)-1;
    fprio_insere (world, time, 7, &hb);

    return;  
}