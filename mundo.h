#ifndef MUNDO_H
#define MUNDO_H
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"

struct jusho{
    int x;
    int y;
};

struct hero{
    int ID_Hero;
    int Power;
    int Patience;
    int Speed;
    int EXP;
    int Base;
};

struct base{
    int ID_Base;
    //struct cjto_t info;
    int Lotação;
    struct lista_t *present;
    int N_present;
    struct lista_t *espera;
    int N_espera;
    struct jusho Local;
};

struct world{
    int N_Hero;
    struct hero heroes[50];
    int N_Base;
    struct base bases[10];   
    int N_Mission;
    const int N_habilidades;
    struct fprio_t *lef;
    
};

struct hero_base{
    int hero;
    int base;
};



void add_ev(struct world *world, int time, int type, struct hero_base *hb);

//hero and base are indexes for world->hero/base arrays
void chega_ev(struct world *world, int time, struct hero_base hb);

void espera_ev(struct world *world, int time, struct hero_base hb);


#endif