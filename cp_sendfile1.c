// Comando cp systema call sendfile(): transferencia zero-copy entre archivos.
// source: archivo regular; destino: archivo regular file o socket.
// includes manejo de errores; preserva permisos del archivo

#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source_file destination_file\n", argv[0]);
        return 1;
    }

    const char *src_path = argv[1];
    const char *dst_path = argv[2];

    // Open source file
    int src_fd = open(src_path, O_RDONLY);
    if (src_fd < 0) {
        perror("open (source)");
        return 1;
    }

    // Get size of source file
    struct stat stat_buf;
    if (fstat(src_fd, &stat_buf) < 0) {
        perror("fstat");
        close(src_fd);
        return 1;
    }

    // Open destination file
    int dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, stat_buf.st_mode);
    if (dst_fd < 0) {
        perror("open (destination)");
        close(src_fd);
        return 1;
    }

    // Use sendfile to copy data from src_fd to dst_fd
    off_t offset = 0;
    ssize_t bytes_sent;

    while (offset < stat_buf.st_size) {
        bytes_sent = sendfile(dst_fd, src_fd, &offset, stat_buf.st_size - offset);
        if (bytes_sent <= 0) {
            if (errno == EINTR)
                continue;
            perror("sendfile");
            close(src_fd);
            close(dst_fd);
            return 1;
        }
    }

    close(src_fd);
    close(dst_fd);
    return 0;
}

