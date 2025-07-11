// shmSemPosixW.c - ejecutar primero. Crea/abre el segmento shm y escribe un msg
//
#include <stdio.h>
#include <fcntl.h>      /* O_* flags */
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "shmSemPosixCommon.h"

int main(void) {
    int fd;
    int just_created = 0;

    /* Try to create a fresh segment first */
    fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (fd == -1 && errno == EEXIST) {
        /* Already exists: just open it */
        fd = shm_open(SHM_NAME, O_RDWR, 0666);
    } else {
        just_created = 1;
    }
    if (fd == -1) { perror("shm_open"); return 1; }

    if (ftruncate(fd, sizeof(shared_data_t)) == -1) {
        perror("ftruncate"); return 1;
    }

    shared_data_t *data = mmap(NULL, sizeof(*data),
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) { perror("mmap"); return 1; }

    /* Initialise semaphores only the first time the segment is created */
    if (just_created) {
        if (sem_init(&data->sem_empty, 1, 1) == -1 ||
            sem_init(&data->sem_full,  1, 0) == -1) {
            perror("sem_init"); return 1;
        }
    }

    /* Write the message */
    const char *msg = "Hello from writer with POSIX semaphores!";
    sem_wait(&data->sem_empty);              /* Wait for buffer to be empty */
    strncpy(data->message, msg, MESSAGE_SIZE - 1);
    data->message[MESSAGE_SIZE - 1] = '\0';  /* Ensure NUL‑termination */
    sem_post(&data->sem_full);               /* Signal reader */

    printf("Writer: message sent.\n");

    munmap(data, sizeof(*data));
    close(fd);
    return 0;
}

