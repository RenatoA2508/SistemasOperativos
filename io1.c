#include <unistd.h>
int main(int argc, char *argv[]){
	char c; //Recibir caracter del teclado
	read(0, &c, 1);
	c += 1;
	write(1, &c, 1);

}
