#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
 
int main(int argc, char *argv[])
{
     int portno;
     //Leemos de los argumentos que pasen por la consola que despues de la bandera --port vendra el numero de puerto
     //Nuestro metodo no checa error puesto que si los argumentos que se pasan son erroneos el programa morira
     if(argc > 1){
	     for (int i  = 0 ; i < argc; i++){

                  if(strcmp("--port", argv[i]) == 0){
			portno = atoi(argv[i+1]);
		  	printf("Puerto: %d\n", portno);
		  }
	     }
     //Si no pasan arumentos inicializamos con los por default
     }else{
     		portno = 7654;
     }
  int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  struct sockaddr_in sa; 
  char buffer[1024];
  ssize_t recsize;
  socklen_t fromlen;
 
  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = INADDR_ANY;
  sa.sin_port = htons(portno);
  fromlen = sizeof(sa);
  
  if (-1 == bind(sock,(struct sockaddr *)&sa, sizeof(sa)))
  {
    perror("error bind failed");
    close(sock);
    exit(EXIT_FAILURE);
  } 
 
  printf ("recv test....\n");
  recsize = recvfrom(sock, (void *)buffer, sizeof(buffer), 0, (struct sockaddr *)&sa, &fromlen);
  if (recsize < 0) {
      fprintf(stderr, "%s\n", strerror(errno));
      exit(EXIT_FAILURE);
  }
  printf("recsize: %d\n ", recsize);
  sleep(1);
  printf("datagram: %.*s\n", (int)recsize, buffer);

  return 0; 
}
