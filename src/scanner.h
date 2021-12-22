#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#define SHELL_TOK_DELIM " " 

using namespace std;

const string keywords[10] = { "string","int","float","bool","tree","if","else","for","avl","simple"};
const string functions_name[3] = { "eliminar_arbol","visualizar","exportar"};
const string delim[5] = { "(",")","{","}" ,";"};
const string ops[10] = { "=","+","-","*","/","<",">","!","%","." };
vector<pair<string, string>> list_tokens;
map<string, int> asociation;
int lines_cont = 1;

bool isNumber(string s) {
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;

    return true;
}

bool symbolCheck(char symbol) {
    // CHECKING FOR ALPHABET
    if ((symbol >= 65 && symbol <= 90) || (symbol >= 97 && symbol <= 122))
        return 0;

    // CHECKING FOR DIGITS
    else if (symbol >= 48 && symbol <= 57)
        return 0;

    // OTHERWISE SPECIAL CHARACTER
    else
        return 1;
}

void scanner(vector<string> splited) {
    //Valores, Palabras Reservadas, Identificadores, Funciones, Simbolos
    if (splited.empty() || splited[0].compare("#") == 0) {
        return;
    }

    int i = 0;
    bool rw_flag = 0; bool fn_flag = 0; bool op_flag = 0; bool sim_flag = 0;

    for (int j = 0; j < splited.size(); ++j) {//analiza toda la linea recibida

        if (isNumber(splited[j])) {//verifica si es un valor numerico
            list_tokens.push_back(make_pair("number_literal", splited[j]));
        }

        else if (splited[j][0] == '"') {//verifica si es un valor string
            list_tokens.push_back(make_pair("string_literal", splited[j]));
        }

        else if (symbolCheck(splited[j][0])) {//verifica si es un simbolo
            for (i = 0; i < 15; ++i) {//verifica si es una operacion
                if (splited[j].compare(ops[i]) == 0) {
                    list_tokens.push_back(make_pair("op", splited[j]));
                    op_flag = 1;
                    break;
                }
            }

            if (!op_flag) {
                for (i = 0; i < 4; ++i) {//verifica si es un delimitador
                    if (splited[j].compare(delim[i]) == 0) {
                        list_tokens.push_back(make_pair("delim", splited[j]));
                        sim_flag = 1;
                        break;
                    }
                }

                if (!sim_flag) {
                    cout << "ERROR : Simbolo no permitido en la linea " << lines_cont << endl;
                }
            }
        }

        else {
            for (i = 0; i < 4; ++i) {//verifica si es funcion
                if (splited[j].compare(functions_name[i]) == 0) {
                    list_tokens.push_back(make_pair("func", splited[j]));
                    fn_flag = 1;
                    break;
                }
            }

            if (!fn_flag) {
                for (i = 0; i < 15; ++i) {//verifica si es una palabra reservada
                    if (splited[j].compare(keywords[i]) == 0) {
                        list_tokens.push_back(make_pair("keyword", splited[j]));
                        rw_flag = 1;
                        break;
                    }
                }

                if (!rw_flag) {// verifica si es un identificador
                    list_tokens.push_back(make_pair("id", splited[j]));
                }
            }
        }

        rw_flag = 0; fn_flag = 0; op_flag = 0; sim_flag = 0;
    }
}

vector<string> split_line(string command) {
    char* token;
    char* next_token = NULL;
    vector<string> palabras;

    char* c_command = new char[command.length() + 1];//copia el comando en char*
    strcpy_s(c_command, command.length() + 1, command.c_str());

    token = strtok_s(c_command, SHELL_TOK_DELIM, &next_token);
    while (token != NULL) {
        palabras.push_back(string(token));

        token = strtok_s(NULL, SHELL_TOK_DELIM, &next_token);
    }

    return palabras;
}

void read_lines() {
    string filename("example.bi.txt");
    string line;
    vector<string> lines_splited_all;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open the file '" << filename << "'" << endl;
        exit(-1);
    }

    while (!file.eof()) {
        line.clear();
        lines_splited_all.clear();

        getline(file, line); //lee la linea respectiva
        lines_splited_all = split_line(line); //la desglosa

        /*for (const auto& i : lines_splited_all)
            cout << i << endl;*/
        scanner(lines_splited_all);
        lines_cont++;
    }

    cout << "######################## Lista de Tokens Obtenidos ########################" << endl;
    for (const auto& i : list_tokens) {
        cout << "<" << i.first << "," << i.second << ">" << endl;
    }
    cout << "###########################################################################" << endl << endl;

    file.close();
}

pair<int*, int> tokens_to_input() {
    read_lines();
    int* input = ((int*)malloc(sizeof(int) * list_tokens.size()));

    asociation["func"] = 19;
    asociation["id"] = 20;
    asociation["="] = 21;
    asociation["."] = 22;
    asociation["%"] = 23;
    asociation["string"] = 24;
    asociation["int"] = 25;
    asociation["float"] = 26;
    asociation["bool"] = 27;
    asociation["tree"] = 28;
    asociation["number_literal"] = 29;
    asociation["string_literal"] = 30;
    asociation["avl"] = 31;
    asociation["simple"] = 32;
    asociation["+"] = 33;
    asociation["-"] = 34;
    asociation["*"] = 35;
    asociation["/"] = 36;
    asociation["<"] = 37;
    asociation[">"] = 38;
    asociation["!"] = 39;
    asociation["("] = 40;
    asociation[")"] = 41;
    asociation["{"] = 42;
    asociation["}"] = 43;
    asociation["if"] = 44;
    asociation["else"] = 45;
    asociation["for"] = 46;
    asociation[";"] = 47;

    for (int i = 0; i < list_tokens.size(); ++i) {
        if (((list_tokens[i].first.compare("number_literal")) != 0) &&
            ((list_tokens[i].first.compare("string_literal")) != 0) &&
            ((list_tokens[i].first.compare("id")) != 0) &&
            ((list_tokens[i].first.compare("func")) != 0)) {

            if (asociation.find(list_tokens[i].second) != asociation.end()) {
                input[i] = asociation.find(list_tokens[i].second)->second;
            }
            else {
                cout << "Error con el token de tipo " << list_tokens[i].first << " de valor " << list_tokens[i].second << endl;
            }

        }
        else {
            if (asociation.find(list_tokens[i].first) != asociation.end()) {
                input[i] = asociation.find(list_tokens[i].first)->second;
            }
            else {
                cout << "Error con el token de tipo " << list_tokens[i].first << " de valor " << list_tokens[i].second << endl;
            }
        }
    }

    return make_pair(input, list_tokens.size());
}