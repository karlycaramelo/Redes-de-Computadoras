#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, portnoNuevo;
     portnoNuevo = 10100;
     //Puerto por default para realizar la conexion
     portno = 10080;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;

     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     //Hacemos un while infinito para dejar al servidor escuchando clientes en el puerto 10080
     while(1){
             //Le sumanos 1 al puerto que empieza en 10100 para  cada vez que se conecte un nuevo clienta darle un puerto 
             //diferente
  	     portnoNuevo++;

	     newsockfd = accept(sockfd, 
		         (struct sockaddr *) &cli_addr, 
		         &clilen);
	     if (newsockfd < 0) 
		  error("ERROR on accept");
	     bzero(buffer,256);
	     n = read(newsockfd,buffer,255);
	     if (n < 0) error("ERROR reading from socket");
	     printf("Here is the message: %s\n",buffer);
	     char nuevopuerto[5];
             sprintf(nuevopuerto, "%d", portnoNuevo);
             //Mandamos como respuesta al primermensaje de cliente el otro puerto
	     //que abriremos para la comunicacion
	     n = write(newsockfd,nuevopuerto,5);
	     if (n < 0) error("ERROR writing to socket");
	     close(newsockfd);




	     //Hacemos otro socket completamente nuevo (de ahi el nombre de todas sus partes que ahora terminaan en Nuevo)
             //En donde entablaremos comunicacion con el cliente
	     printf("Abriendo conexion al puerto: %d\n", portnoNuevo);
	     int sockfdNuevo, newsockfdNuevo;
	     socklen_t clilenNuevo;
	     char bufferNuevo[256];
	     struct sockaddr_in serv_addrNuevo, cli_addrNuevo;
	     int nNuevo;
	     sockfdNuevo = socket(AF_INET, SOCK_STREAM, 0);
	     if (sockfdNuevo < 0) 
		error("ERROR opening socket");
	     bzero((char *) &serv_addrNuevo, sizeof(serv_addrNuevo));
	     serv_addrNuevo.sin_family = AF_INET;
	     serv_addrNuevo.sin_addr.s_addr = INADDR_ANY;

	     serv_addrNuevo.sin_port = htons(portnoNuevo);

	     if (bind(sockfdNuevo, (struct sockaddr *) &serv_addrNuevo,
		      sizeof(serv_addrNuevo)) < 0) 
		      error("ERROR on binding");
	     listen(sockfdNuevo,6);
	     clilenNuevo = sizeof(cli_addrNuevo);
	
             //Mantendremos este nuevo socket abierto hasta que keepopen tenga valor 0
             //Esto pasa cuando el cliente manda el mensaje close\n
             int keepopen = 1;
	     while(keepopen){
		     newsockfdNuevo = accept(sockfdNuevo, 
				 (struct sockaddr *) &cli_addrNuevo, 
				 &clilenNuevo);
		     if (newsockfdNuevo < 0) 
			  error("ERROR on accept");
		     bzero(bufferNuevo,256);
		     nNuevo = read(newsockfdNuevo,bufferNuevo,255);
		     if (nNuevo < 0) error("ERROR reading from socket");
		     printf("Here is the message: %s\n",bufferNuevo);

                     //Si nos mandane el mensaje close cambiamos keepopen a 0
                     //Y conetamos al cliente un close
		     if(strcmp("close\n", bufferNuevo) == 0){
			     nNuevo = write(newsockfdNuevo,"close",5);
			     if (nNuevo < 0) error("ERROR writing to socket");
			     close(newsockfdNuevo);
			     keepopen = 0;
                     //Si no mandale el mensaje close mandamos un mensaje de que leeimos lo que se mando
		     }else{
			     nNuevo = write(newsockfdNuevo,"I got your message",18);
			     if (nNuevo < 0) error("ERROR writing to socket");
			     close(newsockfdNuevo);
                     }
	     }
	     close(sockfdNuevo);
	     //Cerramos la comunicacion en este puerto y regresamos a escuchar el puerto 10080
             //Para atender a un nuevo cliente si este se conecta
	     printf("Regresando a escuchar al puerto: 10080\n");
     }
     close(sockfd);
     
     return 0; 
}
