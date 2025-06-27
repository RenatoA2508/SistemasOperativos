// Comando cp con system call copy_file_range(); sin copiar a bufers del usuario. 
// Feature		Description
// copy_file_range()	Copies file data within the kernel (zero-copy).
// Supports offsets	Can copy specific file ranges.
// Efficient for large files	Avoids user-space memory allocation and memcpy overhead.

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SZ 1048576  // 1 MB per chunk

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

    // Open destination file
    int dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        perror("open (destination)");
        close(src_fd);
        return 1;
    }

    // Copy using copy_file_range()
    off_t src_offset = 0, dst_offset = 0;
    ssize_t n;

    while ((n=copy_file_range(src_fd, &src_offset, dst_fd, &dst_offset, BUF_SZ, 0)) > 0) {
    }

    if (bytes < 0) {
        perror("copy_file_range");
        close(src_fd);
        close(dst_fd);
        return 1;
    }

    close(src_fd);
    close(dst_fd);
    return 0;
}

