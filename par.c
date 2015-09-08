#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "xglib.h"
#include "matriz.h"

#define AREA     700
#define FONDO    SetColor (255, 255, 255)
#define PAR      SetColor (0, 0, 0)
#define FIN      1000

 matriz *psM;
 dword  DT;

 int main ()
 {
   displayX *psdX;
   windowX *psw1;
   int x;
   dword y;
   psdX = psDXIniciaXGraph (NULL);
   psw1 = pswXCreaVentana (psdX, "Area", 0, 0, AREA, AREA, 0, 0, FONDO, 0, 0);
   psM = psmCreaMatriz (500);
   vCreaAleatoria (psM, AREA, AREA);
   for (y=0; y<FIN; y++)
   {
     for (x=0; x<(psM->iNodos); x++)
     {
       vColorForeground (psdX, psw1, FONDO);
       XFillRectangle (ObtenDisplay(psdX), ObtenBufferV(psw1), ObtenGCV(psw1), psM->psnNodos[x].iX-2, psM->psnNodos[x].iY-2, 4, 4);
       psM->psnNodos[x].iX = psM->psnNodos[x].iX + (int)(5 * cos (psM->psnNodos[x].fA)); 
       psM->psnNodos[x].iY = psM->psnNodos[x].iY + (int)(5 * sin (psM->psnNodos[x].fA));
       if (psM->psnNodos[x].iY>AREA) psM->psnNodos[x].fA = psM->psnNodos[x].fA * -1;
       if (psM->psnNodos[x].iY<0) psM->psnNodos[x].fA = psM->psnNodos[x].fA * -1;
       if (psM->psnNodos[x].iX>AREA) psM->psnNodos[x].fA = PI - psM->psnNodos[x].fA;
       if (psM->psnNodos[x].iX<0) psM->psnNodos[x].fA = PI - psM->psnNodos[x].fA;
       vColorForeground (psdX, psw1, PAR);
       XFillRectangle (ObtenDisplay(psdX), ObtenBufferV(psw1), ObtenGCV(psw1), psM->psnNodos[x].iX-2, psM->psnNodos[x].iY-2, 4, 4);
       if (iObtenKey(psdX)==KEY_ESC) y = FIN;
       vReDrawVentanas (psdX);
     }
   }
   vRun (psdX);
   vTerminaXGraph (psdX);
   return (0);
 }
 
 void vReDraw (int iV)
 { /*Se declara, pero no hace nada*/ } 