#include <sys/ptrace.h>
#include <sys/syscall.h>						// Include the syscall SYS_kill
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>  

int main(int argc, char* argv[]) {
	int status;
	pid_t child;
	int sysno;
	int rax;
	int signal;

	if (argc <= 1) {
		fprintf(stderr, "Uso: %s comando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		/* Solo se ejecuta en el Hijo */
		if(ptrace(PTRACE_TRACEME, child, NULL, NULL)){
			/* Proceso solicita ser monitoreado por el padre */
			perror("ERROR child ptrace(PTRACE_TRACEME, child, NULL, NULL)");
			exit(1);
		}
		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);
	} else {
		/* Solo se ejecuta en el Padre */
		while(1) {
			if (wait(&status) < 0) { perror("waitpid"); break; }
			if (WIFEXITED(status)) break; /* Proceso terminado */

			sysno =  ptrace(PTRACE_PEEKUSER, child, 8 * ORIG_RAX, NULL); 	// Numero de la syscall del proceso hijo
			rax =  	 ptrace(PTRACE_PEEKUSER, child, 8 * RAX, NULL);  		// Estado del proceso
			signal = ptrace(PTRACE_PEEKUSER, child, 8 * RSI, NULL); 			// Parametro del proceso 

			if(rax = -ENOSYS){
				/* Estoy en la entrada de una syscall */
				if(sysno == SYS_kill){
					fprintf(stderr, "se envio la señal %i\n", signal);
					if(signal == SIGKILL){
						fprintf(stderr, "se ha evitado el envio de la señal SIGKILL(%i)!\n", SIGKILL);
						ptrace(PTRACE_POKEUSER, child, 8 * RSI, NULL);
						ptrace(PTRACE_POKEUSER, child, 8 * RSI, NULL);
						break;
					}
				}
			}
			ptrace(PTRACE_SYSCALL, child, 8 * RSI, NULL);       // Se reanuda al hijo hasta la siguiente señal o syscall recibida
		}
		ptrace(PTRACE_DETACH, child, 8 * RSI, NULL); 			// Se deja de monitorear al proceso hijo 
	}
	return 0;
}
