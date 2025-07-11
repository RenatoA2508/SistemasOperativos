#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

void error(char *msg) {
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  int sd, nsd, portno, pid;
  socklen_t cltl;
  char buf[256];
  struct sockaddr_in srv_addr, clt_addr;
  int n;
  char clt_addr_ipv4[100];

  if (argc < 2) {
    fprintf(stderr, "USO: %s <puerto>\n", argv[0]);
    exit(0);
  }

  if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    error("ERROR en socket");

  bzero((char *) &srv_addr, sizeof(srv_addr));
  portno = atoi(argv[1]);
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_addr.s_addr = INADDR_ANY;
  srv_addr.sin_port = htons(portno);

  printf ("Puerto: %d\n", portno);
  
  if (bind(sd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) < 0)
    error("ERROR en bind");
printf ("sd: %d\n", sd); 

  listen(sd, 5);
  cltl = sizeof(clt_addr);

  while(1) {  
    if ((nsd = accept(sd, (struct sockaddr *) &clt_addr, &cltl)) < 0)
      error("ERROR en accept");

    printf("PID: %d - nsd: %d\n", getpid(), nsd);
	//printf ("nsd: %d\n", nsd); 

    pid = fork();
    if (pid>0) {
      close (nsd);
    } 
    else {
      inet_ntop(AF_INET, &(clt_addr.sin_addr), clt_addr_ipv4, 100);
      printf("Direccion IPv4 del cliente: %s\n", clt_addr_ipv4);

      memset(buf,0, 256);
    
      if((n = recv(nsd, buf, 255, 0)) <0)
        error("ERROR en recv");

      printf("Msg del cliente: %s\n", buf);
      sleep (10);

      if((n = send(nsd, "SVR recibio su msg", 18+1, 0)) < 0)
        error("ERROR en send");
    
      close(nsd);
      exit(0);
    }
  }  
}
