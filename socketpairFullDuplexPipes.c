/* 
 * Crear par de sockets conectados, fork y comunicarse con ellos.
 * Como PIPES pero full-duplex => send msgs en las dos direcciones
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DATA1 "Yo soy el mas correista . . ." 
#define DATA2 "Yo soy el mas morenista . . ."

int main(int argc, char *argv[]) { 
  int sockets[2]; 
  int pid;
  char buf[1024];

  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) { 
    perror("abriendo stream socket pair"); 
    exit(1); 
  }
  if ((pid = fork()) <  0) perror("fork"); 
  else if (pid) { // Padre
    close(sockets[0]); 
    if (read(sockets[1], buf, 1024) < 0) 
      perror("leyendo stream (padre)"); 
    printf("Padre -->%s\n", buf); 
    if (write(sockets[1], DATA2, sizeof(DATA2)) < 0) 
      perror("escribiendo stream (padre)"); 
    close(sockets[1]); 
  } 
  else { // Hijo
    close(sockets[1]); 
    if (write(sockets[0], DATA1, sizeof(DATA1)) < 0) 
      perror("escribiendo stream (hijo)"); 
    if (read(sockets[0], buf, 1024) < 0) 
      perror("leyendo stream (hijo)"); 
    printf("Hijo -->%s\n", buf);
    close(sockets[0]); 
  } 
  return 0;
} 
