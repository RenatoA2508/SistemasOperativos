// Experimentar con layout de MEM; imprime direcciones VIRTUALES de:
// variables locales, globales y estaticas inicializadas y no inicializadas
// Las direcciones VIRTUALES pueden ser utilizadas en el programa virt2phys
// USO: ./memLayout 
//   *** size memLayout - layout de memoria (text, data, bss)

#include <stdio.h>
#include <unistd.h>

int data = 10; // variable en DATA
int bss; // variable en BSS
 
int main(void) {
  static int staticInit = 100; // static variable en DATA
  static int staticZ; // static variable en BSS
  int local;     // variable local en stack

  int intV;
  float floatV;
  char charV;

  printf("direccion de main():     %p\n", main);
  printf("direccion de data:       %p\n", &data);
  printf("direccion de bss:        %p\n", &bss);
  printf("direccion de staticInit: %p\n", &staticInit);
  printf("direccion de staticZ:    %p\n", &staticZ);
  printf("direccion de local:      %p\n", &local);
  printf("direccion de intV:       %p\n", &intV);
  printf("direccion de floatV:     %p\n", &floatV);
  printf("direccion de charV:      %p\n", &charV);

  char a=0, b=0;

  printf("a= %d;  b= %d\n", a, b);
  int *p = (int*)&b;
  *p = 258;
  printf("a= %d;  b= %d\n", a, b);

  printf ("PID: %d\n", getpid());
  while (1) sleep (10);

}
