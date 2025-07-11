// shmSemPosixR.c - abre el segmento shm y lee el msg
// 
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "shmSemPosixCommon.h"

int main(void) {
    int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (fd == -1) { perror("shm_open"); return 1; }

    shared_data_t *data = mmap(NULL, sizeof(*data),
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) { perror("mmap"); return 1; }

    /* Wait until writer signals that a message is ready */
    sem_wait(&data->sem_full);
    printf("Reader received: \"%s\"\n", data->message);
    sem_post(&data->sem_empty);   /* Mark buffer empty again */

    munmap(data, sizeof(*data));
    close(fd);
    return 0;
}

