// Grabar datos en archivo, con O_SYNC. Utiliza el buffer del kernel (page cache). 
// Cada write es flushed al disco antes de retornar. 
// Comparar con O_DIRECT:
// Feature		O_SYNC				O_DIRECT
// Bypasses page cache	❌ No				✅ Yes
// Flushes to disk	✅ Yes (writes are durable)	✅ Yes (depends on FS/driver)
// Requires alignment	❌ No				✅ Yes (aligned buff, sz, offset)
// Performance		Medium (depends on disk speed)	Fast: large I/O, slow: small I/O

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 4096

int main() {
    const char *filename = "sync_output.bin";
    int fd;

    // Open the file with O_SYNC to force synchronous writes
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Allocate a regular (non-aligned) buffer
    char *buf = malloc(BUF_SIZE);
    if (!buf) {
        perror("malloc");
        close(fd);
        return 1;
    }

    // Fill buffer with data
    memset(buf, 'B', BUF_SIZE);

    // Write buffer to file
    ssize_t written = write(fd, buf, BUF_SIZE);
    if (written < 0) {
        perror("write");
        free(buf);
        close(fd);
        return 1;
    }

    printf("Wrote %zd bytes to %s using O_SYNC\n", written, filename);

    free(buf);
    close(fd);
    return 0;
}

