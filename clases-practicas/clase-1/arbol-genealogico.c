/* Ejercicio 5 

Suponga que Juan tiene 2 hijos, Jorge y Julieta. A su vez, Julieta tiene una hija Jeniffer.
Pero suponga que recien cuando nacio Jennifer, Juan lo tuvo a Jorge. Suponga que quiere procesos que delimiten la vida de cada uno.
¿Como podria hacer para que se lancen los procesos en el momento adecuado y sin problemas? 
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]){
    pid_t pid = fork();                       
    /* Si no hubo error (caso donde pid == -1) en la creacion del proceso hijo, pid = 0 para el hijo y pid_padre = pid_hijo */
    if(pid == -1){
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Hijo Julieta */
        printf("%s\n", "Hola, soy Julieta");
        sleep(1);
        pid_t pid = fork();                         // Creacion de la hija Jennifer
        if(pid == -1){
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            /* Hija Jennifer */
            printf("%s\n", "Hola, soy Jennfer");
            sleep(1);
        }
    } else {
        /* Codigo de Juan */
        printf("%s\n", "Hola, soy Juan");
        sleep(1);

        /* Creacion de Jorge pero antes se espera a Julieta */
        int status;
        if(wait(&status) < 0){
            /* Termino con errores */
            perror("wait");
            exit(-1);
        }
        /* Julieta termino normalmente */
        pid_t pid = fork();                         // Creacion de la hijo Jorge
        if(pid == -1){
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            /* Hija Jorge */
            printf("%s\n", "Hola, soy Jorge");
            sleep(1);
        }
    }
    exit(EXIT_SUCCESS);                             // Cada uno finaliza su proceso
}