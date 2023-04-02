#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ANTERIOR(i,n) ((i+n-1) % n) 					// Calculo del proceso anterior (congruencia para realizar varias posibles vueltas por el anillo)



int main(int argc, char **argv){	
	int status, pid, n;
	int hijo_inicia;
	int buffer;

	if (argc != 4){ 
		printf("Uso: anillo <n> <c> <s> \n"); exit(0);
	}
    
  	/* COMPLETAR */
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i \n", n, buffer[0], start);
    
    /* COMPLETAR */
    
}
