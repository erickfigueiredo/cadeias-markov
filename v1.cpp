/*
*   UNIVERSIDADE FEDERAL DE VIÇOSA
*   
*   Desenvolvido por Erick Figueiredo (@erickfigueiredo)
*   Professor: Salles Vianna    Disc: Estrutura de Dados
*   Data: 
*/

#include <iostream> 
#include <cstring>
#include <fstream>  // Leitura de arquivos
#include <utility>  // Utilização de Pair
#include "MyVecNewIterator.h"
#include "MyMap.h"

using namespace std;

//* Interfaces
bool isAlpha(const char c);
bool isUpper(const char c);
bool isLower(const char c);
bool isNumeric(const char c); 
bool isEspecialCase(const char c);

string formatLine(const string &line);
bool identSentences(MyVec<MyVec<string>> &text, string &lineFormated, bool needLine);

//* Método main
int main(int argc, char *argv[]) {
    MyVec< MyVec <string>> text;


    string sentence;

    bool needLine = false;

    if (argc > 1) {
        ifstream fileIn(argv[1]);
        // Falta trabalhar nessa parte
        fileIn.close();

    }else{
        // Formata os inputs e identa o conteúdo em MyVec
        while(getline(cin, sentence)){
            if(sentence == "COMECO_TREINO")
                continue;

            if(sentence == "FINAL_TREINO")
                break;

            sentence = formatLine(sentence);

            // Se após a formatação não houver nenhuma letra na sentence, passa para a próx iteração
            if(sentence == "" || sentence == "," || sentence == "\n")
                continue;
            cout << sentence << endl;

            needLine = identSentences(text, sentence, needLine); 
        }

        //Preenche o MyMap
        
        for (int i = 0; i < text.size(); i++)
        {
            for (int j = 0; j < text[i].size(); j++)
                cout << text[i][j] << ' ';

            cout << '\n';
        }
        
        //Obedece aos comandos
    }
    
    return 0;
}

//! Desenvolvimento das funções

// Funções auxiliares para reduzir o tamanho das expressões booleanas

bool isAlpha(const char c){ return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }

bool isUpper(const char c){ return (c >= 'A' && c <= 'Z'); }

bool isLower(const char c){ return (c >= 'a' && c <= 'z'); }

bool isNumeric(const char c){ return (c >= '0' && c <= '9'); }

bool isEspecialCase(const char c) { return (c == 34 || c == 39 || c == 10 || c == 32); } // Caso aspas duplas, simples, newline ou espaço


// Adequa a linha à formatação exigida no exercício

string formatLine(const string &line) {
    string formatedLine;

    for(int i = 0; i < line.size(); i++) {

        if (isAlpha(line[i]) || line[i] == '-') { 
            if(isLower(line[i]))    // Se for alfabético e minusculo, copia
                formatedLine += line[i];
            
            else if(isUpper(line[i]))   // Se for alfabetico e maiúsculo, converte e copia
                formatedLine += (line[i] + 32);

            else if ((i > 0 && isAlpha(line[i - 1])) && (i < line.size() - 1 && isAlpha(line[i+1]))) // Se o hífen estiver entre letras do alfabeto, copia
                formatedLine += line[i];

        }else if (i > 0 && isEspecialCase(line[i])) {   // Caso de caracteres especiais, passa a verificar depois do primeiro caractere

            if (formatedLine.size() != 0 && i < line.size() - 1 && formatedLine[formatedLine.size() - 1] != ',' && formatedLine[formatedLine.size() - 1] != '\n') // Se o caractere anterior for diferente de vírgula e new line, converte o atual para virgula
                formatedLine += ',';

            continue;   // Se o anterior já for newline ou vírgula, passa para a próxima iteração
        
        }else if (i > 0 && formatedLine.size() != 0 && formatedLine[formatedLine.size() - 1] != '\n') { // Caso a string formatada não esteja vazia, o último caractere seja diferente de newline e esteja a frente do primeio caractere

            if (formatedLine[formatedLine.size() - 1] == ',') // Se o anterior for vírgula, converte para newline
                formatedLine[formatedLine.size() - 1] = '\n';

            else // Senão, adiciona newline
                formatedLine += '\n';
        }
    }

    return formatedLine;
}

// Ajusta colocando apenas uma sentença por linha no vector

bool identSentences(MyVec<MyVec<string>> &text, string &lineFormated, bool needLine) {
    int index = 0;

    if(!text.size()){   // Se MyVector está vazio, adiciona o primeiro objeto newLine
        MyVec <string> newLine;
        text.push_back(newLine);
    }else
        index = text.size() - 1;    // O índice é ajustado para a última linha alocada até o momento em text
    
    string word;

    for(int i = 0; i < lineFormated.size(); i++){
        if(needLine){   // Se precisa de uma nova linha, aloca em text e incrementa o indice
            MyVec<string> newLine;
            text.push_back(newLine);

            index++;
            needLine = false;
        }

        if(lineFormated[i] == '\n'){    // Indica nova sentença, aloca a ultima palavra formada na linha atual e ativa a flag para nova alocação
            text[index].push_back(word);
            word = "!";
            needLine = true;

        }else if(lineFormated[i] == ','){   // Indica fim da palavra atual, aloca word na linha e reseta seu valor para receber nova palavra
            text[index].push_back(word); 
            word = "!";

        }else if(i == lineFormated.size()-1){   // Indica que a frase inserida até o momento acabou, porém a senetença não, insere a palavra na linha
            if(word == "!")
                word = lineFormated[i];
            else
                word += lineFormated[i];
            text[index].push_back(word);
            
        }else{
            if(word == "!") // Se word está resetado, começa a receber as letras da nova palavra
                word = lineFormated[i];
            else
                word += lineFormated[i];    
        }
    }

    return needLine;    // Indica se a nova frase precisará ser escrita em uma nova linha ou não
}

