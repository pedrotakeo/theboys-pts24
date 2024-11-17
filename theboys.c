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
    // iniciar o mundo
    printf("TESTES DE EVENTO");



    int i = 1;
    printf("%6d: CHEGA \033[36mHEROI %d \033[31mBASE %d\033[0m ", i, i, i);
    printf("\033[32mESPERA\033[0m\n");
    printf("%6d: ESPERA \033[36mHEROI %2d \033[31mBASE %d\033[0m (%2d)\n", i, i, i, i);
    printf("%6d: DESIST \033[36mHEROI %2d \033[31mBASE %d\033[0m\n", i, i, i);
    // executar o laço de simulação

    // destruir o mundo

    return (0) ;
}

