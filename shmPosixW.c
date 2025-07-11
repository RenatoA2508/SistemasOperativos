// shmPosixW.c - Crea y escribe un msg en la memoria compartida (POSIX)
//
#include <stdio.h>
#include <fcntl.h>    // O_CREAT, O_RDWR
#include <sys/mman.h> // shm_open, mmap
#include <unistd.h>   // ftruncate, close
#include <string.h>
#include <sys/stat.h>        /* For mode constants */

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE 4096

int main() {
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    if (ftruncate(fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        return 1;
    }

    void *ptr = mmap(NULL, SHM_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    const char *message = "Dayana hazme caso pls";
    memcpy(ptr, message, strlen(message) + 1);

    printf("Writer: message written.\n");

    munmap(ptr, SHM_SIZE);
    close(fd);

    return 0;
}

