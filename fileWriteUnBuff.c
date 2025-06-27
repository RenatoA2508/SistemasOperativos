// Graba datos en archivo SIN buffering en el kernel- flag O_DIRECT. Bypass el page cache.
// O_DIRECT minimizes cache effects by performing direct I/O. Requerimientos estrictos:
// Bufer aligned al FS block size (512 o 4096 bytes).
// Size y offset deben ser multiplos de block size.
// Usar posix_memalign() para asignar memoria debidamente aligned.
// Verificar que utiliza E/S directa (sin cache), con strace u observando que no hay
// actividad en el page cache (/proc/meminfo, iostat).

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BLOCK_SIZE 4096

int main() {
    const char *filename = "direct_output.bin";
    int fd;

    // Open file with O_DIRECT (no kernel buffering)
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC | O_DIRECT, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Allocate aligned memory
    void *buf;
    if (posix_memalign(&buf, BLOCK_SIZE, BLOCK_SIZE) != 0) {
        perror("posix_memalign");
        close(fd);
        return 1;
    }

    // Fill buffer with data (e.g., 'A's)
    memset(buf, 'A', BLOCK_SIZE);

    // Write to file
    ssize_t written = write(fd, buf, BLOCK_SIZE);
    if (written < 0) {
        perror("write");
        free(buf);
        close(fd);
        return 1;
    }

    printf("Wrote %zd bytes to %s using O_DIRECT\n", written, filename);

    free(buf);
    close(fd);
    return 0;
}

