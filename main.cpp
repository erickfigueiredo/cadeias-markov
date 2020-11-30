/*
*   UNIVERSIDADE FEDERAL DE VIÇOSA
*   
*   Desenvolvido por Erick Figueiredo (@erickfigueiredo)
*   Professor: Salles Vianna    Disc: Estrutura de Dados
*   Data: 
*/

#include <iostream>
#include <cstring>

#include <fstream> // Leitura de arquivos
#include <algorithm> // utilização do sort

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
bool identSentences(MyVec<MyVec<string>> &text, const string &lineFormated, bool needLine);
void populateDictionary(Dictionary &dic, const MyVec<MyVec<string>> &text);


struct Dictionary{
    public:
        MyMap<string, int> lv1;
        MyMap<string, MyMap<string, int>> lv2;
        MyMap<string, MyMap<string, MyMap<string, int>>> lv3;
};

//* Método main
int main(int argc, char *argv[]) {
    MyVec< MyVec <string>> text;
    
    Dictionary dic;

    string sentence;
    bool needLine = false;

    if (argc > 1) {

        bool isTraining = true;

        ifstream fileIn(argv[1]);
        while (!fileIn.eof()){
            getline(fileIn, sentence);
            
            if(sentence == "FINAL_TREINO"){
                isTraining = false;
                populateDictionary(dic, text);
                
                continue;
            }
            
            if(isTraining) {
                sentence = formatLine(sentence);

                if (sentence.size())
                    needLine = identSentences(text, sentence, needLine);

            } else {
                //AREA DOS COMANDOS
            }
        }

        fileIn.close();
    }else{
        // Formata os inputs e identa o conteúdo em MyVec
        while(getline(cin, sentence)){
            if(sentence == "COMECO_TREINO")
                continue;

            if(sentence == "FINAL_TREINO")
                break;

            sentence = formatLine(sentence);

            if (sentence.size())
                needLine = identSentences(text, sentence, needLine);
        }

        //Preenchemos o map com os dados obtidos
        populateDictionary(dic, text);

        //Passa para a leitura dos comandos

    }
    
    return 0;
}


//! Desenvolvimento das funções

// Funções auxiliares para reduzir o tamanho das expressões booleanas

bool isAlpha(const char c) { return ((c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z')); }

bool isUpper(const char c) { return (c >= 'A' and c <= 'Z'); }

bool isLower(const char c) { return (c >= 'a' and c <= 'z'); }

bool isNumeric(const char c) { return (c >= '0' and c <= '9'); }

bool isEspecialCase(const char c) { return (c == 34 or c == 39 or c == 10 or c == 32); } // Caso aspas duplas, simples, newline ou espaço


// Adequa a linha à formatação exigida no exercício

string formatLine(const string &line) {
    string formatedLine;

    for(int i = 0; i < line.size(); i++) {
        if(isAlpha(line[i]) or line[i] == '-') {    // Caractere alfabético ou hífen
            if(isLower(line[i]))    // Se for minúsculo, apenas copia
                formatedLine += line[i];

            else if(isUpper(line[i]))   // Se for maiúsculo, converte e copia
                formatedLine += (line[i]+32);

            else if ((i > 0 and isAlpha(line[i - 1])) and (i < line.size() - 1 and isAlpha(line[i + 1]))) // Se o hífen for precedido e sucedido por letras, copia
                formatedLine += line[i];

        } else if (isEspecialCase(line[i]) and formatedLine.size() != 0 and (i < line.size() - 1)) {    // Se for um caractere do caso especial não sendo o último, na condição de que já haja letra na string de copia

            if (!isEspecialCase(line[i + 1]) and line[i + 1] != '\n' and formatedLine[formatedLine.size()-1] != '\n')   // Se o próximo caractere não for especial e o atual não esteja entre newlines, copia vírgula
                formatedLine += ',';
            
            continue;
        }
        else if (formatedLine.size() != 0 and formatedLine[formatedLine.size() - 1] != '\n') {  // Se a string resultado não estiver vazia e o caractere anterior for diferente de newline copia
            formatedLine += '\n';  
        }
    }

    return formatedLine;
}


// Passagem da linha formatada para MyVec

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

        if(lineFormated[i] == '\n' or lineFormated[i] == ','){    // aloca a ultima palavra formada na linha atual
            text[index].push_back(word);
            word = "!";

            if (lineFormated[i] == '\n')    // Ativa a flag de necessidade de nova linha
                needLine = true;

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


// Popular dicionário com os dados de MyVec

void populateDictionary(Dictionary &dic, const MyVec<MyVec<string>> &text) {
    
    //Nivel 1 - Frequencia da palavra no texto
    //Nivel 2 - Frequencia de duas palavras seguidas no texto
    //Nivel 3 - Frequencia de três palavras seguidas no texto

    for(int i = 0; i < text.size(); i++){
        for(int j = 0; j < text[i].size(); j++){
            

        }
    }
}