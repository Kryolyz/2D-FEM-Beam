#ifndef _BIEGUNG_C_
#define _BIEGUNG_C_
#include "Biegung.h"
#include "Hilfsfunktionen.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <graphics.h>
#include <pthread.h>
#include <time.h>
#define PI 3.1415926535897932384626433
#define bogm(x) PI/180 * x
#define ws 640
#define sqr(x) x*x

int windows[4];

void interfaceDisplay(Balken* T)
{
    T->E = 210000;
    double wsn = 0.6 * ws;
    windows[2] = initwindow(ws , wsn, "Interface" , ws , ws);
    setcurrentwindow(windows[2]);
    int bActive = 0;
    int lActive = 0;    //Flags, T->Fanz als Flag für Kräfte
    int l3Active = 0;
    int clix = 0;
    int cliy = 0;       //Mausposition
    int movx = 0;
    int movy = 0;
    int stop = 0;
    int i;
    settextstyle(10, HORIZ_DIR, 4);
    int mspeicher = 5;
    int Flmspeicher = 16;
    T->F  = (double*)malloc(mspeicher * sizeof(double));
    T->Fp = (double*)malloc(mspeicher * sizeof(double)); //Kräften Speicher geben
    T->Fw = (double*)malloc(mspeicher * sizeof(double));
    T->Fl = (Flaechenlast*)malloc(Flmspeicher * sizeof(double));

    do
    {
    updateScreen(T , bActive , lActive , l3Active, windows); //Alle Flächen neu laden

    //Alle relevanten Klickflächen abfragen
    while(!ismouseclick(WM_LBUTTONDOWN))
    {
     delay(10);
     continue;
    }
    getmouseclick(WM_LBUTTONDOWN, clix, cliy);
    //variable Balkenlänge
    if (((clix < 0.5 * ws - 50 * T->l + 8 && clix > 0.5 * ws - 50 * T->l - 8) || (clix < 0.5 * ws + 50 * T->l + 8 && clix > 0.5 * ws + 50 * T->l - 8)) && cliy < 0.5 * wsn + 8 && cliy > 0.5 * wsn - 8)
    {
        do
        {
         getmouseclick(WM_MOUSEMOVE, movx , movy);
         if (movx > 0)
         {
            T->l = (movx - 0.5 * ws) / 50;
            if (T->l <= 0) T->l = 0;
            updateScreen(T,bActive , lActive , l3Active , windows);
            clearmouseclick(WM_LBUTTONUP);
         }
        }while(!ismouseclick(WM_LBUTTONUP));
    }

    if(bActive)//variable Kräfte
    {
    for (i = 0; i < T->Fanz; i++) //variable Kräfte
    {
        if (clix < 0.5 * ws - 50 * T->l + 100 * T->Fp[i] + 8
         && clix > 0.5 * ws - 50 * T->l + 100 * T->Fp[i] - 8
         && cliy < 0.5 * wsn + (0.05 * T->F[i]) * foepplh0(0, sin(bogm(T->Fw[i]))) - 5
         && cliy > 0.5 * wsn - (0.05 * T->F[i]) * foepplh0(sin(bogm(T->Fw[i])), 0) + 5 )
        {
            do
            {
                getmouseclick(WM_MOUSEMOVE, movx , movy);
                if(movx > 0
                   && movx <= 0.5 * ws + 50 * T->l
                   && movx >= 0.5 * ws - 50 * T->l
                   && movy <= 0.5 * wsn + 15 + (0.05 * T->F[i]) * foepplh0(0, sin(bogm(T->Fw[i])))
                   && movy >= 0.5 * wsn - 15 - (0.05 * T->F[i]) * foepplh0(sin(bogm(T->Fw[i])), 0) ) //winkel eintragen
                {
                T->Fp[i] = (movx - 0.5 * ws + 50 * T->l) / 100;
                if(sin(bogm(T->Fw[i])) < 0)
                {
                T->F[i] = (- movy + 0.5 * wsn) / 0.05;
                }
                else
                {
                T->F[i] = (movy - 0.5 * wsn) / 0.05;
                }
                updateScreen(T, bActive, lActive, l3Active, windows);
                clearmouseclick(WM_LBUTTONUP);

                if(ismouseclick(WM_RBUTTONDOWN))
                {
                    do
                    {
                        getmouseclick(WM_MOUSEMOVE, movx , movy);
                        if(movx > 0)
                        {
                        T->Fw[i] = 360 - 180/PI * atan2((0.5 * wsn - movy), (movx - (0.5 * ws - 50 * T->l + 100 * T->Fp[i])));
                        updateScreen(T, bActive, lActive, l3Active, windows);
                        }
                        else delay(30);
                    }while(!ismouseclick(WM_RBUTTONUP));
                clearmouseclick(WM_RBUTTONDOWN);
                clearmouseclick(WM_RBUTTONUP);
                }
                else delay(30);
                }
                else delay(30);
            }while(!ismouseclick(WM_LBUTTONUP));
        }
    }
    }
    if (lActive)// variable Lager
    {
    for ( i = 0; i < lActive; i++)
    {
        if (clix < 0.5 * ws - 50 * T->l + 100 * T->lager[i] + 12 && clix > 0.5 * ws - 50 * T->l + 100 * T->lager[i] - 12 && cliy > 0.5 * wsn && cliy < 0.5 * wsn + 15)
        {
            do
            {
                getmouseclick(WM_MOUSEMOVE, movx, movy);
                if(movx > 0 && movx <= 0.5 * ws + 50 * T->l && movx >= 0.5 * ws - 50 * T->l)
                {
                    T->lager[i] = (movx - 0.5 * ws + 50 * T->l) / 100;
                    updateScreen(T , bActive , lActive , l3Active , windows);
                    clearmouseclick(WM_LBUTTONUP);
                }
                else delay(30);
            }while(!ismouseclick(WM_LBUTTONUP));
        }
    }
    }
    else if (l3Active) //lager3 variable
    {
        if (clix < 0.5 * ws - 50 * T->l + 100 * T->lager[0] + 6 && clix > 0.5 * ws - 50 * T->l + 100 * T->lager[0] - 6 && cliy > 0.5 * wsn - 25 && cliy < 0.5 * wsn + 25)
        {
            printf("%d\t%d\n", clix,cliy);
            do
            {
                getmouseclick(WM_MOUSEMOVE, movx, movy);
                if(movx > 0 && movx <= 0.5 * ws + 50 * T->l && movx >= 0.5 * ws - 50 * T->l)
                {
                    T->lager[0] = (movx - 0.5 * ws + 50 * T->l) / 100;
                    updateScreen(T , bActive, lActive, l3Active, windows);
                    clearmouseclick(WM_LBUTTONUP);
                }
                else delay(30);
            }while (!ismouseclick(WM_LBUTTONUP));
        }
        else delay(100);
    }
    if (T->Flanz)//variable Flächenlast
    {
        for(i = 0; i < T->Flanz; i++)
        {
            if (clix < 0.5 * ws - 50 * T->l + 100 * T->Fl[i].p + 10
             && clix > 0.5 * ws - 50 * T->l + 100 * T->Fl[i].p - 10
             && cliy < 0.5 * wsn + 0.05 * fabs(T->Fl[i].dens1) - 0.05 * foepplh1(0 , T->Fl[i].dens1 ) + 10
             && cliy > 0.5 * wsn - 0.05 * fabs(T->Fl[i].dens1) + 0.05 * foepplh1(T->Fl[i].dens1 , 0) - 10 )
             {
                do
                {
                    getmouseclick(WM_MOUSEMOVE, movx, movy);
                    if(movx > 0 && movx <= 0.5 * ws + 50 * T->l - 100 * T->Fl[i].len && movx >= 0.5 * ws - 50 * T->l)
                    {
                        T->Fl[i].dens1 = (- movy + 15 + 0.5 * wsn) / 0.05;
                        T->Fl[i].p = (movx - 0.5 * ws + 50 * T->l) / 100;
                        updateScreen(T,bActive,lActive,l3Active,windows);
                        clearmouseclick(WM_LBUTTONUP);
                    }
                    else delay(30);
                }while(!ismouseclick(WM_LBUTTONUP));
             }

            else if (clix < 0.5 * ws - 50 * T->l + 100 * T->Fl[i].p + 100 * T->Fl[i].len + 10
             && clix > 0.5 * ws - 50 * T->l + 100 * T->Fl[i].p + 100 * T->Fl[i].len - 10
             && cliy < 0.5 * wsn + 0.05 * fabs(T->Fl[i].dens2) - 0.05 * foepplh1(0 , T->Fl[i].dens2) + 10
             && cliy > 0.5 * wsn - 0.05 * fabs(T->Fl[i].dens2) + 0.05 * foepplh1(T->Fl[i].dens2 , 0) - 10)
             {
                do
                {
                    getmouseclick(WM_MOUSEMOVE, movx, movy);
                    if(movx > 0 && movx <= 0.5 * ws + 50 * T->l && movx >= 0.5 * ws - 50 * T->l + 100 * T->Fl[i].p)
                    {
                        T->Fl[i].dens2 = (- movy + 15 + 0.5 * wsn) / 0.05;
                        T->Fl[i].len = (movx - 0.5 * ws + 50 * T->l - 100 * T->Fl[i].p) / 100;
                        updateScreen(T,bActive,lActive,l3Active,windows);
                        clearmouseclick(WM_LBUTTONUP);
                    }
                    else delay(30);
                }while(!ismouseclick(WM_LBUTTONUP));
             }

            else if
               (clix < 0.5 * ws - 50 * T->l + 100 * T->Fl[i].p + 100 * T->Fl[i].len
             && clix > 0.5 * ws - 50 * T->l + 100 * T->Fl[i].p
             &&(cliy < 0.5 * wsn + 0.05 * fabs(T->Fl[i].dens2) - 0.05 * foepplh1(0 , T->Fl[i].dens2)
             && ((cliy > 0.5 * wsn - 0.05 * fabs(T->Fl[i].dens2) + 0.05 * foepplh1(T->Fl[i].dens2 , 0))
             ||(cliy < 0.5 * wsn + 0.05 * fabs(T->Fl[i].dens1) - 0.05 * foepplh1(0 , T->Fl[i].dens1)))
             && cliy > 0.5 * wsn - 0.05 * fabs(T->Fl[i].dens1) + 0.05 * foepplh1(T->Fl[i].dens1 , 0)) )
             {
                do
                {
                    getmouseclick(WM_MOUSEMOVE, movx, movy);
                    if(movx > 0 && movx <= 0.5 * ws + 50 * T->l - 50 * T->Fl[i].len && movx >= 0.5 * ws - 50 * T->l + 50 * T->Fl[i].len)
                    {
                        T->Fl[i].p = (movx - 0.5 * ws + 50 * T->l - 50 * T->Fl[i].len) / 100;
                        updateScreen(T,bActive,lActive,l3Active,windows);
                        clearmouseclick(WM_LBUTTONUP);
                    }
                    else delay(30);
                }while(!ismouseclick(WM_LBUTTONUP));
             }
        }
    }

    switch(cliy) //Tasten
    {
    case 337 ... 346:      //Löschtasten
            switch(clix)
            {
            case 0 ... 47:
                    if(bActive)
                    {
                    deconstr(T);
                    lActive = 0;
                    l3Active = 0;
                    bActive = 0;
                    }
                    break;
            case 48 ... 90:
                    if (T->Fanz > 0)
                    {
                        T->F[T->Fanz] = 0;
                        T->Fw[T->Fanz] = 0;
                        T->Fp[T->Fanz] = 0;
                        T->Fanz = T->Fanz - 1;
                    }
                    break;
            case 91 ... 213:
                    if (lActive > 0)
                    {
                        T->lager[lActive] = 0;
                        if(lActive == 1) T->lagerF[lActive+1] = 0;
                        T->lagerF[lActive] = 0;
                        lActive = lActive - 1;
                    }
                    break;
            case 214 ... 357:
                    if(l3Active)
                    {
                        T->lager[0] = 0;
                        T->lagerF[0] = 0;
                        T->lagerF[1] = 0;
                        T->lagerMb = 0;
                        T->lagerwert = 0;
                        l3Active = 0;
                    }
                    break;
            case 358 ... 423:
                    if(T->Flanz > 0)
                    {
                        T->Fl[T->Flanz].dens1 = 0;
                        T->Fl[T->Flanz].dens2 = 0;
                        T->Fl[T->Flanz].p = 0;
                        T->Fl[T->Flanz].len = 0;
                        T->Flanz = T->Flanz - 1;
                        if (T->Flanz > 4 && T->Flanz % 4 == 3)
                        {
                        Flmspeicher = Flmspeicher - 16;
                        realloc(T->Fl,  sizeof(double) * Flmspeicher);
                        }

                    }
                    break;
            }
            break;
    case 347 ... 384:      //untere Reihe
            switch(clix)
            {
            case 0 ... 47: // B Taste
                    if (bActive) break;
                    T->l = 2;
                    bActive = 1;
                    break;
            case 48 ... 90: // F Taste
                    if (!bActive) break;
                    T->Fp[0+T->Fanz] = 1 + 0.2 * T->Fanz;
                    T->F[0+T->Fanz] = 1000;
                    T->Fw[0+T->Fanz] = 270;
                    T->Fanz = T->Fanz + 1;
                    if (T->Fanz % 5 == 0)
                    {
                     mspeicher = mspeicher + 5;
                     realloc(T->Fp, mspeicher * sizeof(double));
                     realloc(T->Fw, mspeicher * sizeof(double));
                     realloc(T->F , mspeicher * sizeof(double));
                    }
                    break;

            case 91 ... 213: // Lager Taste
                    if (bActive)
                    {
                        if(l3Active) break;
                        if(lActive == 0)
                            {
                                T->lager[0] = 0;
                                lActive++;
                            }
                        else if(lActive == 1)
                            {
                                T->lager[1] = 1;
                                lActive++;
                            }
                    }
                    break;

            case 214 ... 357: //Lager3 Taste
                if (bActive)
                {
                    if (lActive || l3Active) break;
                    T->lagerwert = 1;
                    T->lager[0] = 0;
                    T->lager[1] = 0;
                    l3Active = 1;
                }
                break;
            case 358 ... 423: //Fl Taste
                if (bActive)
                {
                    T->Fl[T->Flanz].dens1 = 500;
                    T->Fl[T->Flanz].dens2 = 500;
                    T->Fl[T->Flanz].p = 0.8;
                    T->Fl[T->Flanz].len = 0.5;
                    T->Flanz = T->Flanz + 1;
                    if (T->Flanz % 4 == 0)
                    {
                    Flmspeicher = Flmspeicher + 16;
                    realloc(T->Fl,  sizeof(double) * Flmspeicher);
                    }
                }
                break;

            default:
                break;
            }
            break;
    case 0 ... 32:         //obere Reihe
            switch(clix)
            {
            case 0 ... 79:
                if(bActive && ((lActive == 2) || l3Active) && T->Fanz + T->Flanz > 0)
                {
                    berechneLagerF(T);
                }
                break;
            case 80 ... 187:
                if(bActive && ((lActive == 2) || l3Active) && T->Fanz + T->Flanz > 0)
                {
                    BiegemMax(T);
                }
                break;
            case 188 ... 272:
                if(bActive && ((lActive == 2) || l3Active) && T->Fanz + T->Flanz > 0)
                {
                    DurchbMax(T);
                }
                break;
            case 599 ... 640:
                SwitchIy(T);
                break;
            default:
                break;
            }
            break;
    case 33 ... 65: //
            switch(clix)
            {
            case 599 ... 640:
                eingabeBalken(T);
                bActive = 1;
                if ( T->lagerwert == 0)
                {
                    lActive = 2;
                }
                else l3Active = 1;
                break;

            default:
                break;
            }
    default:
            break;
    }

    }while(!stop);
}

void berechneLagerF(Balken* T)
{
    T->lagerF[0] = 0;
    T->lagerF[1] = 0;
    T->lagerF[2] = 0;
    T->lagerMb = 0;
    float spDrei;
    int i;
    if ((T->lagerwert)) //Wenn nur ein Lager existiert vereinfacht sich die Formel
    {
        for ( i = 0; i < T->Fanz ; i++)
        {
            T->lagerF[0] = T->lagerF[0] - sin(bogm(T->Fw[i])) * T->F[i];
            T->lagerF[1] = T->lagerF[1] - cos(bogm(T->Fw[i])) * T->F[i];
            T->lagerMb = T->lagerMb - (T->Fp[i] - T->lager[0]) * sin(bogm(T->Fw[i])) * T->F[i];

            printf("Lagermoment : %f", T->lagerMb);
        }
        for ( i = 0; i < T->Flanz; i++)
        {
            T->lagerF[0] = T->lagerF[0] + T->Fl[i].dens1 * T->Fl[i].len
                                        + (T->Fl[i].dens2 - T->Fl[i].dens1) * 0.5 * T->Fl[i].len;
           if (T->Fl[i].dens1 >= T->Fl[i].dens2)
            {
                double steig = (T->Fl[i].dens2 - T->Fl[i].dens1) / T->Fl[i].len;
                if (steig >= 0)
                        spDrei = 2.0 / 3.0;
                else
                        spDrei = 1.0 / 3.0;
                T->lagerMb = T->lagerMb + T->Fl[i].len * T->Fl[i].dens1 * ((T->Fl[i].p + 0.5 * T->Fl[i].len) - T->lager[0])
                                        + steig * T->Fl[i].len * T->Fl[i].len * 0.5 * (spDrei * T->Fl[i].len + T->Fl[i].p - T->lager[0]);

            printf("Lagermoment : %f", T->lagerMb);
            }
           if ( T->Fl[i].dens1 < T->Fl[i].dens2)
           {
                double steig = (T->Fl[i].dens1 - T->Fl[i].dens2) / T->Fl[i].len;
                if (steig >= 0)
                        spDrei = 2.0 / 3.0;
                else
                        spDrei = 1.0 / 3.0;
                T->lagerMb = T->lagerMb + T->Fl[i].len * T->Fl[i].dens2 * ((T->Fl[i].p + 0.5 * T->Fl[i].len) - T->lager[0])
                                        + steig * T->Fl[i].len * T->Fl[i].len * 0.5 * (spDrei * T->Fl[i].len + T->Fl[i].p - T->lager[0]);

            printf("Lagermoment : %f", T->lagerMb);
           }

        }
        printf("\nLagerkraefte :\nQ1 = %f\nN1 = %f\nM1 = %f\nAnzahl Kraefte = %d\n", T->lagerF[0], T->lagerF[1], T->lagerMb, T->Fanz + T->Flanz);
    }
    else
        {
            for ( i = 0; i < T->Fanz ; i++)
            {
                T->lagerF[2] = T->lagerF[2] - sin(bogm(T->Fw[i])) * T->F[i] * (T->Fp[i] - T->lager[0]) ;
            }
            for ( i = 0; i < T->Flanz ;i++)
            {
            if (T->Fl[i].dens1 >= T->Fl[i].dens2)
            {
                double steig = (T->Fl[i].dens2 - T->Fl[i].dens1) / T->Fl[i].len;
                if (steig >= 0)
                        spDrei = 2.0 / 3.0;
                else
                        spDrei = 1.0 / 3.0;
               T->lagerF[2] = T->lagerF[2] + T->Fl[i].len * T->Fl[i].dens1 * ((T->Fl[i].p + 0.5 * T->Fl[i].len) - T->lager[0])
                                            + steig * T->Fl[i].len * T->Fl[i].len * 0.5 * (spDrei * T->Fl[i].len + T->Fl[i].p - T->lager[0]);

            }
            if ( T->Fl[i].dens1 < T->Fl[i].dens2)
            {
                double steig = (T->Fl[i].dens1 - T->Fl[i].dens2) / T->Fl[i].len;
                if (steig >= 0)
                        spDrei = 2.0 / 3.0;
                else
                        spDrei = 1.0 / 3.0;
                T->lagerF[2] = T->lagerF[2] + T->Fl[i].len * T->Fl[i].dens2 * ((T->Fl[i].p + 0.5 * T->Fl[i].len) - T->lager[0])
                                            + steig * T->Fl[i].len * T->Fl[i].len * 0.5 * (spDrei * T->Fl[i].len + T->Fl[i].p - T->lager[0]);

            }

            }
            T->lagerF[2] = T->lagerF[2] / (T->lager[1] - T->lager[0]);

            for ( i = 0; i < T->Fanz ; i++)
            {
                T->lagerF[0] = T->lagerF[0] - sin(bogm(T->Fw[i])) * T->F[i];
            }
            for ( i = 0; i < T->Flanz ; i++)
            {
                T->lagerF[0] = T->lagerF[0] + T->Fl[i].dens1 * T->Fl[i].len
                                            + 0.5 * (T->Fl[i].dens2 - T->Fl[i].dens1) * T->Fl[i].len;
            }

            T->lagerF[0] = T->lagerF[0] - T->lagerF[2];
            for ( i = 0; i < T->Fanz; i++)
            {
                T->lagerF[1] = T->lagerF[1] - cos(bogm(T->Fw[i])) * T->F[i];
            }
            T->lagerMb = 0;
            printf("\nLagerkraefte : \nQ1 = %f\nN1 = %f\nQ2 = %f\nAnzahl Kraefte = %d",
                            T->lagerF[0],T->lagerF[1],T->lagerF[2], T->Fanz + T->Flanz);
        }
    zeigeQuerkraft(*T);
}

void zeigeQuerkraft(Balken T)
{
    if (!windows[3])windows[3] = initwindow(ws,ws,"Querkraftverlauf",0,0.5*ws);
    setcurrentwindow(windows[3]);
    cleardevice();
    double i;
    double min = 0;
    double max = 0;
    double scaly = 1;
    double scalx = 1;
    char achsenbeschr[10];
    double* Q = (double*)malloc(sizeof(double) * T.l * 1000);

/*    pthread_t t1,t2,t3,t4;
    arg_t arg1,arg2,arg3,arg4;

    arg1.T = T;
    arg2.T = T;
    arg3.T = T;
    arg4.T = T;
    arg1.start = 0;
    arg1.end = 0.25 * T.l;
    arg2.start = 0.25 * T.l;
    arg2.end = 0.5 * T.l;
    arg3.start = 0.5 * T.l;
    arg3.end = 0.75 * T.l;
    arg4.start = 0.75 * T.l;
    arg4.end = T.l;
    arg1.arr = (double*)malloc(sizeof(double) * 0.25 * T.l * 1000);
    arg2.arr = (double*)malloc(sizeof(double) * 0.25 * T.l * 1000);
    arg3.arr = (double*)malloc(sizeof(double) * 0.25 * T.l * 1000);
    arg4.arr = (double*)malloc(sizeof(double) * 0.25 * T.l * 1000);

    clock_t time1,time2;

    time1 = clock();
    pthread_create(&t1, NULL, &threading0, (void*)&arg1);
    pthread_create(&t2, NULL, &threading0, (void*)&arg2);
    pthread_create(&t3, NULL, &threading0, (void*)&arg3);
    pthread_create(&t4, NULL, &threading0, (void*)&arg4);


    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    time2 = clock();

    printf("\nTIME : %f", ((double)(time2-time1))/ CLOCKS_PER_SEC);

    max = arg1.end > arg2.end ? arg1.end : arg2.end; //MAX
    max = max > arg3.end ? max : arg3.end;
    max = max > arg4.end ? max : arg4.end;

    min = arg1.start < arg2.start ? arg1.start : arg2.start; //MIN
    min = min < arg3.start ? min : arg3.start;
    min = min < arg4.start ? min : arg4.start;


    for ( i = 0; i < 0.25 * T.l; i = i + 0.001)
    {
        Q[(int)(i*1000)] = arg1.arr[(int)(i*1000)];
    }

    for ( i = 0.25 * T.l; i <= 0.5 * T.l; i = i + 0.001)
    {
        Q[(int)(i*1000)] = arg2.arr[(int)(i*1000)];
    }

    for ( i = 0.501 * T.l; i <= 0.75 * T.l; i = i + 0.001 )
    {
        Q[(int)(i*1000)] = arg3.arr[(int)(i*1000)];
    }
    printf("\nDEBUG SOL %f", Q[860]);
    delay(1000);

    for ( i = 0.75 * T.l; i <= T.l; i = i + 0.001)
    {
        Q[(int)(i*1000)] = arg4.arr[(int)(i*1000)];
    }*/

    for ( i = 0; i <= T.l ; i = i + 0.001)
    {
        Q[(int)(i*1000)] = floop0(T, i);
        if (Q[(int)(i*1000)] > max) max = Q[(int)(i*1000)];
        if (Q[(int)(i*1000)] < min) min = Q[(int)(i*1000)];
    }



    if (max > 250)
    {
        scaly = 250 / (max);
        if (fabs(min) >  max) scaly = fabs(250 / (min)); //Skalierung der Achsen
    }
    else if (fabs(min) >  250) scaly = fabs(250 / (min));
    if (max < 10 && min > -10)
    {
        scaly = 250 / (max );
        if(fabs(min) > max) scaly = fabs( 250 / min );
    }
    if ( T.l > 5 ) scalx = 5.0 / T.l; //Skalierung der Beschriftung X-Achse für längere Träger
    else if (T.l < 5) scalx = 5.0 / T.l;

    for ( i = 0 ; i <= T.l ; i = i + 0.001) //Graph plotten
    {
        putpixel(0.5 * ws - 250 + i * 100 * scalx, 0.5 * ws - Q[(int)(i*1000)] * scaly, WHITE);//Graph plotten
        //Wenn Graph springt muss eine Linie zwischen die Punkte, da Graph nur aus einzelnen dots besteht
        if (fabs(  Q[(int)((i-0.001)*1000)] - Q[(int)(i*1000)]  ) > 0.01)
            {
            if (i > 4.986 || i < 4.986)
            {
            line(0.5 * ws - 250 + (i-0.001) * 100  * scalx, 0.5 * ws - Q[(int)((i-0.001)*1000)] * scaly, 0.5 * ws - 250 + i * 100 * scalx, 0.5 * ws - Q[(int)(i*1000)] * scaly);
            printf("\t\t\t%f", Q[(int)(i*1000)]);
            }
            }
    }

    setlinestyle(1 , 1 , 1);
    line(0.5 * ws - 250  , 0.5 * ws       ,  0.5 * ws + 250  ,  0.5 * ws       ); //Achsen, gestrichelte Linie
    line(0.5 * ws - 250  , 0.5 * ws - 250 ,  0.5 * ws - 250  ,  0.5 * ws + 250 );
    setlinestyle(0 , 1 , 1);

    free(Q); //Speicher befreien, da T.l * 1000 * 8 byte relativ viel sein kann

    if (max > 0.1) //X-Achsenbeschriftung unter oder über der Achse, je nachdem wie der Graph verläuft
    {
        for ( i = 0; i <= 500; i = i + 50)
        {
            if ( i == 0) continue;
            sprintf(achsenbeschr, "%.0f", i / scalx);
            outtextxy(0.5 * ws - 250 + i - 0.5 * textwidth(achsenbeschr), 0.5 * ws - textheight(achsenbeschr) - 5, achsenbeschr);
        }
    }
    else
    {
        for ( i = 0; i <= 500; i = i + 50)
        {
            if ( i == 0) continue;
            sprintf(achsenbeschr, "%.0f", i / scalx);
            outtextxy(0.5 * ws - 250 + i - 0.5 * textwidth(achsenbeschr)  , 0.5 * ws + textheight(achsenbeschr) - 5, achsenbeschr);
        }
    }
    outtextxy(0.5 * ws + 250 + 20, 0.5 * ws - 10, "in cm" );
    outtextxy(0.5 * ws - 250 - 0.5 * textwidth("in mm"), 0.5 * ws - 250 - 30, "in N");

    for (i = 0; i <= 250; i = i + 50) // Beschriftung Y-Achse+
    {
        sprintf(achsenbeschr, "%.2f", - i / scaly);
        outtextxy(0.5 * ws - 250 - textwidth(achsenbeschr) - 5, 0.5 * ws + i - 0.5 * textheight(achsenbeschr), achsenbeschr);
    }
    for ( i = 0; i <= 250 ; i = i + 50) //Beschrftung Y-Achse-
    {
        sprintf(achsenbeschr, "%.2f",   i / scaly);
        outtextxy(0.5 * ws - 250 - textwidth(achsenbeschr) - 5, 0.5 * ws - i - 0.5 * textheight(achsenbeschr), achsenbeschr);
    }
    swapbuffers();


}

void BiegemMax(Balken* T)
{
    double i;
    double dummy = 0;
    double max = 0;
    for ( i = 0; i <= T->l; i = i + 0.01)
        {
        dummy = floop1(*T, i);
        if (fabs(dummy) > fabs(max))
            {
            max = dummy;
            T->MbMax[1] = i;
            }
        }
    T->MbMax[0] = max;
    printf("\nBetrag Mb-Max : %fNm\nOrt Mb-Max : %fm\n", T->MbMax[0], T->MbMax[1]);
    MaxSigma(T);
    zeigeBiegemoment(*T);
}

void zeigeBiegemoment(Balken T)
{
    if(!windows[0]) windows[0] = initwindow(ws , ws , "Biegemomentverlauf", 0, 0);
    setcurrentwindow(windows[0]);
    cleardevice();
    double i;
    double min = 0;
    double max = 0;
    double scaly = 1;
    double scalx = 1;
    char Achsebeschr[10];
    double* Mbx = (double*)malloc(sizeof(double) * T.l * 10000);

    for ( i = 0; i <= T.l; i = i + 0.001)
    {
        Mbx[(int)(i*1000)] = floop1(T,i);
        if (Mbx[(int)(i*1000)] > max) max = Mbx[(int)(i*1000)];
        if (Mbx[(int)(i*1000)] < min) min = Mbx[(int)(i*1000)];
    }
    if (max > 250)
    {
        scaly = 250 / (max);
        if (fabs(min) >  max) scaly = fabs(250 / (min)); //Skalierung der Achsen
    }
    else if (fabs(min) >  250) scaly = fabs(250 / (min));
    if (max < 10 && min > -10)
    {
        scaly = 250 / (max );
        if(fabs(min) > max) scaly = fabs( 250 / min );
    }
    if ( T.l > 5 ) scalx = 5.0 / T.l;                    //Skalierung der Beschriftung X-Achse für längere Träger
    else if (T.l < 5) scalx = 5.0 / T.l;
    for ( i = 0; i < T.l; i = i + 0.001)
    {
    putpixel(0.5 * ws - 250 + i * 100 * scalx , 0.5 * ws - Mbx[(int)(i*1000)] * scaly , WHITE);//erst muss scal bekannt sein, deswegen zweite Schleife
    if (fabs(fabs(Mbx[(int)(i*1000)]) - fabs(Mbx[(int)((i-0.001)*1000)])) > 0.1)
    {
        setlinestyle(0,0,1);
        line(0.5 * ws - 250 + i * 100 * scalx, 0.5 * ws - Mbx[(int)(i*1000)] * scaly, 0.5 * ws - 250 + (i-0.001) * 100 * scalx, 0.5 * ws - Mbx[(int)((i-0.001)*1000)] *  scaly);
    }
    }

    free(Mbx); //Speicher befreien

    setlinestyle(1,1,1);
    line(0.5 * ws - 250  , 0.5 * ws       ,  0.5 * ws + 250  ,  0.5 * ws       ); //Achsen, gestrichelte Linie
    line(0.5 * ws - 250  , 0.5 * ws - 250 ,  0.5 * ws - 250  ,  0.5 * ws + 250 );

    for ( i = 0; i <= 5 ; i = i + 0.5) //Achsenbeschriftung (X-Achse)
    {
        if (i == 0) continue;
            sprintf(Achsebeschr, "%.0f", i * 100.0 / scalx );
        if (max > fabs(min))
            {
            outtextxy(0.5 * ws - 250 + i * 100 - 0.5 * (double)textwidth(Achsebeschr) , 0.5 * ws + 10 , Achsebeschr);
            }
        else
            {
            outtextxy(0.5 * ws - 250 + i * 100 - 0.5 * (double)textwidth(Achsebeschr) , 0.5 * ws - 20 , Achsebeschr); //
            }

    }

    for ( i = 0; i <= 2.5 ; i = i + 0.5) //Achsenbeschriftung generieren (Y-Achse+)
        {
        sprintf(Achsebeschr, "%.1f", i * (100 / scaly));
        outtextxy(0.5 * ws - 250 - 10 - textwidth(Achsebeschr), 0.5 * ws - i * 100 - 0.5 * textheight(Achsebeschr), Achsebeschr);
        }

    for ( i = 0; i >= -2.5 ; i = i - 0.5) //Achsenbeschriftung generieren (Y-Achse-)
    {
        sprintf(Achsebeschr, "%.1f", i * (100 / scaly));
        outtextxy(0.5 * ws - 250 - 10 - textwidth(Achsebeschr), 0.5 * ws - i * 100 - 0.5 * textheight(Achsebeschr), Achsebeschr);
    }


    outtextxy(0.5 * ws - 250 - 0.5 * textwidth("in Nm"), 0.5 * ws - 250 - 30, "in Nm");
    outtextxy(0.5 * ws + 250 + 20, 0.5 * ws - 10, "in cm");   //Einheitenbeschriftung am Ende der Achsen
    swapbuffers();

}

void DurchbMax(Balken* T)
{

    double i = 0;
    double dummy = 0;
    double max = 0;
    berechneKonst(T);

    for ( i = 0; i <= T->l + 0.01; i = i + 0.01)
    {
        dummy = floop3(*T, i) + T->C1 * i + T->C2;
        if ( fabs(dummy) > fabs(max) )
        {
            max = dummy;
            T->wmax[1] = i;
            T->wmax[0] = max;
        }
    }
    T->wmax[0] = (T->wmax[0] * 1000000000) / (T->E * T->FTM); //von m in mm
    printf("\nMaximale Durchbiegung w(x = %fm) = %f mm ", T->wmax[1], -T->wmax[0]);
    zeigeBiegung(*T);
}

void zeigeBiegung(Balken T)
{
    if(!windows[1]) windows[1] = initwindow(ws,ws,"Biegungsverlauf", ws , 0);
    setcurrentwindow(windows[1]);
    cleardevice();
    double i;
    double max = 0;
    double min = 0;
    double* wx = (double*)malloc(sizeof(double) * 10000 * T.l); //Max 10m
    double scaly = 1;
    double scalx = 1;
    char achsenbeschr[10];

    setlinestyle(1,1,1);
    line(0.5 * ws - 250  , 0.5 * ws       ,  0.5 * ws + 250  ,  0.5 * ws       ); //Achsen, gestrichelte Linie
    line(0.5 * ws - 250  , 0.5 * ws - 250 ,  0.5 * ws - 250  ,  0.5 * ws + 250 );
    setlinestyle(0,1,1);

    for ( i = 0; i <= T.l+ 0.001; i = i + 0.001) //Biegung ausrechnen und in Array speichern
    {

        wx[(int)(i*1000)] = (floop3(T,i) + T.C1 * i + T.C2) * 1000000000 / (T.E * T.FTM);

        if (max < wx[(int)(i*1000)]) max = wx[(int)(i*1000)];
        if (min > wx[(int)(i*1000)]) min = wx[(int)(i*1000)];

    }

    if (max  > 250)
    {
        scaly = 250 / (max);
        if (fabs(min) >  max) scaly = fabs(250 / (min)); //Skalierung der Achsen
    }

    if ( T.l > 5 ) scalx = 5.0 / T.l;
    else if (T.l < 5) scalx = 5.0 / T.l;

    if (max < 10 && min > -10)
    {
        scaly = 250 / (max );
        if(fabs(min) > max) scaly = fabs( 250 / min );
    }

    {
        scaly = 250 / (max );
        if(fabs(min) > max) scaly = fabs( 250 / min );
    }

    for ( i = 0 ; i < T.l ; i = i + 0.001) //Graph plotten
    {
        putpixel(0.5 * ws - 250 + i * 100 * scalx, 0.5 * ws - wx[(int)(i*1000)] * scaly, WHITE); //Graph plotten
    }
    free(wx);
    if (max > 0.1) //X-Achsenbeschriftung unter oder über der Achse, je nachdem wie der Graph verläuft
    {
        for ( i = 0; i <= 500; i = i + 50)
        {
            if ( i == 0) continue;
            sprintf(achsenbeschr, "%.0f", i / scalx);
            outtextxy(0.5 * ws - 250 + i - 0.5 * textwidth(achsenbeschr), 0.5 * ws + 0.5 * textheight(achsenbeschr), achsenbeschr);
        }
    }
    else
    {
        for ( i = 0; i <= 500; i = i + 50)
        {
            if ( i == 0) continue;
            sprintf(achsenbeschr, "%.0f", i / scalx);
            outtextxy(0.5 * ws - 250 + i - 0.5 * textwidth(achsenbeschr)  , 0.5 * ws - textheight(achsenbeschr) - 5, achsenbeschr);
        }
    }

    outtextxy(0.5 * ws + 250 + 20, 0.5 * ws - 10, "in cm" );
    outtextxy(0.5 * ws - 250 - 0.5 * textwidth("in mm"), 0.5 * ws - 250 - 30, "in mm");

    for (i = 0; i <= 250; i = i + 50) // Beschriftung Y-Achse+
    {
        sprintf(achsenbeschr, "%.3f", i / scaly);
        outtextxy(0.5 * ws - 250 - textwidth(achsenbeschr) - 5, 0.5 * ws + i - 0.5 * textheight(achsenbeschr), achsenbeschr);
    }
    for ( i = 0; i <= 250 ; i = i + 50) //Beschrftung Y-Achse-
    {
        sprintf(achsenbeschr, "%.3f",  -i / scaly);
        outtextxy(0.5 * ws - 250 - textwidth(achsenbeschr) - 5, 0.5 * ws - i - 0.5 * textheight(achsenbeschr), achsenbeschr);
    }
    swapbuffers();

}

void eingabeBalken(Balken* T)
{
    int i;
    printf("Laenge eingeben (in m) : ");
    scanf("%lf", &T->l);
    printf("Anzahl der bekannten Punktkraefte eingeben : ");
    scanf("%i", &T->Fanz);
    T->F  = (double*)malloc(sizeof(double) * T->Fanz);
    T->Fw = (double*)malloc(sizeof(double) * T->Fanz);
    T->Fp = (double*)malloc(sizeof(double) * T->Fanz);
    for ( i = 0; i < T->Fanz; i++ )
    {
        printf("Betrag von Kraft %i (in N) : ",i+1);
        scanf("%lf", T->F+i);
        printf("Position von Kraft %i (in m) : ",i+1);
        scanf("%lf", T->Fp+i);
        printf("Winkel von Kraft %i in Grad ( 270 entspricht senkrecht nach unten ) : ", i+1);
        scanf("%lf", T->Fw+i);
    }
    printf("Anzahl der bekannten Flaechenlasten eingeben : ");
    scanf("%i", &T->Flanz);
    T->Fl = (Flaechenlast*)malloc(sizeof(double) * 4 * T->Flanz);
    for (i = 0; i < T->Flanz; i++)
    {
        printf("Startposition von Flaechelast %i eingeben : ", i+1);
        scanf("%lf", &T->Fl[i].p);
        printf("Laenge von Flaechenlast %i eingeben : ", i+1);
        scanf("%lf", &T->Fl[i].len);
        printf("N/m an Startposition eingeben : ");
        scanf("%lf", &T->Fl[i].dens1);
        printf("N/m an Endposition eingeben : ");
        scanf("%lf", &T->Fl[i].dens2);
    }
    printf("3-wertiges Lager vorhanden ? (0 oder 1) : ");
    scanf("%lf", &T->lagerwert);
    printf("Lager 1 an Stelle : ");
    scanf("%lf", T->lager);
    if (T->lagerwert != 1)
    {
        printf("Lager 2 an Stelle : ");
        scanf("%lf", T->lager+1);
    }
    else
    {
        T->lager[1] = 0;
    }
    printf("E-Modul : ");
    scanf("%lf", &T->E);
    SwitchIy(T);
}

void MaxSigma(Balken* T)
{
    char sqr = 253;
    T->Wy = T->FTM / T->zMax;
    T->sigmaMax = T->MbMax[0] /  T->Wy; // Nm*1000 / mm³
    printf("\nMaximale Spannung SigmaMax(x = %fm) = %f N/mm%c", T->MbMax[1], T->sigmaMax * 1000, sqr);
}


#endif
