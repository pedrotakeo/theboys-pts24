#ifndef MUNDO_H
#define MUNDO_H
//---------------------
#include "fprio.h"
#include "fila.h"
#include "conjunto.h"
//---------------------
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
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
#define MISSAO 10
//-----------------------
#define VALOR_INVALIDO -1

struct local_cart{
    int x;
    int y;
};

struct dados{
    int heroi;
    int base_n;
    int missao;
};

struct heroi{
    int ID_hero;
    struct cjto_t *habilidades;  
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
    int fila_max;
    struct fila_t *espera;
    struct local_cart local;
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
    struct heroi herois[N_HEROIS];
    int N_base;
    struct base bases[N_BASES];   
    int N_missao;
    struct missao missoes[N_MISSOES];
    int N_habilidades;
    struct fprio_t *lef;
    int eventos_tratados;
    int missoes_cumpridas;
    int herois_mortos;
};

// cria um mundo e inicializa todos seus componentes
// RETORNO: ponteiro para struct mundo inicializado ou NULL em erro
struct mundo *ini_mundo();

// destroi mundo e todos seus componentes
// RETORNO: NULL
struct mundo *destroi_mundo(struct mundo *mundo);

// executa eventos da lef
void ex_ev(struct mundo *mundo);


#endif