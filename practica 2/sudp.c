#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
 
int main(void)
{
/*socket(): crea un socket no ligado en un dominio de comunicaciones y devuelve descriptor de archivo que se puede utilizar en llamadas de función posteriores que operan en sockets Los parametros que toma son Dominio de comunicaciones en el que se va a crear un socket, tipo de socket que va a ser creado y Y un protocolo particular que se utilizará con el socket. Especificar un protocolo de 0 hace que socket () utilice un protocolo predeterminado no especificado apropiado para el tipo de socket solicitado.*/
  int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  struct sockaddr_in sa; 
  char buffer[1024];
  ssize_t recsize;
  socklen_t fromlen;
 /* memset toma como parametros, un puntero de Dirección de inicio de la memoria a llenar, valor a llenar, el número de bytes que deben rellenarse a partir de la dirección de inicio a llenar*/
  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = INADDR_ANY;
  int portno = 7654;
  sa.sin_port = htons(portno);
  fromlen = sizeof(sa);

/*bind(): enlaza un nombre a un socket, toma como parametros el descriptor de archivo del socket que va a enlazar, señala una estructura sockaddr con la dirección con la que se va a enlazar el socket y especifica la longitud de la estructura sockaddr apuntada por el argumento de dirección.*/
  
  if (-1 == bind(sock,(struct sockaddr *)&sa, sizeof(sa)))
  {
    perror("error bind failed");
/* close() cerrar un descriptor de archivo.*/
    close(sock);
    exit(EXIT_FAILURE);
  } 
 
  printf ("recv test....\n");
/*recvfrom(): Recibe un mensaje desde un socket toma como parametros un socket, un buffer, la longitud en bytes de la memoria intermedia apuntada por el argumento del buffer, y banderas
Especifica el tipo de recepción del mensaje. Los valores de este argumento se forman logicamente OR'ing cero o más de los siguientes valores:
MSG_PEEK: Observa un mensaje entrante. Los datos se tratan como no leídos y la siguiente función recvfrom () o similar seguirán devolviendo estos datos.
MSG_OOB: Solicita datos fuera de banda. La significación y la semántica de los datos fuera de banda son específicas del protocolo.
MSG_WAITALL: Solicita que el bloque de funciones hasta que se pueda devolver la cantidad total de datos solicitados. La función puede devolver una cantidad menor de datos si se detecta una señal, si se termina la conexión, si se especificó MSG_PEEK o si hay un error pendiente para el socket.*/
  recsize = recvfrom(sock, (void *)buffer, sizeof(buffer), 0, (struct sockaddr *)&sa, &fromlen);
  if (recsize < 0) {
      fprintf(stderr, "%s\n", strerror(errno));
      exit(EXIT_FAILURE);
  }
  printf("recsize: %d\n ", recsize);
/*sleep () hará que el hilo llamado sea suspendido de la ejecución hasta que haya transcurrido el número de segundos en tiempo real especificado por los segundos de argumento o una señal sea entregada al hilo que llama y su acción es invocar una función de captura de señal o Para terminar el proceso. El tiempo de suspensión puede ser mayor que el solicitado debido a la programación de otra actividad por parte del sistema.*/
  sleep(1);
  printf("datagram: %.*s\n", (int)recsize, buffer);

  return 0; 
}
