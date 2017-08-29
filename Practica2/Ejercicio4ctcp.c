#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
     //Puerto por default para realizar la conexion
    portno = 10080;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    char hostn[] = "127.0.0.1";
    server = gethostbyname(hostn);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    bzero(buffer,256);

    //Vamos a manda por default el primer mensaje al servidor
    //Para que nos conteste el puerto por que el cual tendremos la comunicacion con el
    strcpy(buffer,"Hola, dame un puerto XP");
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);

    //Convertimos el mensaje que nos responde el servidor a un entero ya que este es el nuevo pierto
    int portnoNuevo = atoi(buffer);
    printf("Reconectando al puerto:%d\n",portnoNuevo);
    close(sockfd);

    //Con el nuevo puerto creamos un nuevo socket para comunicarnos con el servidor 
    //(de ahi el nombre de todas sus partes que ahora terminaan en Nuevo)
    printf("Para cerrar la conexion con el servidor mandale el mensaje: close\n");
    int keepopen = 1;
    //Rearemos la conexion cada ves que se mande un mensaje para mantener un charla con el servidor
    //Hasta que el servidor conteste close, que esto pasara cuando nosotros le mandemos el mensaje close
    while(keepopen){
	    int sockfdNuevo, nNuevo;
	    struct sockaddr_in serv_addrNuevo;
	    struct hostent *serverNuevo;
	    char bufferNuevo[256];
	    sockfdNuevo = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfdNuevo < 0) 
		error("ERROR opening socket");

	    serverNuevo = gethostbyname(hostn);
	    if (serverNuevo == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	    }
	    bzero((char *) &serv_addrNuevo, sizeof(serv_addrNuevo));
	    serv_addrNuevo.sin_family = AF_INET;
	    bcopy((char *)serverNuevo->h_addr, 
		 (char *)&serv_addrNuevo.sin_addr.s_addr,
		 serverNuevo->h_length);
	    serv_addrNuevo.sin_port = htons(portnoNuevo);
	 
	    if (connect(sockfdNuevo,(struct sockaddr *) &serv_addrNuevo,sizeof(serv_addrNuevo)) < 0) 
		error("ERROR connecting");


	    printf("Please enter the message: ");
	    bzero(bufferNuevo,256);
	    fgets(bufferNuevo,255,stdin);
	    nNuevo = write(sockfdNuevo,bufferNuevo,strlen(bufferNuevo));
	    if (nNuevo < 0) 
		 error("ERROR writing to socket");
	    bzero(bufferNuevo,256);
	    nNuevo = read(sockfdNuevo,bufferNuevo,255);
	    if (nNuevo < 0) 
		 error("ERROR reading from socket");

	    //Si nos mandane el mensaje close cambiamos keepopen a 0
	    //Y ya no reabrieremos el socket
	    if(strcmp("close", bufferNuevo) == 0){
	        printf("Cerrando la comunicacion...\n");
                keepopen = 0;
	    //Si nos mandane el mensaje imprimos el emsnaje que nos mandaron
	    }else{
	        printf("%s\n",bufferNuevo);
	    }
	    close(sockfdNuevo);
    }

    return 0;
}
