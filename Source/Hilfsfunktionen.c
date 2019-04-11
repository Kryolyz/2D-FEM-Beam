#ifndef _HILFSFUNKTIONEN_C_
#define _HILFSFUNKTIONEN_C_
#include "Biegung.h"
#include "Hilfsfunktionen.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <graphics.h>
#include <pthread.h>
#define PI 3.14159265358979323846
#define bogm(x) PI/180 * x
#define ws 640
#define sqr(x) x*x

//Flächenmomente 2. Grad nach DIN-Standard in mm^4
double  I100 = 1710000;
double  I120 = 3280000;
double  I140 = 5730000;
double  I160 = 9350000;
double  I180 = 14500000;
double  I200 = 21400000;
double  I220 = 30600000;

double  IPE100 = 1710000;
double  IPE120 = 3180000;
double  IPE140 = 5410000;
double  IPE160 = 8690000;
double  IPE180 = 13200000;
double  IPE200 = 19430000;
double  IPE220 = 27700000;

double foepplh0(double xo, double x)
{
    if (x-xo >= 0) return 1;
    else return 0;
}

double foepplh1(double xo, double x)
{
    if (x-xo >= 0) return (x-xo);
    else return 0;
}

double foepplh2(double xo, double x)
{
    double erg;
    if (x-xo >= 0)
    {
        erg = x-xo;
        erg = erg * erg;
        return erg;
    }
    else return 0;
}

double foepplh3(double xo,double x)
{
   double erg;
    if (x-xo >= 0)
    {
        erg = x-xo;
        erg = erg*erg*erg;
        return erg;
    }
    else return 0;
}

void berechneKonst(Balken* T)
{
    double ablRBD1[2] = {0,0}; //Randbedingung für Ableitung
    double RBD1[2] = {0,0}; //Randbedingungen
    double RBD2[2] = {0,0};
    int ablFlag = 0; //Flag für Eingabe

    if (T->lagerwert == 1)
    {
        ablFlag = 1;
        ablRBD1[0] = T->lager[0];
        ablRBD1[1] = 0;
        RBD1[0] = T->lager[0];
        RBD1[1] = 0;
    }
    else
    {
        RBD1[0] = T->lager[0];
        RBD2[0] = T->lager[1];
        RBD1[1] = 0;
        RBD2[1] = 0;
    }
    printf("\n\nIntegrationskonstanten berechnen...");
    if (ablFlag)
    {
        T->C1 = - floop2(*T , ablRBD1[0]);
        T->C2 = - floop3(*T , RBD1[0] ) - T->C1 * RBD1[0];
    printf("\nC1 : %f\nC2 : %f", T->C1, T->C2);
    }
    else
    {
        T->C1 = - (floop3(*T, RBD1[0])
                - (floop3(*T, RBD2[0])))
                / (RBD1[0] - RBD2[0]);
        printf("\nC1 : %f\n", T->C1);
        T->C2 =  - floop3(*T, RBD1[0]) - T->C1 * RBD1[0];
        printf("C2 : %f\n", T->C2);
    }
}

void deconstr(Balken* T)
{
    T->l = 0;
    T->lager[0] = 0;
    T->lager[1] = 0;
    T->lagerF[0] = 0;
    T->lagerF[1] = 0;
    T->lagerF[2] = 0;
    T->Fanz = 0;
    T->FTM = 0;
    T->E = 0;
    T->zMax = 0;
    T->sigmaMax = 0;
    T->Wy = 0;
    T->lagerwert = 0;
    T->lagerMb = 0;
    T->C1 = 0;
    T->C2 = 0;
    T->MbMax[0] = 0;
    T->MbMax[1] = 0;
    T->wmax[0] = 0;
    T->wmax[1] = 0;
    T->Flanz = 0;

}

void SwitchIy(Balken* T)
{
    int choice;
    double Iy;
    float z;
    printf("Flaechenmoment 2. Grades nach DIN-Standard in mm^4 :\n1 : IPE100\t8 : I100\n2 : IPE120\t9 : I120\n3 : IPE140\t10 : I140\n4 : IPE160\t11 : I160\n5 : IPE180\t12 : I180\n6 : IPE200\t13 : I200\n7 : IPE220\t14 : I220\n");
    printf("Nummer auswaehlen oder 15 fuer eigene Eintragung : ");
    scanf("%d", &choice );
    switch(choice)
    {
    case 1 :
        T->FTM = IPE100;
        T->zMax = 50;
        break;
    case 2 :
        T->FTM = IPE120;
        T->zMax = 60;
        break;
    case 3 :
        T->FTM = IPE140;
        T->zMax = 70;
        break;
    case 4 :
        T->FTM = IPE160;
        T->zMax = 80;
        break;
    case 5:
        T->FTM = IPE180;
        T->zMax = 90;
        break;
    case 6:
        T->FTM = IPE200;
        T->zMax = 100;
        break;
    case 7:
        T->FTM = IPE220;
        T->zMax = 110;
        break;
    case 8:
        T->FTM = I100;
        T->zMax = 50;
        break;
    case 9:
        T->FTM = I120;
        T->zMax = 60;
        break;
    case 10:
        T->FTM = I140;
        T->zMax = 70;
        break;
    case 11:
        T->FTM = I160;
        T->zMax = 80;
        break;
    case 12:
        T->FTM = I180;
        T->zMax = 90;
        break;
    case 13:
        T->FTM = I200;
        T->zMax = 100;
        break;
    case 14:
        T->FTM = I220;
        T->zMax = 110;
        break;
    case 15:
        printf("Iy eingeben : ");
        scanf("%lf", &Iy);
        T->FTM = Iy;
        printf("zMax eingeben : ");
        scanf("%f", &z);
        T->zMax = z;
        break;
    default:
        printf("Fehler.\n");
        break;
    }
}

void *threading0(void *var)
{
    arg_t *vari = (arg_t*) var;
    double a, dummy, max = 0, min = 0;
    for (a = vari->start; a < vari->end; a = a + 0.001)
    {
        dummy = (a-vari->start) * 1000;
        vari->arr[(int)dummy] = floop0(vari->T, a);
        if(vari->arr[(int)dummy] > max) max = vari->arr[(int)dummy];
        if(vari->arr[(int)dummy] < min) min = vari->arr[(int)dummy];
    }
    printf("\nLoop done\n");
    vari->end = max;
    vari->start = min;
}

double floop0(Balken T, double i) //"force loop hoch 0"
{
    int b;
    double erg = 0;
    for ( b = 0; b < T.Fanz; b++)
    {
        erg = erg + sin(bogm(T.Fw[b])) * T.F[b] * foepplh0(T.Fp[b], i);
    }
    for ( b = 0; b < T.Flanz; b++)
    {
        double iib = foepplh1(T.Fl[b].p, i) - foepplh1(T.Fl[b].p + T.Fl[b].len, i);
        double steig = (T.Fl[b].dens2 - T.Fl[b].dens1) / T.Fl[b].len;
        erg = erg - iib * T.Fl[b].dens1
                  - (0.5 * iib * iib * steig) ; //i * steig = dens2x
    }
    erg = erg + T.lagerF[0] * foepplh0( T.lager[0] , i)
              + T.lagerF[2] * foepplh0( T.lager[1] , i);
    return erg;
}

double floop1(Balken T, double i)
{
    int b;
    double erg = 0;
    for ( b = 0; b < T.Fanz; b++)
    {
        erg = erg + sin(bogm(T.Fw[b])) * T.F[b] * foepplh1( T.Fp[b] , i );
    }
    erg = erg + Flloop1(T, i);
    erg = erg + T.lagerF[0] * foepplh1( T.lager[0] , i)
              + T.lagerF[2] * foepplh1( T.lager[1] , i)
              - T.lagerMb   * foepplh0( T.lager[0] , i);
    return erg;
}

double floop2(Balken T, double i)
{
    double erg = 0;
    int b;
    for ( b = 0; b < T.Fanz; b++ )
    {
        erg = erg + 0.5 * (sin(bogm(T.Fw[b])) * T.F[b] * foepplh2(T.Fp[b], i));
    }
    erg = erg + Flloop2(T, i);
    erg = erg + 0.5 * (T.lagerF[0] *  foepplh2(T.lager[0], i)
                     + T.lagerF[2] *  foepplh2(T.lager[1], i))
                     - T.lagerMb   *  foepplh1(T.lager[0], i);
    return erg;
}

double floop3(Balken T, double i)
{
    int b;
    double erg = 0;
    double pre = 1.0/6.0;
    for ( b = 0; b < T.Fanz; b++)
    {
        erg = erg + pre * (sin(bogm(T.Fw[b])) * T.F[b] * foepplh3(T.Fp[b], i));
    }
    erg = erg + Flloop3(T, i);
    erg = erg + pre * (  T.lagerF[0] * foepplh3(T.lager[0], i)
                       + T.lagerF[2] * foepplh3(T.lager[1], i))
             - 1.0/2.0 * T.lagerMb   * foepplh2(T.lager[0], i);

    return erg;
}

double Flloop1(Balken T, double i)
{
    int b;
    double erg = 0;
    double iib;
    double steig;
    float spDrei; //Schwerpunkt Dreieck
    for ( b = 0; b < T.Flanz; b++)
    {
        iib = foepplh1(T.Fl[b].p, i) - foepplh1(T.Fl[b].p + T.Fl[b].len, i);
        if ( T.Fl[b].dens1 <= T.Fl[b].dens2)
        {
            steig = (  T.Fl[b].dens2 - T.Fl[b].dens1 ) / T.Fl[b].len;
            if (steig >= 0)
                    spDrei = 1.0 / 3.0;

            else
                    spDrei = 2.0 / 3.0;
            erg = erg - iib * T.Fl[b].dens1 * (iib * 0.5 + foepplh1(T.Fl[b].p + T.Fl[b].len, i))
                      - steig * iib * iib * 0.5 * (spDrei * iib + foepplh1(T.Fl[b].p + T.Fl[b].len, i)); // * p+ 2/3 * l
        }
        else if ( T.Fl[b].dens2 < T.Fl[b].dens1 )
        {
            steig = (  T.Fl[b].dens1 - T.Fl[b].dens2 ) / T.Fl[b].len;
            if (steig >= 0)
                spDrei = 1.0 / 3.0;

            else
                spDrei = 2.0 / 3.0;
            erg = erg - iib * T.Fl[b].dens2 * (iib * 0.5 + foepplh1(T.Fl[b].p + T.Fl[b].len, i))
                      - steig * iib * iib * 0.5 * (spDrei * iib + foepplh1(T.Fl[b].p + T.Fl[b].len, i)); // * p+ 2/3 * l
        }
    }
    return erg;
}

double Flloop2(Balken T, double i)
{
    int b;
    double erg = 0;
    double pre = 1.0/2.0;
    double steig;
    double iib;
    float spDrei; //Schwerpunkt Dreieck
    for ( b = 0; b < T.Flanz; b++)
    {
        iib = foepplh1(T.Fl[b].p, i) - foepplh1(T.Fl[b].p + T.Fl[b].len, i);
        if ( T.Fl[b].dens1 <= T.Fl[b].dens2)
        {
            steig = (  T.Fl[b].dens2 - T.Fl[b].dens1 ) / T.Fl[b].len;
            if (steig >= 0)
                    spDrei = 1.0 / 3.0;

            else
                    spDrei = 2.0 / 3.0;
            erg = erg - iib * T.Fl[b].dens1 * pre * h2(iib * 0.5 + foepplh1(T.Fl[b].p + T.Fl[b].len, i))
                      - (steig * iib * iib * 0.5 * pre * h2(spDrei * iib + foepplh1(T.Fl[b].p + T.Fl[b].len, i))); // * p+ 2/3 * l
        }
        if ( T.Fl[b].dens2 < T.Fl[b].dens1 )
        {
            steig = (  T.Fl[b].dens1 - T.Fl[b].dens2 ) / T.Fl[b].len;
            if (steig >= 0)
                spDrei = 1.0 / 3.0;

            else
                spDrei = 2.0 / 3.0;
            erg = erg - iib * T.Fl[b].dens2 * pre * h2(iib * 0.5 + foepplh1(T.Fl[b].p + T.Fl[b].len, i))
                      - steig * iib * iib * 0.5 * pre * h2(spDrei * iib + foepplh1(T.Fl[b].p + T.Fl[b].len, i)); // * p+ 2/3 * l
        }
    }
    return erg;

}

double Flloop3(Balken T, double i)
{
    int b;
    double erg = 0;
    double pre = 1.0/6.0;
    double steig;
    double iib;
    float spDrei; //Schwerpunkt Dreieck
    for ( b = 0; b < T.Flanz; b++)
    {
        iib = foepplh1(T.Fl[b].p, i) - foepplh1(T.Fl[b].p + T.Fl[b].len, i);
        if ( T.Fl[b].dens1 <= T.Fl[b].dens2)
        {
            steig = (  T.Fl[b].dens2 - T.Fl[b].dens1 ) / T.Fl[b].len;
            if (steig >= 0)
                    spDrei = 1.0 / 3.0;

            else
                    spDrei = 2.0 / 3.0;
            erg = erg - iib * T.Fl[b].dens1 * pre * h3(iib * 0.5 + foepplh1(T.Fl[b].p + T.Fl[b].len, i))
                      - steig * iib * iib * 0.5 * pre * h3(spDrei * iib + foepplh1(T.Fl[b].p + T.Fl[b].len, i));
        }
        if ( T.Fl[b].dens2 < T.Fl[b].dens1 )
        {
            steig = (  T.Fl[b].dens1 - T.Fl[b].dens2 ) / T.Fl[b].len;
            if (steig >= 0)
                spDrei = 1.0 / 3.0;

            else
                spDrei = 2.0 / 3.0;
            erg = erg - iib * T.Fl[b].dens2 * pre * h3(iib * 0.5 + foepplh1(T.Fl[b].p + T.Fl[b].len, i))
                      - steig * iib * iib * 0.5 * pre * h3(spDrei * iib + foepplh1(T.Fl[b].p + T.Fl[b].len, i));
        }
    }
    return erg;
}

double h3(double val)
{
    return val*val*val;
}

double h2(double val)
{
    return val*val*val;
}

void zeichneLager2(double x)
{
    setlinestyle(3,0,2);
    moveto(x, 192 + 5);
    linerel(8,12);
    linerel(-16,0);
    linerel(8,-12);
}

void zeichneLager1(double x)
{
    setlinestyle(3,0,2);
    moveto(x, 192 + 5);
    linerel(8,12);
    linerel(-16,0);
    linerel(8,-12);
    line(x-8,192+22,x+8,192+22);
}

void zeichneLager3(double x)
{
    setlinestyle(2,0,3);
    line(x,192-30,x,192+30);
}

void zeichneKraft(double x, double f, double w)
{
    int x1;
    int y1;
    int x2;
    int y2;
    setlinestyle(0 , 0 , 2);

    moveto(x , 192);
    linerel(0.05 * f * cos(bogm(w))  ,  0.05 * f * sin(bogm(w)));
    moveto(x , 192);
    linerel(16 * cos(bogm(w - 45)) , 16 * sin(bogm(w - 45)));
    x1 = getx();
    y1 = gety();
    moveto(x, 192);
    linerel(16 * cos(bogm(w + 45)) , 16 * sin(bogm(w + 45)));
    x2 = getx();
    y2 = gety();
    line(x1,y1,x2,y2);

}

void zeichneFl(double x1, double y1, double x2, double y2)
{
    setlinestyle(0,0,1);
    setfillstyle(9,WHITE);
    int points[8] = {x1,0.5*0.6*ws,x1,y1,x2,y2,x2,0.5*0.6*ws};
    fillpoly(4, points);
}

void updateScreen(Balken* T, int bActive, int lActive, int l3Active, int* windows)
{
    double wsn = 0.6 * ws;
    if (windows[3]) berechneLagerF(T);
    if (windows[1]) DurchbMax(T);
    if (windows[0]) BiegemMax(T);
    setcurrentwindow(windows[2]);
    int i;
    char fBesch[6];
    settextstyle(10, HORIZ_DIR, 4);
    double Uy = wsn - textheight("B") - 15;
    int polyp[8] = {0 , 347 , 0 , Uy , 100 + textwidth("BFLagerLager3Fl") , Uy , 100 + textwidth("BFLagerLager3Fl") , 347 };


        setcurrentwindow(windows[2]);
        cleardevice(); //Nach jeder Schleife aktualisieren

        //Tasten darstellen
        setfillstyle(SLASH_FILL, RED);
        fillpoly(4 , polyp);
        setlinestyle(0,0,1);
        outtextxy( 10 , 0.6 * ws - textheight("B"), "B"); //Klcikflä
        outtextxy( 10 + textwidth("B") + 20, 0.6 * ws - textheight("F"), "F");
        outtextxy( 10 + textwidth("BF")+ 40, 0.6 * ws - textheight("Lager"), "Lager");
        outtextxy( 10 + textwidth("BFLager") + 60, 0.6 * ws - textheight("Lager3"), "Lager3");
        outtextxy( 10 + textwidth("BFLagerLager3") + 80, 0.6 * ws - textheight("Fl"), "Fl");
        line(0 , Uy , 100 + textwidth("BFLagerLager3Fl"), Uy);
        line(0 , 347, 100 + textwidth("BFLagerLager3Fl"), 347);                             //Horizontale Linie unten
        line(20 + textwidth("B"), Uy , 20 + textwidth("B"), 0.6 * ws );
        line(40 + textwidth("BF"), Uy , 40 + textwidth("BF"), 0.6 * ws );                   //vertikale Linien unten
        line(60 + textwidth("BFLager"), Uy , 60 + textwidth("BFLager"), 0.6 * ws);
        line(80 + textwidth("BFLagerLager3"), Uy , 80 + textwidth("BFLagerLager3"), 0.6 * ws);
        line(100 + textwidth("BFLagerLager3Fl"), Uy , 100 + textwidth("BFLagerLager3Fl"), 0.6 * ws);

        outtextxy(0 , 0 , "Q(x)");
        outtextxy(textwidth("Q(x)") + 10, 0, "Mb(x)");
        outtextxy(textwidth("Q(x)Mb(x)") + 20, 0 , "w(x)");
        outtextxy(640 - textwidth("Iy") - 5, 0, "Iy");
        outtextxy(640 - textwidth("E") - 10, textheight("Iy") ,"E");
        line(0, textheight("Q(x)"), textwidth("Q(x)Mb(x)w(x)") + 25, textheight("Q(x)"));       //horizontale Linie oben
        line(640, textheight("Iy"), 640 - textwidth("Iy") - 10, textheight("Iy"));
        line(640, textheight("E") + textheight("Iy"), 640 - textwidth("Iy") - 10, textheight("E") + textheight("Iy"));
        line(textwidth("Q(x)") + 5, textheight("Q(x)"), textwidth("Q(x)") + 5, 0);              //vertikale Linien oben
        line(textwidth("Q(x)Mb(x)") + 15, textheight("Q(x)"), textwidth("Q(x)Mb(x)") + 15, 0);
        line(textwidth("Q(x)Mb(x)w(x)") + 25, textheight("Q(x)"), textwidth("Q(x)Mb(x)w(x)") + 25, 0);
        line(640 - textwidth("Iy") - 10, 0, 640 - textwidth("Iy") - 10, textheight("Iy") + textheight("E"));



        if (bActive) //Aktive Dinge darstellen
        {
            setlinestyle(0 , 0 , 3);
            line(0.5 * ws - 0.5 * T->l * 100, 0.5 * wsn, 0.5 * ws + 0.5 * T->l * 100, 0.5 * wsn);
            setlinestyle(0 , 0 , 1);
            line(0.5 * ws - 0.5 * T->l * 100, 0.5 * wsn - 10, 0.5 * ws - 0.5 * T->l * 100, 0.5 * wsn + 10);
            line(0.5 * ws + 0.5 * T->l * 100, 0.5 * wsn - 10, 0.5 * ws + 0.5 * T->l * 100, 0.5 * wsn + 10);
            settextstyle(0,0,2);
            sprintf(fBesch, "%.2fm", T->l);
            outtextxy(0.5 * ws + 0.5 * T->l * 100 + 5, 0.5 * wsn - 0.5 * textheight(fBesch), fBesch);

            for(i = 0; i < T->Flanz; i++)//Flächenlast einzeichnen
                {
                zeichneFl(0.5 * ws - 50 * T->l + 100 * T->Fl[i].p, 0.5 * wsn - 0.05 * T->Fl[i].dens1, 0.5 * ws - 50 * T->l + 100 * T->Fl[i].p + 100 * T->Fl[i].len, 0.5 * wsn - 0.05 * T->Fl[i].dens2 );
                sprintf(fBesch, "%.1fkN", fabs(T->Fl[i].dens1 / 1000));
                settextstyle(0,0,1);
                outtextxy(0.5 * ws - 50 * T->l + 100 * T->Fl[i].p - 0.5 * textwidth(fBesch), 0.5 * wsn - 0.05 * T->Fl[i].dens1 - foepplh0(0, T->Fl[i].dens1) * textheight(fBesch) + foepplh0(T->Fl[i].dens1, 0) * textheight(fBesch), fBesch);
                sprintf(fBesch, "%.1fkN", fabs(T->Fl[i].dens2 / 1000));
                outtextxy(0.5 * ws - 50 * T->l + 100 * T->Fl[i].p + 100 * T->Fl[i].len - 0.5 * textwidth(fBesch), 0.5 * wsn - 0.05 * T->Fl[i].dens2 - foepplh0(0, T->Fl[i].dens2) * textheight(fBesch) + foepplh0(T->Fl[i].dens2, 0) * textheight(fBesch), fBesch);
                printf("\np%i : %f\tlen%i : %f\n", i+1,T->Fl[0].p, i+1 ,T->Fl[0].len);
                }


            for (i = 0; i < T->Fanz; i++)// Kräfte einzeichnen
                {
                zeichneKraft(0.5 * ws - 50 * T->l + 100 * T->Fp[i], T->F[i], T->Fw[i]);
                sprintf(fBesch, "%.1fkN", fabs(T->F[i]/1000));
                settextstyle(0,0,2);
                if(T->Fanz + T->Flanz > 3)
                settextstyle(0,0,1);
                outtextxy(0.5 * ws - 50 * T->l + 100 * T->Fp[i] - 0.5 * textwidth(fBesch) + 50 * cos(bogm(T->Fw[i])), 192 + 0.05 * T->F[i] * sin(bogm(T->Fw[i])) - textheight(fBesch), fBesch);
                if(T->Fw[i] != 270 && T->Fw[i] != 540)
                {
                sprintf(fBesch, "%.0f°" ,  T->Fw[i] - 360);
                outtextxy(0.5 * ws - 50 * T->l + 100 * T->Fp[i] + 2, 0.5 * wsn - 10, fBesch);
                }
                printf("\nFp(%d) = %.2f \tF(%d) = %.2f", i , T->Fp[i], i , T->F[i]);
                }

            if (lActive) // Lager einzeichnen
            {
                zeichneLager2(0.5 * ws - T->l * 50 + T->lager[0] * 100);
                if(lActive == 2)
                {
                    zeichneLager1(0.5 * ws - T->l * 50 + T->lager[1] * 100);
                }
                printf("\nLager 1 bei l = %.2f\tLager 2 bei l = %.2f", T->lager[0], T->lager[1]);
            }
            else if(l3Active) // 3-wertiges Lager einzeichnen
            {
                zeichneLager3(0.5 * ws - 50 * T->l + T->lager[0] * 100);
                printf("\nLager bei l = %.2f", T->lager[0]);
            }
        }

    swapbuffers();
}

#endif
