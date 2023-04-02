#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "config.h"

int main(int argc, char **argv){
	char bufrecv[MSGLEN + 1], *bufsend, *p;
	struct sockaddr_in name;
	size_t bufsendsiz=0;
	ssize_t r, w;
	int s;
	bufsend = NULL;
	memset(&name, 0, sizeof(name));

	if (argc != 2) {
		fprintf(stderr, "uso: %s ip\n", argv[0]);
		exit(1);
	}

	//Creacion del socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	//Configuracion, no tocar
	if (!inet_aton(argv[1], &name.sin_addr)) {
		fprintf(stderr, "%s: ip desconocida\n", argv[1]);
		exit(1);
	}

	name.sin_family = AF_INET;
	name.sin_port = htons(PORT);

	//COMPLETAR: conectar el socket
	if(connect(s, (struct sockaddr *)&name, sizeof(name)) == -1){
		perror("connect");
		exit(1);
	}
	
	//COMPLETAR: Recibir mensaje de bienvenida y ponerlo en bufrecv
	if((r = recv(s, bufrecv, MSGLEN, 0)) == -1){
		perror("recv");
		exit(1);
	}
	bufrecv[r] = '\0';
	printf("Bienvenida: %s\n",bufrecv);

	for (;;) {
		printf("[%s]> ", argv[1]);
		//Tomamos el input
		if ((w = getline(&bufsend, &bufsendsiz, stdin)) == -1) {
			/* Lectura de mensajes que van llegando desde STDIN */
			if (!feof(stdin))
				perror("getline");
			break;
		}
		//Nos fijamos si era ENDMSG para cortar la ejecucion
		if (strncmp(bufsend, ENDMSG, w) == 0){
			break;
		}

		//COMPLETAR: Enviar el mensaje
		if(send(s, bufsend, w, 0) == -1){
			perror("send");
			exit(1);
		}

		//COMPLETAR: escibir mensajes hasta que envie CMDSEP
		while((r = recv(s, bufrecv, MSGLEN, 0)) > 0){
			bufrecv[r] = '\0';

			p = strstr(bufrecv, CMDSEP); 					// Se imprimen los mensajes hasta que llega CMDSEP
			if(p){
				*p = '\0';
			}
			if(p){
				break;
			}
			printf("%s\n", bufrecv);
		}
		if(r == -1){
			perror("recv");
			exit(1);

		}
	}
	free(bufsend);
	close(s);

	return 0;
}



