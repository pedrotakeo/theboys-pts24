#ifndef MUNDO_H
#define MUNDO_H
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HEROIS 50
#define N_BASES 10
#define N_MISSOES 5256
#define N_HABILIDADES 10

struct jusho{
    int x;
    int y;
};

struct hero{
    int ID_hero;
    struct cjto_t *power;  
    int Patience;
    int Speed;
    int EXP;
    int Base;
};

struct base{
    int ID_Base;
    int Lotação;
    struct lista_t *present;
    struct lista_t *espera;
    struct jusho Local;
};

struct world{
    int N_Hero;
    struct hero heroes[N_HEROIS];
    int N_Base;
    struct base bases[N_BASES];   
    int N_Mission;
    int N_habilidades;
    struct fprio_t *lef;
};

struct hero_base{
    int hero;
    int base;
};



//hero and base are indexes for world->hero/base arrays
void chega_ev(struct world *world, int time, struct hero_base hb);

void espera_ev(struct world *world, int time, struct hero_base hb);


#endif