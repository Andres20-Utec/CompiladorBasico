

#include "Compiler.h"

enum Register { A = 100, B = 101, C = 102, D = 103, E = 104, F = 105, G = 106, H = 107 };

// Constructor vacío
Compiler::Compiler() {
    // Inicializar el mapa de registros
    registerMap['a'] = A;
    registerMap['b'] = B;
    registerMap['c'] = C;
    registerMap['d'] = D;
    registerMap['e'] = E;
    registerMap['f'] = F;
    registerMap['g'] = G;
    registerMap['h'] = H;

    usedRegisters['a'] = false;
    usedRegisters['b'] = false;
    usedRegisters['c'] = false;
    usedRegisters['d'] = false;
    usedRegisters['e'] = false;
    usedRegisters['f'] = false;
    usedRegisters['g'] = false;
    usedRegisters['h'] = false;

    // vector
    for(int i = 0; i < 8; i++){
        registersValues[i] = {"0"};
    }
}

// fstream -> ifstream, ofstream

void Compiler::compile(const std::string& inputFile, const std::string& outputFile) {
    // Abrir el archivo de entrada
    std::ifstream input(inputFile);
    // Abrir el archivo de salida
    std::ofstream output(outputFile);
    std::string line;
    vector<string> lines;
    // Leer el archivo línea por línea
    while (std::getline(input, line)) {
        // Remover los espacios y punto y coma de la línea
        lines.push_back(removerPuntoComa(removerEspacios(line))); // func(func1(linea)) -> func(linea2) -> "c=a+b"
    }

    // Obtener el resultado de compilar las líneas del archivo
    vector<string> resultado =  compileLines(lines);

    // Escritura a .txt
    for(int i = 0; i < resultado.size(); i++){
        output << resultado[i] << endl;
    }

    // Cerrar el archivo de entrada y salida
    input.close();
    output.close();
}

vector<string> Compiler::compileLines(vector<string> lines){
    vector<int> whilePosition = {}; // Guardar la posición de los whiles  {0}
    int num_whiles = 0; // Contar el número de whiles  | num_whiles = 1
    vector<string> nuevaslineas; // Guardar las nuevas líneas de c++ a assembly
    int i = 0;
    while(i < lines.size()){
        string line = lines[i]; // Obtener la linea -> }
        // While -> jmp
        if(line.substr(0, 5) == "while"){
            whilePosition.push_back(i);
            num_whiles++;
        }
        // Cin -> in | load
        if(line.substr(0, 3) == "cin"){
            pair<string, string> input = leerInput(line, usedRegisters); //cin>>a -> in 100 | load a, 100
            vector<string> loads;
            nuevaslineas.push_back(input.first); // {"in 100",}
            loads.push_back(input.second); // {"load a, 100",}
            while(i < lines.size() && lines[i + 1].substr(0, 3) == "cin"){ // i = 2 || i + 1 = 3
                i++; // i = 2
                line = lines[i]; //"cin>>b"
                input = leerInput(line, usedRegisters); //cin>>b -> in 101 | load b, 101
                nuevaslineas.push_back(input.first); // {"in 100", "in 101"}
                loads.push_back(input.second); // {"load a, 100", "load b, 101"}
            }
            // {"in 100", "in 101", "load a, 100", "load b, 101"}
            for(int j = 0; j < loads.size(); j++) {
                nuevaslineas.push_back(loads[j]);
            }
        }
        // Add and Store | c = a + b -> c=a+b
        if(line[1] == '=' && line[3] == '+'){
            char variable = line[0];
            char variable1 = line[2];
            char variable2 = line[4];
            // add c, a, b
            string addLine = "add " + string(1, variable) + ", " + string(1, variable1) + ", " + string(1, variable2);
            // store c, 102
            string storeLine = "store " + string(1, variable) + ", " + to_string(registerMap[variable]);
            nuevaslineas.push_back(addLine);
            nuevaslineas.push_back(storeLine);
            usedRegisters[variable] = true;
        }
        //{1 : "Lucciana", 2 : "Luis"}
        //{'a' : 100, 'b' : 101, 'c' : 102} diccionario

        // Multiply and Store | c = a * b
        if(line[1]=='=' && line[3] == '*'){
            char variable = line[0];
            char variable1 = line[2];
            char variable2 = line[4];
            string mulLine = "mul " + string(1, variable) + ", " + string(1, variable1) + ", " + string(1, variable2);
            string storeLine = "store " + string(1, variable) + ", " + to_string(registerMap[variable]);
            nuevaslineas.push_back(mulLine);
            nuevaslineas.push_back(storeLine);
            usedRegisters[variable] = true;
            registersValues[variable - 97] = {to_string(stoi(registersValues[variable1 - 97][0]) * stoi(registersValues[variable2 - 97][0]))};
        }
        //  Mvi and Store | c=5
        if(line[1] == '=' && isdigit(line[line.length() - 1])){
            char variable = line[0];
            string mviLine = "mvi " + string(1, variable) + ", " + line.substr(2, line.length() - 2);
            string storeLine = "store " + string(1, variable) + ", " + to_string(registerMap[variable]);
            // Coloca true en el mapa de registros usados
            usedRegisters[variable] = true;
            // Guardar el valor en el vector de valores de los registros
            registersValues[variable - 97] = {line.substr(2, line.length() - 2)};
            nuevaslineas.push_back(mviLine);
            nuevaslineas.push_back(storeLine);
        }

        // mov <reg> <reg> | c=a -> mov a, c
        if(line.length() == 3 && line[1] == '='){
            char variable = line[0]; // c
            char variable1 = line[2]; // a
            // mov a,c
            string movLine = "mov " + string(1, variable1) + ", " + string(1, variable);
            // Coloca true en el mapa de registros usados
            usedRegisters[variable] = true;
            // Guardar el valor en el vector de valores de los registros
            //  {"10", "0", "10", "0", "0", "0", "0", "0"}
            // 'c' - 97 -> 99 - 97 = 2 | a -> 97 - 97 = 0
            registersValues[variable - 97] = registersValues[variable1 - 97];
            nuevaslineas.push_back(movLine);
        }

        // ++a | a++
        if(line.length() == 3 && line[1] == '+'){
            char variable = line[0]; //"a++"
            if(line[0] == '+'){
                variable = line[2]; //"++a"
            }
            string incLine = "inc " + string(1, variable); // inc a
            // Coloca true en el mapa de registros usados
            usedRegisters[variable] = true;
            // Guardar el valor en el vector de valores de los registros
            //registersValues[variable - 97] = {to_string(stoi(registersValues[variable - 97][0]) + 1)};
            nuevaslineas.push_back(incLine);
        }
        // --a | a--
        if(line.length() == 3 && line[1] == '-'){
            char variable = line[0]; //"a--"
            if(line[0] == '+'){
                variable = line[2]; //"--a"
            }
            string decLine = "dec " + string(1, variable);
            // Coloca true en el mapa de registros usados
            usedRegisters[variable] = true;
            // Guardar el valor en el vector de valores de los registros
            //registersValues[variable - 97] = {to_string(stoi(registersValues[variable - 97][0]) - 1)};
            nuevaslineas.push_back(decLine);
        }

        // Out 102
        if(line.substr(0, 4) == "cout"){
            char variable = line[line.length() - 1];
            string outLine = "out " + to_string(registerMap[variable]);
            nuevaslineas.push_back(outLine);
        }
        // JMP
        if(line[0] == '}'){
            nuevaslineas.push_back("jmp " + to_string(whilePosition[num_whiles - 1]));
            num_whiles--;
        }
        i++;
    }
    return nuevaslineas;
}

// cin>>a -> in 100 | load a, 100
pair<string, string> Compiler::leerInput(string line,unordered_map<char, bool>& usedRegisters){
    char variable = line[line.length() - 1]; // a   'a' -> "a"
    int registerNumber = registerMap[variable]; // 100
    string in = "in " + to_string(registerNumber);
    string load = "load " + string(1, variable) + " " +  to_string(registerNumber); // 100 -> "100"
    // Coloca true en el mapa de registros usados
    usedRegisters[variable] = true;
    return make_pair(in, load); // pair<in 100, load a, 100>
}