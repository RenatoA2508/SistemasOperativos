#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
  int *b1, *b2, *b3;
  void *sbrk1, *sbrk2, *sbrk3, *sbrk4;

  if (argc == 5) printf ("dir st: %p\n", &b1);

  if (argc < 4) {printf ("USO: %s <size1> <size2> <size3>\n", argv[0]); exit (1);}
  sbrk1 = sbrk(0);
  printf ("sbrk 1: %p\n", sbrk1);
  
  b1 = (int *)malloc (atoi(argv[1]));
  printf ("dir b1: %p (%ld)\n", b1, (void *)b1 - sbrk1);
  sbrk2 = sbrk(0);
  printf ("sbrk 2: %p (%ld)\n", sbrk2, sbrk2 - sbrk1);
 
  b2 = (int *)malloc (atoi(argv[2]));
  printf ("dir b2: %p (%ld)\n", b2, (void *)b2 - sbrk1);
  sbrk3 = sbrk(0);
  printf ("sbrk 3: %p (%ld)\n", sbrk3, sbrk3 - sbrk1);

  b3 = (int *)malloc (atoi(argv[3]));
  printf ("dir b3: %p (%ld)\n", b3, (void *)b3 - sbrk1);
  sbrk4 = sbrk(0);
  printf ("sbrk 4: %p (%ld)\n", sbrk4, sbrk4 - sbrk1);

}
