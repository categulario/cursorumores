#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matriz.h"
#include "lib.h"


 matriz *psmCreaMatriz (int iNodos)
 {
   int x;
   nodo *psnRet;
   matriz *psmM;
   psmM = (matriz *)malloc (sizeof(matriz));
   if (!psmM) return (0);
   psnRet = (nodo *)malloc (sizeof(nodo)*iNodos);
   if (!psnRet) return (0);
   for (x=0; x<iNodos; x++)
   {
     psnRet[x].iId = x;
     psnRet[x].iEnlaces = 0;
     psnRet[x].iX = 0;
     psnRet[x].iY = 0;
     psnRet[x].fA = 0;
     psnRet[x].pseIni = 0;
     psnRet[x].pseFin = 0;
   }
   psmM->psnNodos = psnRet;
   psmM->iNodos = iNodos;
   return (psmM);
 }
 
 char cHayEnlace (matriz *psmM, int i, int j)
 {
   enlace *pseEi, *pseEj;
   char cRet=0;
   pseEi = psmM->psnNodos[i].pseIni;
   while (pseEi)
   {
     if (pseEi->iNodo==j) cRet = cRet | 1;
     pseEi = pseEi->pseSig;
   }
   pseEj = psmM->psnNodos[j].pseIni;
   while (pseEj)
   {
     if (pseEj->iNodo==i) cRet = cRet | 2;
     pseEj = pseEj->pseSig;
   }
   return (cRet);
 }
 
 char cCreaEnlace (matriz *psmM, int i, int j, double fW)
 {
   enlace *pseEi, *pseEj;
   if (cHayEnlace(psmM, i, j)) return (0);
   pseEi = (enlace *)malloc (sizeof(enlace));
   pseEj = (enlace *)malloc (sizeof(enlace));
   if ((!pseEi)||(!pseEj)) return (0);
   pseEi->iNodo = j;
   pseEi->fW = fW;
   pseEi->pseAnt = 0;
   pseEi->pseSig = 0;
   pseEj->iNodo = i;
   pseEj->fW = fW;
   pseEj->pseAnt = 0;
   pseEj->pseSig = 0;
   if (!(psmM->psnNodos[i].pseIni)) psmM->psnNodos[i].pseIni = pseEi;
   else psmM->psnNodos[i].pseFin->pseSig = pseEi; 
   pseEi->pseAnt = psmM->psnNodos[i].pseFin;
   psmM->psnNodos[i].pseFin = pseEi;
   if (!(psmM->psnNodos[j].pseIni)) psmM->psnNodos[j].pseIni = pseEj;
   else psmM->psnNodos[j].pseFin->pseSig = pseEj; 
   pseEj->pseAnt = psmM->psnNodos[j].pseFin;
   psmM->psnNodos[j].pseFin = pseEj;
   (psmM->psnNodos[i].iEnlaces)++;
   (psmM->psnNodos[j].iEnlaces)++;
   return (1);
 }
 
 char cEsSimetrica (matriz *psmM)
 {
   char cRet=1;
   enlace *pseE;
   int x;
   for (x=0; x<(psmM->iNodos); x++)
   {
     pseE = psmM->psnNodos[x].pseIni;
     while (pseE)
     {
       if (cHayEnlace(psmM, x, (pseE->iNodo))!=3) cRet = 0; 
       pseE = pseE->pseSig;
     }
   }
   return (cRet);
 }
 
 void vImprimeEnlaces (matriz *psmM)
 {
   int x, y;
   enlace *pseE;
   for (x=0; x<(psmM->iNodos); x++)
   {
     pseE = psmM->psnNodos[x].pseIni;  y = 0;
     while (pseE)
     {
       y++;
       pseE = pseE->pseSig;
     }
     printf ("Nodo: %d con %d enlaces.\n", x, y);
   }
 }
 
 void vCreaFreeScale (matriz *psmM)
 {

 }
 
 void vCreaAleatoria (matriz *psmM, int iX, int iY)
 {

 }
 