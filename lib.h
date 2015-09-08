/*Este archivo contiene algunas declaraciones importantes para el manejo de tipos de datos*/
/*cuando se emplean 32 y 64 bits; es importante definir el la palabra de procesador en    */
/*donse se ejecutar� el programa. Por default se emplean 32 bits                          */


/*CPU de 64 bits*/
//#define LIBCPU_64   1
#define LIBCPU_32   1

#ifndef LIB_H
    #define LIB_H

    #ifndef TRUE
        #define TRUE    1
        #define FALSE   0
        #define CERO    0
    #endif

    #ifdef LIBCPU_64
        /*Para CPU de 64 bits*/
        typedef unsigned char      byte;      /* 8 bits*/
        typedef short    int       sint;      /*16 bits*/
        typedef unsigned short int sword;     /*16 bits*/
        typedef unsigned int       word;      /*32 bits*/
        typedef unsigned int       dword;     /*32 bits*/
        typedef unsigned long      dlword;    /*64 bits*/
    #else
        /*Para CPU de 32 bits*/
        typedef unsigned char      byte;      /* 8 bits*/
        typedef short    int       sint;      /*16 bits*/
        typedef unsigned short int sword;     /*16 bits*/
        typedef unsigned int       word;      /*32 bits*/
        typedef unsigned long      dword;     /*32 bits*/
        typedef unsigned long      dlword;    /*32 bits*/
    #endif

    #define PI           (double)3.1415926535897932384626433832795
    #define DPI          (double)6.283185307179586476925286766559
    #define PIM          (double)1.5707963267948966192313216916398

#endif
