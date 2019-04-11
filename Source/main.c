#include "Biegung.h"
#include "Hilfsfunktionen.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <graphics.h>

int main()
{
    Balken T = {0};
    interfaceDisplay(&T);
    while (getchar() != 'q')
    {
        delay(100);
    }
    printf("\nPress enter to exit...");
    free(T.F);
    free(T.Fp);
    free(T.Fw);
    return 0;
}
