#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "xglib.h"

displayX *psDXIniciaXGraph (char *pcNombreDisp) {
    /*Crea un display con ventana madre con colores default*/
    displayX *psdXA;
    psdXA = (displayX *)malloc (sizeof(displayX));
    if (!psdXA) {
        return (0);
    }
    psdXA->pcNombreDisp = pcNombreDisp;
    psdXA->psDDisplay = XOpenDisplay (pcNombreDisp);
    if  (!(psdXA->psDDisplay)) return (0);
    psdXA->iPantallaD = DefaultScreen (psdXA->psDDisplay);
    psdXA->pswXIni = 0;
    psdXA->pswXFin = 0;
    psdXA->iHijos = 0;
    psdXA->iContID = 0;
    XFlush (ObtenDisplay(psdXA));
    return (psdXA);
}

windowX *pswXCreaVentana (displayX *psdXA, char *pcTitulo, int iX, int iY, word wW, word wH, word wBorde, long lMask, color cCol, int iCX, int iCY) {
    /*Crea una nueva ventana, regresa su identificador*/
    windowX *pswXA;
    Pixmap pixmP;
    dword dBlanco, dNegro;
    pswXA = (windowX *)malloc (sizeof(windowX));
    if (!pswXA) {
        return (0);
    }
    dBlanco = WhitePixel (psdXA->psDDisplay, psdXA->iPantallaD);
    dNegro = BlackPixel (psdXA->psDDisplay, psdXA->iPantallaD);
    pswXA->swW = XCreateSimpleWindow (psdXA->psDDisplay, DefaultRootWindow(psdXA->psDDisplay), iX, iY, wW, wH, wBorde, dNegro, dBlanco);
    pswXA->iX = iX;  pswXA->iY = iY;  pswXA->wBorde = wBorde;
    pswXA->wW = wW;  pswXA->wH = wH;  pswXA->cFondo = cCol;
    XGetWindowAttributes(ObtenDisplay(psdXA), pswXA->swW, &(pswXA->swaWA));
    XFlush (ObtenDisplay(psdXA));
    XMapWindow (ObtenDisplay(psdXA), pswXA->swW);
    XSetWindowBackground (ObtenDisplay(psdXA), ObtenVentanaV(pswXA), cCol);
    if (!lMask) {
        lMask = KeyPressMask;
    }
    XSelectInput (ObtenDisplay(psdXA), pswXA->swW, lMask);
    XStoreName (ObtenDisplay(psdXA), pswXA->swW, pcTitulo);
    psdXA->iHijos++;  psdXA->iContID++;
    pswXA->iId = psdXA->iContID;
    pswXA->lMask = lMask;
    vAgregaVentana (psdXA, pswXA);
    pixmP = XCreatePixmap (ObtenDisplay(psdXA), ObtenVentanaV(pswXA), ObtenAtributosV(pswXA).width, ObtenAtributosV(pswXA).height, ObtenAtributosV(pswXA).depth);
    pswXA->pixmP = pixmP;
    pswXA->gc = XCreateGC (ObtenDisplay(psdXA), ObtenBufferV(pswXA), 0, NULL);
    XCopyArea (ObtenDisplay(psdXA), ObtenVentanaV(pswXA), pixmP, ObtenGCV(pswXA), 0, 0, ObtenAtributosV(pswXA).width, ObtenAtributosV(pswXA).height,  0, 0);
    XMoveWindow (ObtenDisplay(psdXA), ObtenVentanaV(pswXA), iX, iY);
    pswXA->iCx = iCX;   /*Fija el origen de coordenadas*/
    pswXA->iCy = iCY;
    pswXA->iXo = 0;
    pswXA->iYo = 0;
    pswXA->wFlags = 0;
    vColorForeground (psdXA, pswXA, cCol);
    XFillRectangle (ObtenDisplay(psdXA), ObtenBufferV(pswXA), ObtenGCV(pswXA), 0, 0, ObtenAtributosV(pswXA).width, ObtenAtributosV(pswXA).height);
    XFlush (ObtenDisplay(psdXA));
    return pswXA;
}

void vAgregaVentana (displayX *psdXA, windowX *pswXA) {
    pswXA->pswXAnt = psdXA->pswXFin;
    pswXA->pswXSig = 0;
    if (!(psdXA->pswXFin)) {
        psdXA->pswXIni = pswXA;
    } else {
        psdXA->pswXFin->pswXSig = pswXA;
    }
    psdXA->pswXFin = pswXA;
}

void vTerminaXGraph (displayX *psdX) {
    /*Cierra la conexión con el display*/
    windowX *pswX, *pswAux;
    pswX = psdX->pswXIni;
    while (pswX) {
        pswAux = pswX;
        pswX = pswX->pswXSig;
        XFreePixmap (psdX->psDDisplay, ObtenBufferV(pswAux));
        XFreeGC (psdX->psDDisplay, ObtenGCV(pswAux));
        XDestroyWindow (psdX->psDDisplay, ObtenVentanaV(pswAux));
        free (pswAux);
    }
    XCloseDisplay (psdX->psDDisplay);
    free (psdX);
}

windowX *pswXObtenVentana (displayX *psdX, int iVentana) {
    /*Regresa el manejador de la ventana*/
    windowX *pswX;
    pswX = psdX->pswXIni;
    while (pswX) {
        if (pswX->iId==iVentana) {
            return (pswX);
        } else {
            pswX = pswX->pswXSig;
        }
    }
    return 0;
}

void vRun (displayX *psdX){
    /*Mantiene todo hasta que se presiona el ratón o tecla*/
    byte bSalir;
    XEvent sXE;
    windowX *pswX;
    bSalir = FALSE;
    vReDrawVentanas (psdX);
    while (!bSalir) {
        pswX = psdX->pswXIni;
        while (pswX) {
            if (XCheckWindowEvent(ObtenDisplay(psdX), ObtenVentanaV(pswX), pswX->lMask, &sXE)) {
                switch (sXE.type) {
                    case Expose:
                        if (ObtenBufferV(pswX)) {
                            XCopyArea (ObtenDisplay(psdX), ObtenBufferV(pswX), ObtenVentanaV(pswX), ObtenGCV(pswX), 0, 0, ObtenAtributosV(pswX).width, ObtenAtributosV(pswX).height, 0, 0);
                            XFlush (ObtenDisplay(psdX));
                        }
                        if (sXE.xexpose.count==0) {
                            vReDraw (pswX->iId);
                        }
                        break;
                    case ButtonPress:
                    case ButtonRelease:
                        //bSalir = TRUE;
                        break;
                    case KeyPress:
                        bSalir = TRUE;
                        break;
                    default:
                        //usleep (10);  /*Evita uso excesivo del procesador*/
                        break;
                }
            }
            pswX = pswX->pswXSig;
        }
    }
}

void vEventos (displayX *psdX, long lMask) {
    /*Actualiza mascara de eventos*/
    windowX *pswX;
    pswX = psdX->pswXIni;
    while (pswX) {
        XSelectInput (ObtenDisplay(psdX), pswX->swW, lMask);
        pswX->lMask = lMask;
        pswX = pswX->pswXSig;
    }
    XFlush (ObtenDisplay(psdX));
}

int iObtenEvento (displayX *psdX) {
    /*Retorna el primer tipo de evento*/
    XEvent sXE;
    windowX *pswX;
    pswX = psdX->pswXIni;
    while (pswX) {
        if (XCheckWindowEvent(ObtenDisplay(psdX), ObtenVentanaV(pswX), pswX->lMask, &sXE)) {
            return sXE.type;
        }
        pswX = pswX->pswXSig;
    }
    return 0;
}

int iObtenKey (displayX *psdX) {
    /*Retorna la tecla presionada en todas las ventanas*/
    XEvent sXE;
    int iKey;
    windowX *pswX;
    pswX = psdX->pswXIni;
    while (pswX) {
        if (XCheckWindowEvent(ObtenDisplay(psdX), ObtenVentanaV(pswX), pswX->lMask, &sXE)) {
            if (sXE.type==KeyPress) {
                iKey = XLookupKeysym((XKeyEvent *)&sXE, 0);
                return iKey;
            }
        }
        pswX = pswX->pswXSig;
    }
    return 0;
}

void vColorForeground (displayX *psdX, windowX *pswXA, color cCol) {
    /*Fija el color de primer plano (frente)*/
    XSetForeground (ObtenDisplay(psdX), ObtenGCV(pswXA), cCol);
}

void vColorBackground (displayX *psdX, windowX *pswXA, color cCol) {
    /*Fija el color de segundo plano (fondo)*/
    XSetBackground (ObtenDisplay(psdX), ObtenGCV(pswXA), cCol);
}

void vReDrawVentanas (displayX *psdX) {
    /*Actualiza todas la ventanas*/
    windowX *pswX;
    pswX = psdX->pswXIni;
    while (pswX) {
        XCopyArea (ObtenDisplay(psdX), ObtenBufferV(pswX), ObtenVentanaV(pswX), ObtenGCV(pswX), 0, 0, ObtenAtributosV(pswX).width, ObtenAtributosV(pswX).height, 0, 0);
        XFlush (ObtenDisplay(psdX));
        pswX = pswX->pswXSig;
    }
}

void vImprimeTexto (displayX *psdX, windowX *pswXA, int iX, int iY, char *pTexto, color cCol) {
    int iLen;
    iLen = strlen (pTexto);
    vColorForeground (psdX, pswXA, cCol);
    XDrawString (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), iX, iY, pTexto, iLen);
    vReDrawVentanas (psdX);
}

void vColocaPunto (displayX *psdX, windowX *pswXA, int iX, int iY, color cCol, int iP, word wFlags)
{
    iX = pswXA->iCx + iX;
    iY = pswXA->iCy - iY;
    vColorForeground (psdX, pswXA, cCol);
    if ((pswXA->wFlags&W_XY)&&(wFlags&xP_UNIR)) {
        XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), pswXA->iXo, pswXA->iYo, iX, iY);
    }
    if (wFlags&xP_PUNTO) {
        if (iP>0) {
            XFillRectangle (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), (iX-iP), (iY-iP), (iP+iP), (iP+iP));
        } else {
            XDrawPoint (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), iX, iY);
        }
    }
    pswXA->wFlags = pswXA->wFlags | W_XY;
    pswXA->iXo = iX;
    pswXA->iYo = iY;
}

void vDibujaEjes (displayX *psdX, windowX *pswXA, color cCol) {
    vColorForeground (psdX, pswXA, cCol);
    XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), pswXA->iCx, 0, pswXA->iCx, pswXA->wH);
    XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), 0, pswXA->iCy, pswXA->wW, pswXA->iCy);
}

void vResetPunto (windowX *pswXA) {
    pswXA->wFlags = pswXA->wFlags ^ W_XY;
}

void vDibujaOrigen (displayX *psdX, windowX *pswXA, color cCol, int iLen) {
    int iAux, iCx, iCy;
    if (iLen<=0) {
        iAux = 1;
    } else {
        iAux = iLen;
    }
    iCx = ObtenAtributosV(pswXA).width / 2;
    iCy = ObtenAtributosV(pswXA).height / 2;
    vColorForeground (psdX, pswXA, cCol);
    XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), iCx, (iCy-iAux), iCx, (iCy+iAux));
    XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), (iCx-iAux), iCy, (iCx+iAux), iCy);
}

void vColocaDivX (displayX *psdX, windowX *pswXA, color cCol, int iLen, double fD, int iMod, dword dTotal, int iBar, color cColBars) {
    int x;
    dword y;
    for (y=1; y<dTotal; y++) {
        x = (int)(fD * (double)y);
        if (y%iMod) {
            vColorForeground (psdX, pswXA, cCol);
            XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), (pswXA->iCx+x), pswXA->iCy-iLen, (pswXA->iCx+x), pswXA->iCy+iLen);
            //vColorForeground (psdX, pswXA, cColBars);
            //if (iBar) XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), (pswXA->iCx+x), pswXA->iCy-iLen, (pswXA->iCx+x), 0);
        } else {
            vColorForeground (psdX, pswXA, cCol);
            XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), pswXA->iCx+x, pswXA->iCy-iLen-2, pswXA->iCx+x, pswXA->iCy+iLen+2);
            vColorForeground (psdX, pswXA, cColBars);
            if (iBar) XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), pswXA->iCx+x, pswXA->iCy-iLen, pswXA->iCx+x, 0);
        }
    }
}

void vColocaDivY (displayX *psdX, windowX *pswXA, color cCol, int iLen, double fD, int iMod, dword dTotal, int iBar, color cColBars) {
    int x;
    dword y;
    for (y=1; y<dTotal; y++) {
        x = (int)(fD * (double)y);
        if (y%iMod) {
            vColorForeground (psdX, pswXA, cCol);
            XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), (pswXA->iCx-iLen), pswXA->iCy-x, (pswXA->iCx+iLen), pswXA->iCy-x);
            //vColorForeground (psdX, pswXA, cColBars);
            //if (iBar) XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), (pswXA->iCx-iLen), pswXA->iCy-x, ObtenAtributosV(pswXA).width, pswXA->iCy+x);
        } else {
            vColorForeground (psdX, pswXA, cCol);
            XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), pswXA->iCx-iLen-2, pswXA->iCy-x, pswXA->iCx+iLen+2, pswXA->iCy-x);
            vColorForeground (psdX, pswXA, cColBars);
            if (iBar) XDrawLine (ObtenDisplay(psdX), ObtenBufferV(pswXA), ObtenGCV(pswXA), pswXA->iCx+iLen+2, pswXA->iCy-x, ObtenAtributosV(pswXA).width, pswXA->iCy-x);
        }
    }
}
