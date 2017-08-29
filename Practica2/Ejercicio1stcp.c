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
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
/*Socket(): crea un socket no ligado en un dominio de comunicaciones y devuelve descriptor de archivo que se puede utilizar en llamadas de función posteriores que operan en sockets los parametros que toma son Dominio de comunicaciones en el que se va a crear un socket, tipo de socket que va a ser creado Y un protocolo particular que se utilizará con el socket. Especificar un protocolo de 0 hace que socket () utilice un protocolo predeterminado no especificado apropiado para el tipo de socket solicitado.*/
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     portno = 1008;
     serv_addr.sin_port = htons(portno);
/*bind enlaza un nombre a un socket, toma como parametros el descriptor de archivo del socket que va a enlazar, señala una estructura */
 /*sockaddr con la dirección con la que se va a enlazar el socket y especifica la longitud de la estructura sockaddr apuntada por el argumento de dirección.*/
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)                
              error("ERROR on binding");            
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
/*Read () intenta leer hasta contar bytes desde el descriptor de archivo fd en el buffer comenzando en buf. En los archivos que admiten la búsqueda, la operación de lectura comienza en el desplazamiento del archivo y el desplazamiento del archivo se incrementa por el número de bytes leídos. Si el desplazamiento del archivo es igual o mayor al final del archivo, no se leen bytes, y read () devuelve cero.*/
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
/*Write() escribe hasta contar los bytes desde el buffer señalado buf al archivo referido por el descriptor de archivo fd, el número de bytes escritos puede ser menor que el recuento si, por ejemplo,
hay espacio insuficiente en el medio físico subyacente, o se encuentra el límite de recursos RLIMIT_FSIZE, o la llamada fue interrumpida por un manejador de señal después de haber escrito menos
que contar bytes.*/
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
 /* close() cerrar un descriptor de archivo.*/
     close(newsockfd);
     close(sockfd);
     return 0; 
}
