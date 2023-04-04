#include <iostream>
#include <unistd.h>
#include <chrono>
#include <sys/wait.h>
#include <algorithm>

#define RANGO_MIN 2
#define RANGO_MAX 1000000001

using namespace std;

int procesos;

bool esPar(int numero) {
    return (numero % 2) == 0;
}

int contarPares(int minimo, int maximo) {
    int cantidad = 0;
    for (int i = minimo; i < maximo; i++) {
        if (esPar(i)) {
            cantidad++;
        }
    }
    return cantidad;
}

void cerrarPipes(int pipes[][2], int i) {
    for (int j = 0; j < procesos; ++j) {
        if (j != i) {
            close(pipes[j][0]);
            close(pipes[j][1]);
        }
    }
}

void ejecutarHijo(int pipes[][2], int i) {
    cerrarPipes(pipes, i);

    int minimo;
    if (read(pipes[i][0], &minimo, sizeof(minimo)) == -1) {
        cout << "No se puede leer el pipe " << i << endl;
    }
    int maximo;
    if (read(pipes[i][0], &maximo, sizeof(maximo)) == -1) {
        cout << "No se puede leer el pipe " << i << endl;
    }

    // cout << i << ": minimo " << minimo << " maximo " << maximo << endl;

    int totalPares = contarPares(minimo, maximo);
    if (write(pipes[i][1], &totalPares, sizeof(totalPares)) == -1) {
        cout << "No se puede escribir el pipe " << i << endl;
    }
}

int main(int argc, char const *argv[]) {
    // Contar el tiempo de ejecución del programa
    auto start = chrono::steady_clock::now();

    // Parsing de los parámetros
    if (argc != 2) {
        cout << "Debe ejecutar con la cantidad de procesos N como parámetro. Ejemplo:" << endl;
        cout << argv[0] << " N" << endl;
        return 1;
    }
    procesos = atoi(argv[1]);
    int pipes[procesos][2];

    // Creacion de los N pipes
    for (int i = 0; i < procesos; ++i) {
        if (pipe(pipes[i]) == -1) {
            cout << "No se puede crear el pipe." << endl;
        }
    }

    // Creacion de los N hijos
    for (int i = 0; i < procesos; ++i) {
        int pid = fork();
        if (pid == -1) {
            cout << "No se puede forkear el proceso." << endl;
            return 1;
        }

        if (pid == 0) {
            ejecutarHijo(pipes, i);
            return 0;
        }
    }

    // El intervalo es: [RANGO_MIN, RANGO_MAX) (es decir, cerrado-abierto)
    int cantidad = ((RANGO_MAX - RANGO_MIN) + (procesos - 1)) / procesos;
    int inicio_rango = RANGO_MIN;
    for (int i = 0; i < procesos; i++) {
        int fin_rango = min(inicio_rango + cantidad, RANGO_MAX);
        if (write(pipes[i][1], &inicio_rango, sizeof(inicio_rango)) == -1) {
            cout << "No se puede escribir el pipe " << i << endl;
        }
        if (write(pipes[i][1], &fin_rango, sizeof(fin_rango)) == -1) {
            cout << "No se puede escribir el pipe " << i << endl;
        }
        inicio_rango = inicio_rango + cantidad;
    }
    
    for (int i = 0; i < procesos; ++i) {
        wait(NULL);
    }
    

    // Recoleccion de resultados
    int resultado = 0;
    for (int i = 0; i < procesos; ++i) {
        int resultado_i;
        if (read(pipes[i][0], &resultado_i, sizeof(resultado_i)) == -1) {
            cout << "No se puede leer el pipe " << i << endl;
        }
        // cout << "Resultado parcial para i " << i << " = " << resultado_i << endl;
        resultado = resultado + resultado_i;
    }

    // Impresion resultado total
    cout << "Resultado total: " << resultado << endl;
    auto end = chrono::steady_clock::now();
    cout << "Tiempo total (ms): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    return 0;
}