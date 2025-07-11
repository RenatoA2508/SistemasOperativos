// Cliente de aplicacion C/S utilizando sockets UNIX
// Envia un msg al servidor, el cual lo devuelve despues de 10 segs.

#include "./uncltsvr.h"

int main (int argc, char *argv[]) {
  struct sockaddr_un svr;
  char request[REQUEST], reply[REPLY];
  int sd, n;

  if (argc != 3) {
    printf ("USO: %s <path de archivo UNIX> <msg>\n", argv[0]);
    exit (1);
  }
  if ((sd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
    printf ("Error: socket\n");
    exit (1);
  }

  memset (&svr, 0, sizeof (svr));
  svr.sun_family = AF_UNIX;
  strncpy (svr.sun_path, argv[1], sizeof (svr.sun_path));

  if (connect (sd, (SA)&svr, sizeof(svr)) < 0) {
    printf ("Error: connect\n");
    exit (1);
  }
  strncpy (request, argv[2], REQUEST);
  if (write (sd, request, REQUEST) != REQUEST) {
    printf ("Error: write\n");
    exit (1);
  }
  printf ("RQST: %s\n", request);

  if (shutdown(sd, 1) < 0) {
    printf ("Error: shutdown\n");
    exit (1);
  }

  if ((n = read (sd, reply, REPLY)) < 0){
    printf ("Error: read\n");
    exit (1);
  }
  printf ("RPLY: %s\n", reply);
  close (sd);

  exit (0);
}
