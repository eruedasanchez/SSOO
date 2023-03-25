#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    pid_t pidchild = fork();                            // Se crea un proceso hijo
    if(pidchild == 0){
        printf("%s\n", "Hola, soy el proceso hijo");
    } else {
        wait(NULL);                                     // El proceso padre espera que termine el proceso hijo
        printf("%s\n", "Hola, soy el proceso padre");
    }
    return 0;
}
