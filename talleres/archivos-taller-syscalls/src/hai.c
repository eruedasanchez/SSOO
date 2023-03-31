#include <signal.h>     				/* constantes como SIGINT */
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int contador_hijo = 0;

void handler_hijo(){
	contador_hijo++;
	write(1, "ya va!\n", 7);
}

void handler_padre(){
	int status;
	if(wait(&status) < 0){
		perror("waitpid");
		exit(1);
	}
	if (WIFEXITED(status)){
		exit(0); 				// Proceso terminado
	}
}

int main(int argc, char* argv[]) {
  	// Completar
	pid_t pid_hijo;

	/* Se chequea el correcto pasaje de argumentos */
	if(argc <= 1){
		fprintf(stderr, "Uso: %s comando [argumentos ...]\n" , argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	pid_hijo = fork();

	/* Chequeo de errores en la creacion de procesos */
	if(pid_hijo == -1){
		perror("fork");
		return 1;
	}
	if(pid_hijo == 0){
		/* Codigo del proceso hijo */
		signal(SIGURG, handler_hijo);		     // Interrumpe la ejecucion y en lugar de ejecutar la señal SIGURG, se llama al handler_hijo
		while(contador_hijo < 5){}
		kill(getpid(),SIGINT); 					// Envia la señal SIGINT al proceso padre
	
		execvp(argv[1], argv+1);
		/* Si vuelve de exec(), hubo un error */
		perror("ERROR child exec(...)");
		exit(1);
	} else {
		/* Codigo del proceso padre */
		signal(SIGINT, handler_padre);

		while (1){
			sleep(1);
			write(1, "sup!\n" ,5);
			kill(pid_hijo, SIGURG); 			// Envia al proceso hijo la señal SIGURG
		}
	}
	return 0;
}
