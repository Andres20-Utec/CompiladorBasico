
#ifndef COMPILADORBASICO_COMPILER_H
#define COMPILADORBASICO_COMPILER_H

#include "Funciones.h"
// unordered_map o un map -> Profe, los dos funcionan como un diccionario en python, es por eso que los usamos.

class Compiler {
public:
    Compiler(); // Constructor vacío
    void compile(const std::string& inputFile, const std::string& outputFile); // Función para compilar el código

private:
    // Mapa de registros: char -> int (ASCII) (a -> 100, b -> 101, ...)
    unordered_map<char, int> registerMap;
    // Mapa de registros usados: char -> bool (a -> false, b -> false, ...)
    unordered_map<char, bool> usedRegisters;
    // Vector de valores de los registros: int -> vector<string> (100 -> ["1"], 101 -> ["4"], ...)
    vector<string> registersValues[8];

    vector<string> compileLines(vector<string> lines);
    // Función para leer cin y obtener el assembly de in | load
    // cin >> a; -> pair<in 100, load a, 100>    usedRegisters['a'] = true;
    pair<string, string> leerInput(string line,unordered_map<char, bool>& usedRegisters);
};

#endif //COMPILADORBASICO_COMPILER_H
