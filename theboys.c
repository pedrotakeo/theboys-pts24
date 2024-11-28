// programa principal do projeto "The Boys - 2024/2"
// Autor: Pedro Takeo Shima, GRR 20240627

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "fila.h"
#include "conjunto.h"
#include "mundo.h"
#include "eventos.h"

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
    ini_missoes(world);
    ini_lef(world);
    ex_ev(world);


    //fprio_imprime(world->lef);

    world = destroi_mundo(world);

    return (0) ;
}

