// programa principal do projeto "The Boys - 2024/2"
// Autor: Pedro Takeo Shima, GRR 20240627

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "lista.h"
#include "conjunto.h"
#include "mundo.h"

// seus #defines vão aqui


// minimize o uso de variáveis globais

// programa principal
int main ()
{
    srand(0);
    // iniciar o mundo
    printf("TESTES DE EVENTO\n");
    struct world *world;
    world = ini_mundo();
    ini_heroi(world);
    ini_base(world);
    struct hero_base hb;

    printf("teste %d\n", world->bases[3].Lotação);
    lista_insere(world->bases[1].espera, 1, -1);
    printf("teste %d\n", world->bases[1].espera->prim->valor);
    printf("teste %d\n", world->bases[1].present->num);
    printf("teste %d\n", world->bases[4].Lotação);
   

    hb.hero = 1;
    hb.base = 1;
    hb.base_n = 3;

    //chega_ev(world, 0, hb);

    //desiste_ev(world, 0, hb);
    
    viaja_ev(world, 1000, hb);
    
    return (0) ;
}

