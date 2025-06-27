// Acceso a metadatos de archivo (inodo)

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main (int argc, char *argv[]) {
   FILE *file = fopen(argv[1], "r");
   int fd = fileno(file); 
   struct stat s;
   fstat(fd, & s);
   if (S_ISDIR( s.st_mode)) printf ("%s is a directory\n", argv[1]);
   if (S_ISREG( s.st_mode)) printf ("%s is a regular file\n", argv[1]);
   printf("Last accessed %s", ctime(&s.st_atime));
}
