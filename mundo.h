#ifndef MUNDO_H
#define MUNDO_H
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
//--------------------
#define T_INICIO 0
//#define T_FIM_DO_MUNDO 525600
#define T_FIM_DO_MUNDO 10000
#define N_TAMANHO_MUNDO 20000
#define N_HEROIS 50
#define N_BASES 10
#define N_MISSOES 5256
#define N_HABILIDADES 10
//----------------------
#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define FIM 9

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
    struct cjto_t *present;
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
    int base_n;
};



//hero and base are indexes for world->hero/base arrays
struct world *ini_mundo();

struct world *destroi_mundo(struct world *world);

void ini_heroi(struct world *world);

void ini_base(struct world *world);

void ini_lef (struct world *world);

void ex_ev(struct world *world);

void event_trigger (struct world *world, struct hero_base *data);

void chega_ev(struct world *world, int time, struct hero_base *hb);

void espera_ev(struct world *world, int time, struct hero_base *hb);

void desiste_ev(struct world *world, int time, struct hero_base *hb);

void avisa_ev(struct world *world, int time, struct hero_base *hb);

void entra_ev(struct world *world, int time, struct hero_base *hb);

void sai_ev(struct world *world, int time, struct hero_base *hb);

void viaja_ev(struct world *world, int time, struct hero_base *hb);

void fim (struct world *world);


#endif