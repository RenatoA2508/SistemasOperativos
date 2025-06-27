// Permite grabar con: 
// O_SYNC (sync, buffered),
// O_DIRECT (unbuffered, bypasses kernel page cache), or
// both O_SYNC | O_DIRECT.
// Seleccionar el modo en el argumento CLI
//    ./flexible_writer file_sync.bin sync
//    ./flexible_writer file_direct.bin direct
//    ./flexible_writer file_both.bin both
// Monitorear:
//    strace ./flexible_writer ... to confirm syscalls.
//    hdparm --direct or iostat to observe disk activity.

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BLOCK_SIZE 4096

void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <filename> [sync|direct|both]\n", prog);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc < 3) usage(argv[0]);

    const char *filename = argv[1];
    const char *mode_str = argv[2];

    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    int use_direct = 0;

    if (strcmp(mode_str, "sync") == 0) {
        flags |= O_SYNC;
    } else if (strcmp(mode_str, "direct") == 0) {
        flags |= O_DIRECT;
        use_direct = 1;
    } else if (strcmp(mode_str, "both") == 0) {
        flags |= O_SYNC | O_DIRECT;
        use_direct = 1;
    } else {
        usage(argv[0]);
    }

    // Open the file
    int fd = open(filename, flags, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Allocate buffer
    void *buf;
    if (use_direct) {
        if (posix_memalign(&buf, BLOCK_SIZE, BLOCK_SIZE) != 0) {
            perror("posix_memalign");
            close(fd);
            return 1;
        }
    } else {
        buf = malloc(BLOCK_SIZE);
        if (!buf) {
            perror("malloc");
            close(fd);
            return 1;
        }
    }

    // Fill buffer with data
    memset(buf, 'X', BLOCK_SIZE);

    // Write to file
    ssize_t written = write(fd, buf, BLOCK_SIZE);
    if (written < 0) {
        perror("write");
        free(buf);
        close(fd);
        return 1;
    }

    printf("Wrote %zd bytes to %s using mode: %s\n", written, filename, mode_str);

    free(buf);
    close(fd);
    return 0;
}

