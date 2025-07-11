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
  int sd, nsd, portno;
  socklen_t cltl;
  char buf[256];
  struct sockaddr_in6 srv_addr, clt_addr;
  int n;
  char clt_addr_ipv6[100];

  if (argc < 2) {
    fprintf(stderr, "USO: %s <puerto>\n", argv[0]);
    exit(0);
  }

  sd = socket(PF_INET6, SOCK_STREAM, 0);
  if (sd < 0)
    error("ERROR en socket");

  bzero((char *) &srv_addr, sizeof(srv_addr));
  portno = atoi(argv[1]);
  srv_addr.sin6_flowinfo = 0;
  srv_addr.sin6_family = AF_INET6;
  srv_addr.sin6_addr = in6addr_any;
  srv_addr.sin6_port = htons(portno);
  
  if (bind(sd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) < 0)
    error("ERROR en bind");

  listen(sd, 5);
  cltl = sizeof(clt_addr);

while(1) {  
  nsd = accept(sd, (struct sockaddr *) &clt_addr, &cltl);
  if (nsd < 0)
    error("ERROR en accept");

  inet_ntop(AF_INET6, &(clt_addr.sin6_addr), clt_addr_ipv6, 100);
  printf("Direccion IPv6 del cliente: %s\n", clt_addr_ipv6);

  memset(buf,0, 256);
  
  n = recv(nsd, buf, 255, 0);
  if (n < 0)
    error("ERROR en recv");

  printf("Msg del cliente: %s\n", buf);

  n = send(nsd, "SVR recibio su msg", 18+1, 0);
  if (n < 0)
      error("ERROR en send");
  
  close(nsd);
}  
}
