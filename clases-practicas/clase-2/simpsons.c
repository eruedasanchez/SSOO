/* Ejercicio Simpsons */

/* Bart y Lisa estan muy aburridos y para pasar el tiempo Homero idea un juego:

Bart y Lisa tiran un dado cada uno

Luego, le cuentan a Homero que valor les dio el dado

Finalmente, Homero cuenta cual es el numero mas grande y dice quien gana */

/********** Resolucion **********/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>


/* Se comienza creando la funcion auxiliar que devuelve el numero que salio al arrojar el dado */
int dado(){
    srand(time(NULL) ^ (getpid()<<16));
    return(rand() % 6) + 1;
}

int main(int argc, char **argv){
    srand(time(0));
    write(1, "Hola, soy Homero\n", 16);    // Comienza la ejecucion Homero

    /* Creacion de los pipes Bart y Lisa para la comunicacion */
    int pipe_bart[2];
    int pipe_lisa[2];

    /* Inicializacion y chequeo de errores de cada pipe */
    if(pipe(pipe_bart) == -1){
        perror("pipe");
        exit(1);
    }

    if(pipe(pipe_lisa) == -1){
        perror("pipe");
        exit(1);
    }

    int pid_bart = fork();                                       // Creacion del proceso Bart
    if(pid_bart == 0){
        /* Codigo de Bart */
        close(pipe_lisa[0]);                                // Se cierra el extremo de lectura de Lisa porque no va a estar leyendo ningun dato
        close(pipe_lisa[1]);                                // Se cierra el extremo de escritura de Lisa porque no va a estar escribiendo ningun dato
        close(pipe_bart[0]);                                // Se cierra el extremo de lectura de Bart porque no va a estar leyendo ningun dato
        write(1, "Hola, soy Bart\n", 14);
        int mi_dado = dado();                               // Bart tira el dado

        /* Escritura del valor que saco Bart en su pipe de escritura. Ademas, se chequea que no haya errores en la escritura */
        if(write(pipe_bart[1], &mi_dado, sizeof(int)) != sizeof(int)){
            perror("write");
            exit(1);
        }
        write(1, "Soy Bart y mi dado fue %d\n", mi_dado);
        close(pipe_bart[1]);                                // Se cierra el extremo de escritura porque ya se uso
        exit(0);                                            // Se asegura que la ejecucion del proceso Bart termina en esta linea
    }

    int pid_lisa = fork();                                       // Creacion del proceso Lisa
    if(pid_lisa == 0){
        /* Codigo de Lisa */
        close(pipe_bart[0]);                                // Se cierra el extremo de lectura de Bart porque no va a estar leyendo ningun dato
        close(pipe_bart[1]);                                // Se cierra el extremo de escritura de Bart porque no va a estar escribiendo ningun dato
        close(pipe_lisa[0]);                                // Se cierra el extremo de lectura de Lisa porque no va a estar leyendo ningun dato
        write(1, "Hola, soy Lisa\n", 14);
        int mi_dado = dado();                               // Lisa tira el dado

        /* Escritura del valor que saco Lisa en su pipe de escritura. Ademas, se chequea que no haya errores en la escritura */
        if(write(pipe_lisa[1], &mi_dado, sizeof(int)) != sizeof(int)){
            perror("write");
            exit(1);
        }
        write(1, "Soy Lisa y mi dado fue %d\n", mi_dado);
        close(pipe_lisa[1]);                                // Se cierra el extremo de escritura porque ya se uso
        exit(0);                                            // Se asegura que la ejecucion del proceso Lisa termina en esta linea
    }

    /* Ejecucion de Homero */
    wait(NULL);                                             // Se espera que terminen de ejecutar Bart y Lisa
    int valor_bart;
    int valor_lisa;

    /* Lectura del valor que escribio Bart en su pipe. Ademas, se chequean errores */
    if(read(pipe_bart[0], &valor_bart, sizeof(int)) != sizeof(int)){
            perror("read");
            exit(1);
    }

    /* Lectura del valor que escribio Lisa en su pipe. Ademas, se chequean errores */
    if(read(pipe_lisa[0], &valor_lisa, sizeof(int)) != sizeof(int)){
            perror("read");
            exit(1);
    }

    if(valor_bart > valor_lisa){
        printf("El ganador es Bart\n");
    } else if(valor_bart < valor_lisa){
        printf("La ganadora es Lisa\n");
    } else {
        printf("Ambos sacaron el mismo valor. Hay empate\n");

    }
    return 0;
}