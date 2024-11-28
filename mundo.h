#ifndef EVENTOS_H
#define EVENTOS_H
#include "fprio.h"
#include "fila.h"
#include "conjunto.h"
//--------------------
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
//#define T_FIM_DO_MUNDO 52560
#define N_TAMANHO_MUNDO 20000
#define N_HEROIS 50
#define N_BASES 10
#define N_MISSOES 5256
//#define N_MISSOES 525
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
#define MISSAO 10

struct jusho{
    int x;
    int y;
};

struct hero_base{
    int hero;
    int base;
    int base_n;
    int missao;
};

struct hero{
    int ID_hero;
    struct cjto_t *power;  
    int Patience;
    int Speed;
    int EXP;
    int Base;
    int vida;
};

struct base{
    int ID_Base;
    int Lotação;
    struct cjto_t *present;
    struct fila_t *espera;
    struct jusho Local;
    int fila_max;
    int missoes;
};

struct missao{
    int ID_missao;
    struct cjto_t *HAB_nec;
    int perigo;
    struct jusho local;
    int tentativas;
};

struct world{
    int N_Hero;
    struct hero heroes[N_HEROIS];
    int N_Base;
    struct base bases[N_BASES];   
    int N_Mission;
    struct missao missoes[N_MISSOES];
    int N_habilidades;
    struct fprio_t *lef;
    int missoes_cumpridas;
};


//hero and base are indexes for world->hero/base arrays
struct world *ini_mundo();

struct world *destroi_mundo(struct world *world);

void ini_heroi(struct world *world);

void ini_base(struct world *world);

void ini_lef (struct world *world);

void ex_ev(struct world *world);

void ini_missoes(struct world *world);


#endif