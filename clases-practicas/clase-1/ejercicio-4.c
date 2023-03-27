#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    int i = 0;
    pid_t pidchild = fork();                            // Se crea un proceso hijo
    if(pidchild == 0){
        printf("%s\n", "Hola, soy el proceso hijo");
        i++;
    } else {
        wait(NULL);                                     // El proceso padre espera que termine el proceso hijo
        printf("%s\n", "Hola, soy el proceso padre");
        printf("%i\n", i);
    }
    return 0;
}

/* ¿Cual va a ser el valor de la variable i que se imprime por pantalla? 

El valor de la variable i que se imprime por pantalla es 0 porque al ejecutar fork()
se hace una copia de memoria (no comparten los procesos la misma memoria).
Entonces, el proceso hijo modifica el valor de la variable i que esta
en su memoria, no la que esta en la memoria del proceso padre. 
*/