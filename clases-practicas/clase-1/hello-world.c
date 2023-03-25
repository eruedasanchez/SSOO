/* Inicializamos C con un hello world */

#include <unistd.h>

int main(int argc, char* argv[]){
    write(1,"Hola SO!\n",9);
    return 0;
}
