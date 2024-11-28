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

struct local_cart{
    int x;
    int y;
};

struct hero_base{
    int heroi;
    int base;
    int base_n;
    int missao;
};

struct heroi{
    int ID_hero;
    struct cjto_t *power;  
    int paciencia;
    int velocidade;
    int EXP;
    int base;
    int vida;
};

struct base{
    int ID_base;
    int lotacao;
    struct cjto_t *present;
    struct fila_t *espera;
    struct local_cart local;
    int fila_max;
    int missoes;
};

struct missao{
    int ID_missao;
    struct cjto_t *HAB_nec;
    int perigo;
    struct local_cart local;
    int tentativas;
};

struct mundo{
    int N_heroi;
    struct heroi heroes[N_HEROIS];
    int N_base;
    struct base bases[N_BASES];   
    int N_missao;
    struct missao missoes[N_MISSOES];
    int N_habilidades;
    struct fprio_t *lef;
    int missoes_cumpridas;
};


//heroi and base are indexes for mundo->heroi/base arrays
struct mundo *ini_mundo();

struct mundo *destroi_mundo(struct mundo *mundo);

void ini_heroi(struct mundo *mundo);

void ini_base(struct mundo *mundo);

void ini_lef (struct mundo *mundo);

void ex_ev(struct mundo *mundo);

void ini_missoes(struct mundo *mundo);


#endif