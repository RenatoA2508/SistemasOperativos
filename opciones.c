// Obtener las opciones de un socket.
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
  int i, j;
  int sd;
  int numopts = 21;
  socklen_t lonopc;
  char valopc[1024];
  int level = SOL_SOCKET;

  if (argc == 2) level = atoi(argv[1]);
  if (argc == 3) numopts = atoi(argv[2]);
printf ("%d %d\n", level, numopts);
  sd = socket (PF_INET, SOCK_STREAM, 0);

  for (i=1; i<=numopts; i++) {
    lonopc = sizeof (valopc);
    if (getsockopt (sd, level, i, (void *)&valopc, &lonopc) == -1) 
      perror ("GETSOCKOPT");
    else  {
      printf ("OPCION (%d): ", i);
      for (j = 0; j<lonopc; j+=4)
        printf ("  %d", *(int *)(valopc + j));
      printf ("\n");
    }
  }
}
