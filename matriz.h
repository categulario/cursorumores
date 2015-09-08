
#ifndef MATRIZ_LIB
 #define MATRIZ_LIB  1


 typedef struct MATRIZ matriz;
 typedef struct NODO nodo;
 typedef struct ENLACE enlace;
 
 struct MATRIZ
 {
   int iNodos;
   nodo *psnNodos;
 };
 
 struct NODO
 {
   int iId;            /*Identificador del nodo*/
   int iEnlaces;       /*Número de enlaces*/
   int iX, iY;         /*Posición del nodo*/
   double fA;          /*Angulo*/
   enlace *pseIni;     /*Inicio de la lista doblemente ligada de enlaces*/
   enlace *pseFin;     /*Fin de la lista doblemente ligada de enlaces*/
 };
 
 struct ENLACE
 {
   int iNodo;    /*Nodo enlazado*/
   double fW;          /*Peso del enlace*/
   enlace *pseAnt;     /*Enlace anterior*/
   enlace *pseSig;     /*Enlace siguiente*/  
 };
#endif 

 matriz *psmCreaMatriz (int iNodos);
 char cHayEnlace (matriz *psmM, int i, int j);
 char cCreaEnlace (matriz *psmM, int i, int j, double fW);
 char cEsSimetrica (matriz *psmM);
 void vImprimeEnlaces (matriz *psmM);
 void vCreaFreeScale (matriz *psmM);
 void vCreaAleatoria (matriz *psmM, int iX, int iY);
 