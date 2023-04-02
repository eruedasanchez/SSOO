#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ANTERIOR(i,n) ((i+n-1) % n) 					// Calculo del proceso anterior (congruencia para realizar varias posibles vueltas por el anillo)

void cerrar_pipes_sin_usar(int i, int n, int pipe_hijo[][2]){
	for(int s=0; s < n; s++){
		if(s != i){
			close(pipe_hijo[s][1]);
		}
		if(s != ANTERIOR(i,n)){
			close(pipe_hijo[s][0]);
		}
	}
}

int main(int argc, char **argv){	
	int status, pid, n;
	int hijo_inicia;
	int buffer;

	if (argc != 4){ 
		printf("Uso: anillo <n> <c> <s> \n"); exit(0);
	}
    
  	/****************** COMPLETAR ******************/

	/* Parsing de los argumentos */
	n = atoi(argv[1]);
	buffer = atoi(argv[2]);
	int start = atoi(argv[3]);
    printf("Se crearán %i procesos, se enviará el caracter %i desde proceso %i\n", n, buffer, start);

	/* El padre crea n pipes */
	int  pipe_hijo[n][2];
	for(int j=0; j < n; j++){
		if(pipe(pipe_hijo[j]) == -1){
			perror("Error al crear el pipe\n");
			exit(1);
		}
	}

	/* El padre crea n hijos que conocen todos los pipes */
	int i = 0;
	for(i=0; i < n; i++){
		pid = fork();
		if(pid == -1){
			perror("fork");
			exit(1);
		}
		if(pid == 0){
			break;
		}
		if(i == start){
			hijo_inicia = pid; 											// Se identifica al hijo que recibe el mensaje
		}
	}

	if(pid == 0){
		/* Cualquier otro proceso hijo */
		cerrar_pipes_sin_usar(i,n,pipe_hijo);
		read(pipe_hijo[ANTERIOR(i,n)][0], &buffer, 1);					 // Lectura del mensaje enviado por el proceso anterior
		close(pipe_hijo[ANTERIOR(i,n)][0]); 							 // Cierre del fd de lectura del proceso anterior
		printf("Soy el proceso %d, con mensaje %d\n", i, buffer);

		/* Se chequea si es el proceso que inicia la comunicacion */
		if(i != start){
			buffer++;
		}

		/* Se envia el mensaje incrementado en una unidad y cierre del fd de escritura del proceso actual */
		write(pipe_hijo[i][1], &buffer, 1);
		close(pipe_hijo[i][1]);
		exit(0);
	} else {
		/* Codigo del roceso padre */
		for(int k=0; i < n; k++){
			close(pipe_hijo[k][1]);
			if(k != start){
				close(pipe_hijo[k][0]);
			}
		}
		waitpid(hijo_inicia, NULL, 0); 									 // Se espera al proceso inicial para leer el ultimo valor

		read(pipe_hijo[start][0], &buffer, 1);					 
		close(pipe_hijo[start][0]); 							 
		printf("Soy el proceso PADRE con mensaje %d\n", buffer);
		exit(0);
	}
}

