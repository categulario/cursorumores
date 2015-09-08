#Inidica el archivo a compilar
CCP=gcc

PAR_LIB= -c -Wall -shared
PAR_LIB_XGRAPH= -lX11
PAR_C= -lm

#Lista de nombres de biblioteca y programas (para cygwin y linux)
LIBS=matriz.so
LIBSXG=libxglib.so

#Lista de los archivos a compilar
LIB01=matriz

PROG_00=fs
PROG_01=par

#Directorios de bibliotecas y cabeceras
INC=.


#Genera solo los programas
prog: 0

#Genera todo
all: lib 0 1

#Solo bibliotecas
lib: matriz

#Se encarga de generar un programa
0:
	@echo PROGRAMA: $(PROG_00)
	$(CCP) $(PAR_C) $(PROG_00).c $(LIBS) -o $(PROG_00) -I$(INC)

1:
	@echo PROGRAMA: $(PROG_01)
	$(CCP) $(PAR_C) $(PROG_01).c $(LIBS) $(LIBSXG) -o $(PROG_01) -I$(INC) $(PAR_LIB_XGRAPH)

matriz:
	$(CCP) $(PAR_LIB) $(LIB01).c -o $(LIB01).so
