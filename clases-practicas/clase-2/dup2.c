#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>          // Habilta O_WRONLY y O_CREAT
// #include <signal.h>

int main(int argc, char **argv){
    int fd = open("archivo.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);    // Se abre el archivo de escritura y de no estar creado, se crea. Ademas, se le asignan permisos al usuario de lectura y esccritura para poder usarlo
    /* Se copia el fd*/
    dup2(fd, STDOUT_FILENO);                                                // La entrada de la Standard Output apunta al nuevo fd que se acaba de spawnear
    
    /* Ejecucion de echo */
    char *args[] = {"echo","hola, hoy es jueves y tu cuerpo lo sabe",NULL};
    execvp("echo",args);
    
    /* Por ultimo, se cierran los fd */
    close(fd);
}

/* Observaciones */

// O_WRONLY archivo de solo escritura
// O_CREAT crea el archivo si todavia no lo esta
// S_IRUSR S_IWUSR asigna permisos al usuario R(ead), W(rite), USR (user)