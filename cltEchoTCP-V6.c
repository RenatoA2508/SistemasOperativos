#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg) {
  perror(msg);
  exit(0);
}

int main(int argc, char *argv[]) {
  int sd, portno, n;
  struct sockaddr_in6 svr_addr;
  struct hostent *svr;
  char buf[256] = "Msg enviado desde el cliente!";
  char svr_addr_ipv6[100];

  if (argc < 3) {
    fprintf(stderr, "Usage: %s <host> <puerto>  \n", argv[0]);
    exit(0);
  }
  portno = atoi(argv[2]);

  sd = socket(PF_INET6, SOCK_STREAM, 0);
  if (sd < 0)
    error("ERROR en socket");

  svr = gethostbyname2(argv[1],AF_INET6);
  if (svr == NULL) {
    fprintf(stderr, "ERROR, host no existe\n");
    exit(0);
  }
  inet_ntop(AF_INET6, &(svr->h_addr), svr_addr_ipv6, 100);
  printf("Direccion IPv6 del servidor: %s\n", svr_addr_ipv6);

  memset((char *) &svr_addr, 0, sizeof(svr_addr));
  svr_addr.sin6_flowinfo = 0;
  svr_addr.sin6_family = AF_INET6;
  memmove((char *) &svr_addr.sin6_addr.s6_addr, (char *) svr->h_addr, svr->h_length);
  svr_addr.sin6_port = htons(portno);
  printf ("port: %d %d\n", portno, htons(portno)); 

  if (connect(sd, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) < 0)
    error("ERROR en connect");
  
  n = send(sd, buf, strlen(buf)+1, 0);
  if (n < 0)
    error("ERROR en send");

  memset(buf, 0, 256);

  n = recv(sd, buf, 255, 0);
  if (n < 0)
    error("ERROR en recv");
  printf("Msg del Servidor: %s\n", buf);

  close(sd);
  return 0;
}


