#include <stdlib.h>
#include <stdio.h>
#include "matriz.h"

matriz *psM;

int main () {
    psM = psmCreaMatriz (500);
    vCreaFreeScale (psM);
    if (cEsSimetrica(psM)) printf ("OK\n");
    vImprimeEnlaces (psM);
    return (0);
}
