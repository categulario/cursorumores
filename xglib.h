/*Funciones para el manejo de las bibliotecas de XLIB                    */
/*                                                                       */
/*Las funciones son muy primitivas, en general solo se explica un poco   */
/*lo que hay y de ahí se deben tomar las funciones.                      */
/*                                                                       */
/*Solo se tiene lo básico para el manejo de ventanas con X11.            */
/*                                                                       */
/*                                                                       */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "lib.h"

#ifndef CLASS_XGLIB
    #define CLASS_XGLIB

    typedef struct DISPLAYX displayX;
    typedef struct WINDOWX  windowX;
    typedef unsigned long color;
    typedef unsigned int  keycode;

    #define ObtenDisplay( psDDDD ) (psDDDD->psDDisplay)
    #define ObtenPantalla( psDDDD ) (psDDDD->iPantallaD)

    #define ObtenVentanaV( psDDDD ) (psDDDD->swW)
    #define ObtenAtributosV( psDDDD ) (psDDDD->swaWA)
    #define ObtenBufferV( psDDDD ) (psDDDD->pixmP)
    #define ObtenCFondoV( psDDDD ) (psDDDD->cFondo)
    #define ObtenGCV( psDDDD ) (psDDDD->gc)
    #define SetColor( rojo, verde, azul ) ((color)(rojo<<16)|(color)(verde<<8)|(color)(azul))
    #define ObtenRojo( ccolor ) ((ccolor>>16)&0xFF)
    #define ObtenVerde( ccolor ) ((ccolor>>8)&0xFF)
    #define ObtenAzul( ccolor ) (ccolor&0xFF)
    #define CROJO  255<<16
    #define CVERDE 255<<8
    #define CAZUL  255

    /*Para dibujar puntos en la grafica*/
    #define xP_UNIR    1         /*Une los puntos*/
    #define xP_PUNTO   2         /*Si se unen puntos, coloca el punto*/

    /*para las banderas de uso unterno*/
    #define W_XY       1         /*Se ha actualizado el punto anterior*/

    struct DISPLAYX {
        Display *psDDisplay;      /*El manejador del display*/
        int     iHijos;           /*Número de hijos*/
        int     iContID;          /*Contador de indentificador*/
        char    *pcNombreDisp;    /*Nombre del display. Generalmente es NULL*/
        int     iPantallaD;       /*Pantalla default del display creado*/
        windowX *pswXIni;         /*Lista de ventanas*/
        windowX *pswXFin;         /*Lista de ventanas*/
    };

    struct WINDOWX {
        int iId;                  /*Idenitificador de la ventana*/
        Window swW;               /*Manejador de la ventana*/
        XWindowAttributes swaWA;  /*Atributos de la ventana*/
        int iX, iY;               /*Posición de la ventana*/
        int iCx, iCy;             /*Centro de la ventana*/
        int iXo, iYo;             /*Condicion inicial*/
        word wW, wH;              /*Ancho y alto de la ventana*/
        word wBorde;              /*Borde de la ventana*/
        long lMask;               /*Mascara*/
        word wFlags;              /*Banderas, uso interno*/
        color cFondo;             /*Color de fondo*/
        GC   gc;                  /*GC*/
        Pixmap pixmP;             /*Buffer*/
        windowX *pswXSig;         /*Siguiente ventana*/
        windowX *pswXAnt;         /*Anterior ventana*/
    };


    #define KEY_ESC      0xFF1B
#endif

/*Crea un display con ventana madre con colores default*/
displayX *psDXIniciaXGraph (char *pcNombreDisp);
/*Crea una nueva ventana, regresa su identificador*/
windowX *pswXCreaVentana (displayX *psdXA, char *pcTitulo, int iX, int iY, word wW, word wH, word wBorde, long lMask, color cColm, int iCX, int iCY);
/*Agrega la ventana a la lista*/
void vAgregaVentana (displayX *psdXA, windowX *pswXA);
/*Cierra la conexión con el display*/
void vTerminaXGraph (displayX *psdX);
/*Regresa el manejador de la ventana*/
windowX *pswXObtenVentana (displayX *psdX, int iVentana);
/*Mantiene todo hasta que se presiona el ratón o tecla*/
void vRun (displayX *psDX);
/*Fija el color de primer plano (frente)*/
void vColorForeground (displayX *psdX, windowX *pswXA, color cCol);
/*Actualiza mascara de eventos*/
void vEventos (displayX *psdX, long lMask);
/*Retorna el primer tipo de evento*/
int iObtenEvento (displayX *psdX);
/*Retorna la tecla presionada*/
int iObtenKey (displayX *psdX);
/*Fija el color de segundo plano (fondo)*/
void vColorBackground (displayX *psdX, windowX *pswXA, color cCol);
/*Actualiza todas la ventanas*/
void vReDrawVentanas (displayX *psdX);
/*Imprime texto*/
void vImprimeTexto (displayX *psdX, windowX *pswXA, int iX, int iY, char *pTexto, color cCol);

/*Estas funciones son para presentar graficas*/
/*Coloca un punto en la ventana de acuerdo al origen de coordenadas*/
void vColocaPunto (displayX *psdX, windowX *pswXA, int iX, int iY, color cCol, int iP, word wFlags);
/*Dibuja los ejes*/
void vDibujaEjes (displayX *psdX, windowX *pswXA, color cCol);
/*Reinicia la colocación de los puntos*/
void vResetPunto (windowX *pswXA);
/*Coloca el origen de coordenadas*/
void vDibujaOrigen (displayX *psdX, windowX *pswXA, color cCol, int iLen);
/*Coloca las divisiones en eje x*/
void vColocaDivX (displayX *psdX, windowX *pswXA, color cCol, int iLen, double fD, int iMod, dword dTotal, int iBar, color cColBars);
/*Coloca las divisiones en eje y*/
void vColocaDivY (displayX *psdX, windowX *pswXA, color cCol, int iLen, double fD, int iMod, dword dTotal, int iBar, color cColBars);

/*Debe existir*/
void vReDraw (int iVentana);
