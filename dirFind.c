// Buscar archivo en directorio
// USO: dirAPI <dir_name> <file_name>
// struct dirent {
//   ino_t          d_ino;       /* Inode number */
//   off_t          d_off;       /* Not an offset; see below */
//   unsigned short d_reclen;    /* Length of this record */
//   unsigned char  d_type;      /* Type of file; not supported by all FS types */
//   char           d_name[256]; /* Null-terminated filename */
// };

#include <sys/types.h>
#include <dirent.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int exists(char *directory, char *name) {
  struct dirent *dp;
  DIR *dirp = opendir(directory);
  while ((dp = readdir(dirp)) != NULL) {
    printf ("%ld   %s\n", dp->d_ino, dp->d_name);
    if (!strcmp(dp->d_name, name)) {
      closedir(dirp);
      return 1; /* Found */
    }
  }
  return 0; /* Not Found */
  closedir(dirp);
}

int main (int argc, char *argv[]) {
  if (argc !=3) {printf ("USO: %s <dir> <file>\n", argv[0]); exit (1);}
  if (exists (argv[1], argv[2]))
    printf ("%s is in %s\n", argv[2], argv[1]);
  else 
    printf ("%s is NOT in %s\n", argv[2], argv[1]);
}
