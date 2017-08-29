#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
 
 
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
     		portno = 7654;
                hostn = "127.0.0.1";
     }
  int sock;
  struct sockaddr_in sa;
  int bytes_sent;
  char buffer[200];
  int i; 
  strcpy(buffer, "Hola mundo!");
 
  sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (-1 == sock) 
    {
      printf("Error Creating Socket");
      exit(EXIT_FAILURE);
    }
 
  memset(&sa, 0, sizeof sa);
 
  sa.sin_family = AF_INET;

  sa.sin_addr.s_addr = inet_addr(hostn);
 
  sa.sin_port = htons(portno);
 
  bytes_sent = sendto(sock, buffer, strlen(buffer), 0,(struct sockaddr*)&sa, sizeof sa);
  if (bytes_sent < 0) {
    printf("Error sending packet: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (bytes_sent < 0) {
    printf("Error sending packet: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
 
  close(sock); 
  return 0;
}
