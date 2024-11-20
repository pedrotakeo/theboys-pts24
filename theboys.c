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
    struct hero_base *a;
    struct hero_base *b;

    if (!(a = malloc(sizeof(struct hero_base)))){
        return 0;

    }
    if (!(b = malloc(sizeof(struct hero_base)))){
        return 0;

    }
    a->hero = 1;
    a->base = 1;
    a->base_n = 3;
    b->hero = 2;
    b->base = 3;
    b->base_n = 4;

    printf("teste %d\n", world->bases[3].Lotação);
    lista_insere(world->bases[1].espera, 1, -1);
    printf("teste %d\n", world->bases[1].espera->prim->valor);
    printf("teste %d\n", world->bases[1].present->num);
    printf("teste %d\n", world->bases[4].Lotação);
   
    chega_ev(world, 0, a);

    desiste_ev(world, 0, a);    
    viaja_ev(world, 3, b);

    fprio_imprime(world->lef);
    printf("\n");
    world = destroi_mundo(world);

    return (0) ;
}

