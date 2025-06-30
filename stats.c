// Acceso a metadatos de archivo (inodo)

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
int main (int argc, char *argv[]) {
   FILE *file = fopen(argv[1], "r");
   int fd = fileno(file); 
   struct stat s;
   fstat(fd, & s);
   if (S_ISDIR( s.st_mode)) printf ("%s is a directory\n", argv[1]);
   if (S_ISREG( s.st_mode)) printf ("%s is a regular file\n", argv[1]);
   printf("Last accessed %s", ctime(&s.st_atime));
   printf("Logical Length %s", ctime(&s.st_atime));
   struct passwd *pw = getpwuid(s.st_uid);
    struct group  *gr = getgrgid(s.st_gid);
    printf("Owner UID: %d (%s)\n", s.st_uid, pw ? pw->pw_name : "unknown");
    printf("Group GID: %d (%s)\n", s.st_gid, gr ? gr->gr_name : "unknown");

    // Size and blocks
    printf("Size (bytes): %ld\n", s.st_size);
    printf("Blocks allocated: %ld\n", s.st_blocks);
    printf("Block size (I/O): %ld\n", s.st_blksize);

    // Link count
    printf("Hard links: %ld\n", s.st_nlink);

    // Timestamps
    printf("Access time : %s", ctime(&s.st_atime));
    printf("Modify time : %s", ctime(&s.st_mtime));
    printf("Change time : %s", ctime(&s.st_ctime));
}
