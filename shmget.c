// Obtiene un segmento de memoria comparitda
//
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int shmid;
    shmid = shmget(IPC_PRIVATE,1024,0777);
    if(shmid < 0) {
       printf("shmget is error n");
       return -1;
    }

    printf("Shared memory segment ID = %d\n", shmid);

    exit(0);
}
