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
    portno = 1008;
/*Socket(): crea un socket no ligado en un dominio de comunicaciones y devuelve descriptor de archivo que se puede utilizar en llamadas de función posteriores que operan en sockets Los parametros que toma son Dominio de comunicaciones en el que se va a crear un socket, tipo de socket que va a ser creado yY un protocolo particular que se utilizará con el socket. Especificar un protocolo de 0 hace que socket () utilice un protocolo predeterminado no especificado apropiado para el tipo de socket solicitado.*/
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
/*Connect () Conecta un socket toma como parametros un socket que especifica el descriptor de archivo asociado con el socket de dirección, Señala una estructura sockaddr que contiene la dirección del mismo nivel  y especifica la longitud de la estructura sockaddr apuntada por el argumento de dirección.*/
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
 Asociado con dicho archivo.
/*Write() escribe hasta contar los bytes desde el buffer señalado buf al archivo referido por el descriptor de archivo fd, el número de bytes escritos puede ser menor que el recuento si, por ejemplo,
hay espacio insuficiente en el medio físico subyacente, o se encuentra el límite de recursos RLIMIT_FSIZE, o la llamada fue interrumpida por un manejador de señal después de haber escrito menos
que contar bytes.*/
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
/*Read ()intenta leer hasta contar bytes desde el descriptor de archivo fd en el buffer comenzando en buf. En los archivos que admiten la búsqueda, la operación de lectura comienza en el desplazamiento del archivo y el desplazamiento del archivo se incrementa por el número de bytes leídos. Si el desplazamiento del archivo es igual o mayor al final del archivo, no se leen bytes, y read () devuelve cero*/
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
 /* close() cerrar un descriptor de archivo.*/
    close(sockfd);
    return 0;
}
