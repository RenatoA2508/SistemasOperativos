// Implementacion del cmd "kill"
// USO: ./kill pid signal
//
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc,char *argv[]) {
    pid_t pid;
    int sig;
    
    if(argc < 3) {
        printf("USO:%s <pid> <signal>\n",argv[0]);
        return -1;
    }
    // Convertir strings a int
    sig = atoi(argv[2]);
    pid = atoi(argv[1]);
    
    kill(pid,sig);
    
    return 0;
}
