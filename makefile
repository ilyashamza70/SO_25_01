#makefile per la prova del 25 GENNAIO 2023 di Hamza Ilyas

CC=gcc 					#parametro utilizzato dal compilatore C
CFLAG=-Wall				#cosi mostro tutti i warning di compilazione e linking
SRC = $(wildcard *.c)	#RICERCA TUTTI I FILE CHE TERMINANO CON .c 
TAR = $(SRC:.c=)		
ver= gcc -V		
#Versione usata del gcc è ver verificabile con cat $ver in seguito (togli il commento)
# $(ver)

all: $(TAR)

%: %.CC
		$(CC) $(FLAG)	$< -o $@	#COmando lanciato per creazione file.out
		
