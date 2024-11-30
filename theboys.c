// programa principal do projeto "The Boys - 2024/2"
// Autor: Pedro Takeo Shima, GRR 20240627

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"
#include "fila.h"
#include "conjunto.h"
#include "mundo.h"
#include "eventos.h"

int main ()
{
    srand(0);
    struct mundo *mundo;
    mundo = ini_mundo();
    if (!mundo){
        return 0;
    }
    
    ex_ev(mundo);
    mundo = destroi_mundo(mundo);

    return (0);
}

