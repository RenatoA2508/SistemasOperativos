/* memaddr.c -- direcciones de text, data, BSS, heap y stack

  * Imprime localizaciones de dos funciones main() y afunc()
  * Demuestra como la pila crece hacia abajo, afunc() imprime direccion de
    la variable (auto) stack_var.
  * Muestra la localizacion de la memoria asignada por alloca().
  * Imprime localizaciones de variables en "data" y BSS y de memoria asignada
    directamente por sbrk().
  ***   Mientras memaddr corre: pmap <pid de memaddr> 
  * Ref: http://www.phptr.com/articles/article.asp?p=173438&seqNum=2
*/

#include <stdio.h>
#include <malloc.h>     /* for definition of ptrdiff_t on GLIBC */
#include <unistd.h>
#include <alloca.h>     /* for demonstration only */

extern void afunc(void);    /* a function for showing stack growth */

int bss_var;            /* auto init to 0, should be in BSS */
int data_var = 42;      /* init to nonzero, should be data */

int main(int argc, char **argv) {
  char *p, *b, *nb;
  
  printf("PID: %d\n", getpid());
  printf("Localizaciones en text (codigo):\n");
  printf("\tDireccion de main: %p\n", main);
  printf("\tDireccion de afunc: %p\n", afunc);

  printf("Localizaciones en Stack:\n");
  afunc();

  p = (char *) alloca(32);
  if (p != NULL) {
    printf("\tInicio del arreglo asignado - alloca(): %p\n", p);
    printf("\tFin del arreglo asignado: %p\n", p + 31);
  }

  printf("Localizaciones en Data:\n");
  printf("\tDireccion de data_var: %p\n", &data_var);

  printf("Localizaciones en BSS:\n");
  printf("\tDireccion de bss_var: %p\n", & bss_var);

  b = sbrk((ptrdiff_t) 32);   /* aumentar el espacion de direcciones */
  nb = sbrk((ptrdiff_t) 0);
  printf("Localizaciones en Heap:\n");
  printf("\tFin initial del heap: %p\n", b);
  printf("\tNuevo fin del heap: %p\n", nb);

  b = sbrk((ptrdiff_t) -16);  /* contraer el heap */
  nb = sbrk((ptrdiff_t) 0);
  printf("\tUltimo fin del heap: %p\n", nb);

  while (1) {} /* loop infinito */
}

void afunc(void) {
  static int lev = 0;     /* nivel de recursion */
  auto int stack_var;    /* variable automatica, en stack */

  if (++lev == 3)       /* recursion controlada */
    return;

  printf("\tNivel de Stack %d: direccion de stack_var: %p\n", lev, & stack_var);
  afunc();          /* llamada recursiva */
}
