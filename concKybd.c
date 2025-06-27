// Dos procesos, padre e hijo leen concurrentemente del teclado
// Experimentar y analizar lo que ocurre.

#include <unistd.h>

int main (int argc, char *argv[]) {
  char c[128];
  int n, pid;

  pid = fork();

  while (1) { 
    n = read (0, &c, 128);

    if (pid) {
      write (1, "PADRE: ", 7);
      write (1, c, n);
    }
    else {
      write (1, "HIJO: ", 6);
      write (1, c, n);
    }
  }
}
