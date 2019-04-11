#ifndef _HILFSFUNKTIONEN_H_
#define _HILFSFUNKTIONEN_H_
#include "Biegung.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <graphics.h>
#include <pthread.h>

typedef struct
{
    double dens1;
    double dens2;
    double p;
    double len;
}Flaechenlast;

typedef struct
{
    double l;
    double lager[2];
    double lagerF[3];
    int Fanz;
    double* F;
    double* Fp;
    double* Fw;
    double FTM;
    double E;
    double zMax;
    double sigmaMax;
    double Wy;
    double lagerwert;
    double lagerMb;
    double C1;
    double C2;
    double MbMax[2];
    double wmax[2];
    Flaechenlast* Fl;
    int Flanz;
}Balken;

typedef struct
{
    Balken T;
    double start;
    double end;
    double *arr;
}arg_t;



double foepplh0(double, double);
double foepplh1(double, double);
double foepplh2(double, double);
double foepplh3(double, double);
void *threading0(void*);
double floop0(Balken, double);
double floop1(Balken, double);
double floop2(Balken, double);
double floop3(Balken, double);
double Flloop1(Balken, double);
double Flloop2(Balken, double);
double Flloop3(Balken, double);
double h2(double);
double h3(double);
void deconstr(Balken*);
void berechneKonst(Balken*);
void SwitchIy(Balken*);
void zeichneLager2(double);
void zeichneLager1(double);
void zeichneLager3(double);
void zeichneKraft(double, double);
void zeichneFl(double, double, double, double);
void updateScreen(Balken*, int, int, int, int*);

#endif
