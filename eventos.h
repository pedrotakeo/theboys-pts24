#ifndef MUNDO_H
#define MUNDO_H
#include "fprio.h"
#include "fila.h"
#include "conjunto.h"
#include "mundo.h"
//-------------------

struct base_dist{
    int base;
    int dist;
};

void chega_ev(struct mundo *mundo, int tempo, struct hero_base *hb);

void espera_ev(struct mundo *mundo, int tempo, struct hero_base *hb);

void desiste_ev(struct mundo *mundo, int tempo, struct hero_base *hb);

void avisa_ev(struct mundo *mundo, int tempo, struct hero_base *hb);

void entra_ev(struct mundo *mundo, int tempo, struct hero_base *hb);

void sai_ev(struct mundo *mundo, int tempo, struct hero_base *hb);

void viaja_ev(struct mundo *mundo, int tempo, struct hero_base *hb);

void morre_ev(struct mundo *mundo, int tempo, struct hero_base *hb);

void missao_ev(struct mundo *mundo, int tempo, struct hero_base *hb);

void fim_ev (struct mundo *mundo, int tempo, int eventos, struct hero_base *hb);

#endif