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
     int portno;
     if(argc > 1){
	     for (int i  = 0 ; i < argc; i++){

                  if(strcmp("--port", argv[i]) == 0){
			portno = atoi(argv[i+1]);
		  	printf("Puerto: %d\n", portno);
		  }
	     }
     }else{
     		portno = 1008;
     }
     int sockfd, newsockfd;
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

     //Para mantener el servidor vivo simplemente hacemos un while infinito
     //Que se queda a la espera del metodo accept
     while(1){
	     newsockfd = accept(sockfd, 
		         (struct sockaddr *) &cli_addr, 
		         &clilen);
	     if (newsockfd < 0) 
		  error("ERROR on accept");
	     bzero(buffer,256);
	     n = read(newsockfd,buffer,255);
	     if (n < 0) error("ERROR reading from socket");
	     printf("Here is the message: %s\n",buffer);
	     n = write(newsockfd,"I got your message",18);
	     if (n < 0) error("ERROR writing to socket");
	     close(newsockfd);
     }
     close(sockfd);
     
     return 0; 
}
