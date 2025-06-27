#include <sys/types.h>
#include <dirent.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void dirlist(char *path) {
   struct dirent *dp;
   struct stat statbuf;
   DIR *dirp = opendir(path);
   if (!dirp) {perror("Could not open directory"); return; }
   printf ("PATH: %s\n", path);
   while ((dp = readdir(dirp)) != NULL) {
      char newpath[strlen(path) + strlen(dp->d_name) + 2];
      sprintf(newpath,"%s/%s", path, dp->d_name);
      printf("%s\n", dp->d_name);
      //if (0 == stat(newpath,&s) && S_ISDIR(s.st_mode)) dirlist(newpath)
      if (stat(newpath, &statbuf) != 0) {
	printf ("Couldn't stat %s\n", path);
        exit(1);
      }
      if (S_ISDIR(statbuf.st_mode))
         if (!(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))) {
        dirlist(newpath);
      }
   }
   closedir(dirp);
}
int main(int argc, char **argv) {
   dirlist(argv[1]);
   return 0;
}
