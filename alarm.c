// Signal SIGALARM despues de argv[1] secs

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig) {
    printf("Received signal %d\n", sig);
}

int main(int argc, char *argv[]) {
    if (argc!=2) {printf ("USO: %s <#_segundos>\n", argv[0]); exit(1);}
    signal(SIGALRM, handler);  // Registrar handler del signal
    alarm(atoi(argv[1]));      // Set timer, trigger SIGALRM despues de argv[1] secs
    printf("Esperando por alarma...\n");
    pause();                   // Bloquear proceso y esperar ALARM 
    printf("Despertado...\n");
    return 0;
}
