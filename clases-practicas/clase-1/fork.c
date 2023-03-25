/* Ejercicio 1 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
    pid_t pidchild = fork();                // Se crea un proceso hijo
    printf("%s\n", "hola");
    return 0;
}

/* 
¿Cuantas veces se va a ejecutar la linea 10, es decir, se va a imprimir por pantalla el string "hola"?

El string "hola" se va a imprimir por pantalla dos veces, una por el proceso padre y otra por el proceso hijo
creado al ejecutar fork().
*/
