#include "Compiler.h"

int main() {
    // Crear el compilador
    Compiler compiler;

    // Compilar el primer código de ejemplo
    compiler.compile("input.txt", "output.txt");

    return 0;
}