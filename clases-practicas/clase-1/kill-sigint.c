#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

int i = 0;

void handlerSigint(){
    i++;
}

int main(){
    pid_t pid = fork();
    if(pid == 0){
        signal(SIGINT, handlerSigint);                              // Con esto se logra que al proceso hijo, cuando le llega la señal SIGINT, llama al handler, aumenta la variable i y sale del ciclo i == 0        
        printf("%s\n", "Soy el hijo y estoy colgado");
        while(i == 0){}
    } else {
        printf("%s\n", "Soy el padre y estoy esperando a:", pid);
        kill(pid, SIGINT);                                              
        wait(NULL);
        printf("%s\n", "Mi hijo ya termino, ahora me toca a mi");
    }
    return 0;
}