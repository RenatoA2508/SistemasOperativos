#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
  int fd;
  char buf[100];

  fd = open(argv[1], O_RDONLY);
  sleep (10);
  read (fd, buf, 10);
  write (1, buf, 10);
}
