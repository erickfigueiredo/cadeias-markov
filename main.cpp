/*
*   UNIVERSIDADE FEDERAL DE VIÇOSA
*   
*   Desenvolvido por Erick Figueiredo (@erickfigueiredo)
*   Professor: Salles Vianna    Disc: Estrutura de Dados
*   Data: 
*/

#include <iostream>
#include <string>

#include <fstream>   // Leitura de arquivos
#include <algorithm> // utilização do sort

#include "MyVecNewIterator.h"
#include "MyMap.h"

using namespace std;

struct Dictionary
{
public:
    MyMap<string, int> lv1;
    MyMap<string, MyMap<string, int>> lv2;
    MyMap<string, MyMap<string, MyMap<string, int>>> lv3;
};

//* Interfaces
bool isAlpha(const char c);
bool isUpper(const char c);
bool isLower(const char c);
bool isNumeric(const char c);
bool isEspecialCase(const char c);

bool sortDescByFreq(const pair<string, int> &p1, const pair<string, int> &p2);

string formatLine(const string &);
bool identSentences(MyVec<MyVec<string>> &, const string &, bool);
void populateDictionary(Dictionary &, const MyVec<MyVec<string>> &);
MyVec<string> formatQuery(const string &);
void search(const MyVec<string> &, Dictionary &);
void generate(const MyVec<string> &, Dictionary &);


//* Método main
int main(int argc, char *argv[])
{
    MyVec<MyVec<string>> text;
    MyVec<string> query;

    Dictionary dic;

    string sentence;
    bool needLine = false;

    if (argc > 1)
    {
        ifstream fileIn(argv[1]);

        while (!fileIn.eof())
        {
            if (sentence == "FINAL_TREINO")
                break;

            sentence = formatLine(sentence);

            if (sentence.size())
                needLine = identSentences(text, sentence, needLine);
        }

        fileIn.close();

        //Preenchemos o map com os dados obtidos
        /*
        populateDictionary(dic, text);

        //Passa para a leitura dos comandos
        while (getline(cin, sentence))
        {
            query = formatQuery(sentence);

            if (query[0] == "consulta")
                search(query, dic);

            else
                generate(query, dic);
        }
        */
    }
    else
    {
        // Formata os inputs e identa o conteúdo em MyVec
        while (getline(cin, sentence))
        {
            if (sentence == "COMECO_TREINO")
                continue;

            if (sentence == "FINAL_TREINO")
                break;

            sentence = formatLine(sentence);

            if (sentence.size())
                needLine = identSentences(text, sentence, needLine);
        }

        // Preenchemos o map com os dados obtidos
        populateDictionary(dic, text);

        // Leitura dos comandos 
        while (getline(cin, sentence)) {
            query = formatQuery(sentence);

            if (query[0] == "consulta")
                search(query, dic);
            else
                generate(query, dic);
        }
    }

    return 0;
}
//* FIM METODO MAIN

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

    for (int i = 0; i < line.size(); i++) {
        if (isAlpha(line[i]) or line[i] == '-') {   // Caractere alfabético ou hífen
            if (isLower(line[i])) // Se for minúsculo, apenas copia
                formatedLine += line[i];

            else if (isUpper(line[i])) // Se for maiúsculo, converte e copia
                formatedLine += (line[i] + 32);

            else if ((i > 0 and isAlpha(line[i - 1])) and (i < line.size() - 1 and isAlpha(line[i + 1]))) // Se o hífen for precedido e sucedido por letras, copia
                formatedLine += line[i];
        } else if (isEspecialCase(line[i]) and formatedLine.size() != 0 and (i < line.size() - 1)) { // Se for um caractere do caso especial não sendo o último, na condição de que já haja letra na string de copia

            if (!isEspecialCase(line[i + 1]) and line[i + 1] != '\n' and !isNumeric(line[i + 1]) and formatedLine[formatedLine.size() - 1] != '\n') // Se o próximo caractere não for especial, ou numérico e o atual não esteja entre newlines, copia vírgula
                formatedLine += ',';

            continue;
        } else if (formatedLine.size() != 0 and formatedLine[formatedLine.size() - 1] != '\n') { // Se a string resultado não estiver vazia e o caractere anterior for diferente de newline copia
            formatedLine += '\n';
        }
    }

    return formatedLine;
}


// Passagem da linha formatada para MyVec
bool identSentences(MyVec<MyVec<string>> &text, const string &lineFormated, bool needLine) {
    int index = 0;

    if (!text.size()) { // Se MyVector está vazio, adiciona o primeiro objeto newLine
        MyVec<string> newLine;
        text.push_back(newLine);
    } else
        index = text.size() - 1; // O índice é ajustado para a última linha alocada até o momento em text

    string word;

    for (int i = 0; i < lineFormated.size(); i++) {
        if (needLine) { // Se precisa de uma nova linha, aloca em text e incrementa o indice
            MyVec<string> newLine;
            text.push_back(newLine);

            index++;
            needLine = false;
        }

        if (lineFormated[i] == '\n' or lineFormated[i] == ',') { // aloca a ultima palavra formada na linha atual
            text[index].push_back(word);
            word = "!";

            if (lineFormated[i] == '\n') // Ativa a flag de necessidade de nova linha
                needLine = true;
        } else if (i == lineFormated.size() - 1) { // Indica que a frase inserida até o momento acabou, porém a senetença não, insere a palavra na linha
            if (word == "!")
                word = lineFormated[i];
            else
                word += lineFormated[i];
            text[index].push_back(word);
        } else {
            if (word == "!") // Se word está resetado, começa a receber as letras da nova palavra
                word = lineFormated[i];
            else
                word += lineFormated[i];
        }
    }

    return needLine; // Indica se a nova frase precisará ser escrita em uma nova linha ou não
}


// Popular dicionário com os dados de MyVec
void populateDictionary(Dictionary &dic, const MyVec<MyVec<string>> &text) {

    //Nivel 1 - Frequencia da palavra no texto
    //Nivel 2 - Frequencia de duas palavras seguidas no texto
    //Nivel 3 - Frequencia de três palavras seguidas no texto

    for (int i = 0; i < text.size(); i++)   // Percorre todas as linhas e colunas de text
        for (int j = 0; j < text[i].size(); j++) {

            dic.lv1[text[i][j]]++; // Caso a chave se repita o valor é incrementado

            if (j + 1 < text[i].size()) // Se existir uma palavra à frente cria uma relação entre essa e a anterior, incrementa a frequência
                dic.lv2[text[i][j]][text[i][j + 1]]++;

            if (j + 2 < text[i].size()) // Se existir uma palavra à frente cria uma relação entre essa e "frase" já formada, incrementa a frequência
                dic.lv3[text[i][j]][text[i][j + 1]][text[i][j + 2]]++;
        }
}


// Formata comandos (mesmo princípio da func. identSentences)
MyVec<string> formatQuery(const string &sentence) {
    MyVec<string> query;
    string aux;

    for (int i = 0; i < sentence.size(); i++) {
        if (sentence[i] == ' ' || i == sentence.size() - 1) {
            if(aux == "!" and isAlpha(sentence[i]))
                aux = sentence[i];

            else if (isAlpha(sentence[i]))
                aux += sentence[i];
            
            query.push_back(aux);
            aux = "!";
        } else {
            if (aux == "!")
                aux = sentence[i];
            else
                aux += sentence[i];
        }
    }

    return query;
}


// Função auxiliar de sort: Ordenar pela maior frequencia, seguido da menor posição Lexograficamente
bool sortDescByFreq(const pair<string, int> &p1, const pair<string, int> &p2)
{
    if (p1.second == p1.second)     // Se tiver a mesma frequencia
        return p1.first < p2.first; // Verifique lexograficamente

    return p1.second > p1.second; // Senão retorna true se p1 tiver mais frequência
}


// Comando: Consulta
void search(const MyVec<string> &query, Dictionary &dic)
{
    int rows = stoi(query[1]); // Converte o número de linhas desejadas para um valor inteiro

    if (rows > 0) { // Se o numero de linhas buscadas for maior que 0

        int nParams = query.size() - 2; //Numero de palavras passadas

        switch (nParams) {
        case 1: // Caso seja passada uma palavra
            if (dic.lv1[query[2]]) { // Se tiver frequência exibe
                cout << query[2] << " (" << dic.lv1[query[2]] << ")\n";

                rows--;

                if (rows && dic.lv2[query[2]].size()) {
                    MyMap<string, int>::iterator it2 = dic.lv2[query[2]].begin();

                    MyVec<pair<string, int>> aux;

                    while (it2 != NULL) {
                        aux.push_back((*it2));
                        it2++;
                    }

                    sort(aux.begin(), aux.end(), sortDescByFreq);

                    for (int i = 0; i < aux.size() && rows; i++, rows--)
                        cout << query[2] << " " << aux[i].first << " (" << aux[i].second << ")\n";
                }

                if (rows && dic.lv3[query[2]].size()) {

                    MyMap<string, int>::iterator it2 = dic.lv2[query[2]].begin();
                    MyMap<string, int>::iterator it3 = dic.lv2[query[2]].begin();

                    MyVec<pair<string, int>> aux;
                    pair<string, int> bind;

                    while (it2 != NULL) {
                        it3 = dic.lv3[query[2]][(*it2).first].begin();
                        while (it3 != NULL) {
                            bind.first = (*it2).first + " " + (*it3).first;
                            bind.second = (*it3).second;

                            aux.push_back(bind);

                            it3++;
                        }
                        it2++;
                    }

                    sort(aux.begin(), aux.end(), sortDescByFreq);

                    for (int i = 0; i < aux.size() and rows; i++, rows--)
                        cout << query[2] << " " << aux[i].first << " (" << aux[i].second << ")\n";
                }
            }
            break;

        case 2: // Caso seja passada duas palavras
            if (dic.lv2[query[2]][query[3]]) { // Se tiver frequência exibe
                cout << query[2] << " " << query[3] << " (" << dic.lv2[query[2]][query[3]] << ")\n";

                rows--;

                if (rows && dic.lv3[query[2]][query[3]].size()) {
                    MyMap<string, int>::iterator it3 = dic.lv3[query[2]][query[3]].begin();

                    MyVec<pair<string, int>> aux;

                    while (it3 != NULL) {
                        aux.push_back((*it3));
                        it3++;
                    }

                    sort(aux.begin(), aux.end(), sortDescByFreq);

                    for (int i = 0; i < aux.size() and rows; i++, rows--)
                        cout << query[2] << " " << query[3] << " " << aux[i].first << " (" << aux[i].second << ")\n";
                }
            }
            break;
        case 3: // Caso seja passada três palavras
            if (dic.lv3[query[2]][query[3]][query[4]]) // Se tiver frequência exibe
                cout << query[2] << query[3] << query[4] << " (" << dic.lv3[query[2]][query[3]][query[4]] << ")\n";
            break;
        }
    }
}


// Comando: Gera
void generate(const MyVec<string> &query, Dictionary &dic) {
    

}