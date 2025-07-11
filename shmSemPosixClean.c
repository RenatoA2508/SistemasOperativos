// shmSemPosixClean.c: destroy semaphores and unlink the segment
//
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "shmSemPosixCommon.h"

int main(void) {
    int fd = shm_open(SHM_NAME, O_RDWR, 0);
    if (fd != -1) {
        shared_data_t *data = mmap(NULL, sizeof(*data),
                                   PROT_READ | PROT_WRITE,
                                   MAP_SHARED, fd, 0);
        if (data != MAP_FAILED) {
            sem_destroy(&data->sem_empty);
            sem_destroy(&data->sem_full);
            munmap(data, sizeof(*data));
        }
        close(fd);
    }

    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        return 1;
    }
    puts("Shared memory and semaphores cleaned up.");
    return 0;
}

