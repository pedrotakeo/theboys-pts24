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
    struct mundo *mundo;
    mundo = ini_mundo();
    ini_heroi(mundo);
    ini_base(mundo);
    ini_missoes(mundo);
    ini_lef(mundo);
    ex_ev(mundo);


    //fprio_imprime(mundo->lef);

    mundo = destroi_mundo(mundo);

    return (0) ;
}

