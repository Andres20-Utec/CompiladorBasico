

#include "Funciones.h"

string removerEspacios(string line){// line : c = a + b;
    string newLine = ""; // c=a+b;
    for(int i = 0; i < line.size(); i++){ // c = a + b;
        if(line[i] != ' '){
            newLine += line[i];
        }
    }
    return newLine; // c=a+b;
}

string removerPuntoComa(string line){ // c=a+b;
    string newLine = "";
    for(int i = 0; i < line.size(); i++){
        if(line[i] != ';'){
            newLine += line[i];
        }
    }
    return newLine;// c=a+b
}