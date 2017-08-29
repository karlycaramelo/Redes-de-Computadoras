#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
 
 
int main()
{
  int sock;
  struct sockaddr_in sa;
  int bytes_sent;
  char buffer[200];
  int i; 
  strcpy(buffer, "Hola mundo!");
/*Socket(): crea un socket no ligado en un dominio de comunicaciones y devuelve descriptor de archivo que se puede utilizar en llamadas de función posteriores que operan en sockets Los parametros que toma son Dominio de comunicaciones en el que se va a crear un socket, tipo de socket que va a ser creado Y un protocolo particular que se utilizará con el socket. Especificar un protocolo de 0 hace que socket () utilice un protocolo predeterminado no especificado apropiado para el tipo de socket solicitado.*/
 
  sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (-1 == sock) 
    {
      printf("Error Creating Socket");
      exit(EXIT_FAILURE);
    }
  /* memset toma como parametros, un puntero de Dirección de inicio de la memoria a llenar, valor a llenar, el número de bytes que deben rellenarse a partir de la dirección de inicio a llenar*/
  memset(&sa, 0, sizeof sa);
 
  sa.sin_family = AF_INET;

  sa.sin_addr.s_addr = inet_addr("127.0.0.1");
 
  sa.sin_port = htons(7654);

/*sendto() envía un mensaje a través de un socket de modo de conexión o sin conexión. Si el socket es connectionless-mode, el mensaje será enviado a la dirección especificada por dest_addr. Si el socket es modo de conexión, se omite dest_addr, toma como argumentos un socket especifica el descriptor del archivo de socket, un mensaje señala a un buffer que contiene el mensaje a enviar, también toma longitud el tamaño del mensaje en bytes, banderas el tipo de transmisión del mensaje. Los valores de este argumento se forman por OR'ing lógicamente cero o más de los siguientes indicadores:
MSG_EOR: Finaliza un registro (si está soportado por el protocolo)
MSG_OOB: Envía datos fuera de banda en sockets que admiten datos fuera de banda. La significación y la semántica de los datos fuera de banda son específicas del protocolo.
Dest_addr: Señala una estructura sockaddr que contiene la dirección de destino. La longitud y el formato de la dirección dependen de la familia de direcciones del socket.
Destino: Especifica la longitud de la estructura sockaddr apuntada por el argumento dest_addr.*/
 
  bytes_sent = sendto(sock, buffer, strlen(buffer), 0,(struct sockaddr*)&sa, sizeof sa);
  if (bytes_sent < 0) {
    printf("Error sending packet: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (bytes_sent < 0) {
    printf("Error sending packet: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
 /* close() cerrar un descriptor de archivo.*/
  close(sock); 
  return 0;
}
