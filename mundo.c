
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

void add_ev(struct world *world, int time, int type, struct hero_base *hb){
    fprio_insere (world->lef, hb, type, time);
}

void chega_ev(struct world *world, int time, struct hero_base hb){
    struct base base_atual;
    bool espera;

    base_atual = world->bases[hb.base]; //deixa comparações mais faceis de ler
    world->heroes[hb.hero].Base = hb.base;

    printf("%6d: CHEGA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d/%2d) ", time, hb.hero, hb.base, base_atual.N_present, base_atual.Lotação);

    //se há vagas em B e a fila de espera em B está vazia:
    if (base_atual.N_present < base_atual.Lotação && !base_atual.N_espera){ 
        espera = true;
    }
    espera = world->heroes[hb.hero].Patience > 10 * base_atual.N_espera;

    if (espera){
        add_ev(world, time, 2, &hb);
        printf("\033[32mESPERA\033[0m\n");

        return;
    }
    add_ev(world, time, 3, &hb);
    printf("\033[31mDESISTE\033[0m\n");

    return;  
}

void espera_ev(struct world *world, int time, struct hero_base hb){
    struct base base_atual;

    base_atual = world->bases[hb.base]; //deixa comparações mais faceis de ler
    world->heroes[hb.hero].Base = hb.base;

    printf("%6d: ESPERA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d)\n", time, hb.hero, hb.base, base_atual.N_espera);
    lista_insere(base_atual.espera, world->heroes[hb.hero].ID_Hero, -1);
    add_ev(world, time, 4, &hb);

    return;  
}

void desiste_ev(struct world *world, int time, struct hero_base hb){
    srand(0);
    struct base base_atual;

    base_atual = world->bases[hb.base]; //deixa comparações mais faceis de ler
    world->heroes[hb.hero].Base = hb.base;

    printf("%6d: DESIST \033[36mHEROI %2d \033[31mBASE %d\033[0m\n", time, hb.hero, hb.base);
    hb.base = (rand()%10)-1;
    add_ev(world, time, 7, &hb);

    return;  
}