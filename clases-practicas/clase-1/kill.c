#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>

int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("%s\n", "Soy el hijo y estoy colgado");
        while(1){}
    } else {
        printf("%s\n", "Soy el padre y estoy esperando a:", pid);
        kill(pid, SIGKILL);                                             // Con esto, se logra que el proceso padre finalice o mate al proceso hijo 
        wait(NULL);
        printf("%s\n", "Mi hijo ya termino, ahora me toca a mi");
    }
    return 0;
}



