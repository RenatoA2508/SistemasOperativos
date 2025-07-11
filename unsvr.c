// Servidor de aplicacion C/S con sockets UNIX
// Lee msgs de los clientes, espera 10 segs y devuelve el msg al cliente
// Es un servidor secuencial, orientado a conexion

#include "uncltsvr.h"

int read_stream();

int main (int argc, char *argv[]) {
  struct sockaddr_un svr, clt;
  char request[REQUEST], reply[REPLY];
  int sd, nsd, n, cltlen;

  if (argc != 2) {
    printf ("USO: %s <path de archivo UNIX>\n", argv[0]);
    exit (1);
  } 

  if ((sd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
    printf ("Error: socket\n");
    exit (1);
  }

  memset (&svr, 0, sizeof (svr));
  svr.sun_family = AF_UNIX;
  printf ("archivo: %s\n", argv[1]);
  strncpy (svr.sun_path, argv[1], sizeof (svr.sun_path));

  if (bind (sd, (struct sockaddr *)&svr, sizeof (svr)) < 0) {
    printf ("Error: bind\n");
    exit (1);
  }
  if (listen (sd, SOMAXCONN) < 0) {
    printf ("Error: listen\n");
    exit (1);
  }
  for (;;) {
    cltlen = sizeof (clt);
    if ((nsd = accept(sd, (SA)&clt, &cltlen)) < 0) {
      printf ("Error: accept\n");
      exit (1);
    }
    if ((n = read_stream (nsd, request, REQUEST)) < 0) {
      printf ("Error: read\n");
      exit (1);
    }
    printf ("NSD: %d\n", nsd);
    //sleep (2);
    strcpy (reply, request);
    reply[0] = 'X';
    if (write (nsd, reply, REPLY) != REPLY) {
      printf ("Error: write\n");
      exit (1);
    }
//    close (nsd);
  }
}

int read_stream (int sd, char *ptr, int maxbytes) {
  int nleft, nread;

  nleft = maxbytes;
  while (nleft > 0) {
    if ((nread = read (sd, ptr, nleft)) < 0)
      return (nread);
    else if (nread == 0)
      break;
    nleft -= nread;
    ptr += nread;
  }
  return (maxbytes - nleft);
}
