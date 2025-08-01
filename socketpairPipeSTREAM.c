// PIPES con socketpair. Utiliza SOCK_STREAM
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

int main (int argc, char *argv[]) {
  int sv[2];
  char buf[1024];

  if (socketpair (PF_UNIX, SOCK_STREAM, 0, sv) < 0) {
    printf ("SOCKETPAIR\n");
    exit (1);
  }
  if (fork () > 0) { // Padre
    close (sv[1]);
    write (sv[0], "HOLA", 5);
    write (sv[0], "MUNDO", 5);
    exit (0);

  }
  close (sv[0]);
  read (sv[1], buf, sizeof(buf));
  printf ("HIJO: %s\n", buf);
  exit (0);
}
