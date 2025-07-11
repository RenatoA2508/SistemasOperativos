// shmPosixR.c - abree shm existente y lee el mensaje
//
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define SHM_NAME "/my_shared_memory"
#define SHM_SIZE 4096

int main() {
    int fd = shm_open(SHM_NAME, O_RDONLY, 0666);
    if (fd == -1) {
        perror("shm_open");
        return 1;
    }

    void *ptr = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    printf("Reader: message read: \"%s\"\n", (char *)ptr);

    munmap(ptr, SHM_SIZE);
    close(fd);

    return 0;
}

