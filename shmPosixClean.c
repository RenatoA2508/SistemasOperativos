// shmPosixClean.c - elimina la memoria compartida
//
#include <stdio.h>
#include <sys/mman.h>

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE 4096

int main() {
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        return 1;
    }

    printf("Shared memory cleaned up.\n");
    return 0;
}
