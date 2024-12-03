#ifndef EVENTOS_H
#define EVENTOS_H
//-------------------
#include "fprio.h"
#include "fila.h"
#include "conjunto.h"
#include "mundo.h"
//-------------------

struct base_dist{
    int base;
    int dist;
};

void chega_ev(struct mundo *mundo, int tempo, struct dados *hb);

void espera_ev(struct mundo *mundo, int tempo, struct dados *hb);

void desiste_ev(struct mundo *mundo, int tempo, struct dados *hb);

void avisa_ev(struct mundo *mundo, int tempo, struct dados *hb);

void entra_ev(struct mundo *mundo, int tempo, struct dados *hb);

void sai_ev(struct mundo *mundo, int tempo, struct dados *hb);

void viaja_ev(struct mundo *mundo, int tempo, struct dados *hb);

void morre_ev(struct mundo *mundo, int tempo, struct dados *hb);

void missao_ev(struct mundo *mundo, int tempo, struct dados *hb);

void fim_ev (struct mundo *mundo, int tempo, struct dados *hb);

#endif