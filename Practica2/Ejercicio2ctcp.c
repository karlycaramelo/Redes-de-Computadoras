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
     int portno;
     char *hostn;
     //Leemos de los argumentos que pasen por la consola que despues de la bandera --port vendra el numero de puerto
     //Despues de la bandera --host vendra el nombre del host
     //Nuestro metodo no checa error puesto que si los argumentos que se pasan son erroneos el programa morira
     if(argc > 1){
	     for (int i  = 0 ; i < argc; i++){

                  if(strcmp("--port", argv[i]) == 0){
			portno = atoi(argv[i+1]);
		  	printf("Puerto: %d\n", portno);
		  }
                  if(strcmp("--host", argv[i]) == 0){
			hostn = argv[i+1];
		  	printf("Host: %s\n", hostn);
		  }
	     }
     //Si no pasan arumentos inicializamos con los por default
     }else{
     		portno = 1008;
                hostn = "127.0.0.1";
     }
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

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
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}
