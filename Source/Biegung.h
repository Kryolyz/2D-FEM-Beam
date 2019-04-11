#ifndef _BIEGUNG_H_
#define _BIEGUNG_H_
#include "Hilfsfunktionen.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <graphics.h>
#include <pthread.h>

//Interfacefunktionen
void interfaceDisplay(Balken*);
void berechneLagerF(Balken*);
void BiegemMax(Balken*);
void zeigeBiegemoment(Balken);
void DurchbMax(Balken*);
void zeigeQuerkraft(Balken);
void zeigeBiegung(Balken);
void eingabeBalken(Balken*);
void MaxSigma(Balken*);


#endif
