#include <unistd.h>
#include <stdio.h>
#include <malloc.h>

int bss_var;
int data_var;

void func1() {
  int stack_var;
  printf ("stack_var: %p\n", &stack_var);
}

int main () {
  int main_var;
  char *p, *b, *nb;

  printf ("main: %p\n", main);
  printf ("func: %p\n", func1);
  func1();
  p = (char *) malloc(32);
  if (p != NULL) {
    printf("\tInicio del arreglo asignado - alloca(): %p\n", p);
    printf("\tFin del arreglo asignado: %p\n", p + 31);
  }
  printf ("data_var: %p\n", &data_var);
  printf ("bss_var: %p\n", &bss_var);

  b = sbrk((ptrdiff_t) 32);   /* aumentar el espacion de direcciones */
  nb = sbrk((ptrdiff_t) 0);
  printf("Localizaciones en Heap:\n");
  printf("\tFin initial del heap: %p\n", b);
  printf("\tNuevo fin del heap: %p\n", nb);
  b = sbrk((ptrdiff_t) -16);  /* contraer el heap */
  nb = sbrk((ptrdiff_t) 0);
  printf("\tUltimo fin del heap: %p\n", nb);
  

  while (1) {}
}



