/*
*   UNIVERSIDADE FEDERAL DE VIÇOSA
*   
*   Desenvolvido por Erick Figueiredo (@erickfigueiredo)
*   Professor: Salles Vianna    Disc: Estrutura de Dados
*   Data: 03/12/2020
*/

#include <iostream>
#include <string>

#include <fstream>   // Leitura de arquivos
#include <algorithm> // utilização de pair e sort

#include "MyVecNewIterator.h"
#include "MyMap.h"

using namespace std;

struct Dictionary {
    public:
        MyMap<string, int> lv1;
        MyMap<string, MyMap<string, int>> lv2;
        MyMap<string, MyMap<string, MyMap<string, int>>> lv3;
};


//-----------------------| Interfaces |-----------------------

bool isAlpha(const char c);
bool isUpper(const char c);
bool isLower(const char c);
bool isNumeric(const char c);
bool isEspecialCase(const char c);

bool sortDescByFreq(const pair<string, int> &, const pair<string, int> &);
void helpSearch(MyMap<string,int>:: iterator &, string &, int);
void findRelation(MyMap <string, int>:: iterator, MyVec<string> &, bool);

string formatLine(const string &);
bool identSentences(MyVec<MyVec<string>> &, const string &, bool);
void populateDictionary(Dictionary &, const MyVec<MyVec<string>> &);
MyVec<string> formatQuery(const string &);
void search(const MyVec<string> &, Dictionary &);
void generate(MyVec<string> &, Dictionary &);


//--------------------------| Main |--------------------------

int main(int argc, char *argv[]) {
    MyVec<MyVec<string>> text;
    MyVec<string> query;

    Dictionary dic;

    string sentence;
    bool needLine = false;

    if (argc > 1) {
        ifstream fileIn(argv[1]);  //Instancia o objeto fileIn passando o nome do arquivo

        while (!fileIn.eof()) { // Enquanto não atingir o fim do arquivo
            if (sentence == "FINAL_TREINO") // Ou chegar ao comando FINAL_TREINO
                break;

            sentence = formatLine(sentence);    // Formata a linha deixando apenas palavras separadas por virgulas e newlines

            if (sentence.size())
                needLine = identSentences(text, sentence, needLine);    // Coloca cada sentença em uma linha de text e informa se a próx. sentence precisa começar em outra linha
        }

        fileIn.close(); // Fecha o arquivo

        populateDictionary(dic, text);  // Adiciona as palavras do texto no dicionario

        //Passa para a leitura dos comandos
        while (getline(cin, sentence)) {
            query = formatQuery(sentence);  // Formata o comando de input

            if (query[0] == "consulta")
                search(query, dic); // Faz a busca e exibe o resultado

            else
                generate(query, dic); // Gera o a frase e exibe o resultado
        }
    } else {
        // Formata os inputs e identa o conteúdo em MyVec
        while (getline(cin, sentence)) {
            if (sentence == "COMECO_TREINO") // Esse input será apenas ignorado
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

            if (query[0] == "consultar")
                search(query, dic);
            else
                generate(query, dic);
        }
    }

    return 0;
}


//------------------------| Funções |------------------------

//* Funções auxiliares para reduzir o tamanho das expressões booleanas
bool isAlpha(const char c) { return ((c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z')); }

bool isUpper(const char c) { return (c >= 'A' and c <= 'Z'); }

bool isLower(const char c) { return (c >= 'a' and c <= 'z'); }

bool isNumeric(const char c) { return (c >= '0' and c <= '9'); }

bool isEspecialCase(const char c) { return (c == 34 or c == 39 or c == 10 or c == 32); } // Caso aspas duplas, simples, newline ou espaço

//* Função auxiliar de sort: Ordenar pela maior frequencia, seguido da menor posição Lexograficamente
bool sortDescByFreq(const pair<string, int> &p1, const pair<string, int> &p2) {
    if (p1.second == p2.second)     // Se tiver a mesma frequencia
        return p1.first < p2.first; // Verifique lexograficamente o menor

    return p1.second > p2.second; // verifique numericamente o maior
}

//* Exibe os dados requisitados pelo comando consulta
void helpSearch(MyMap<string,int>:: iterator &it, string &relParent, int rows) {
    MyVec<pair<string, int>> response;

    while (it != NULL) {
        response.push_back((*it));  // Adiciona os pares no nível 2 que se relacionam com query[2]
        it++;
    }

    sort(response.begin(), response.end(), sortDescByFreq);   // Ordena os pares adicionados

    for (int i = 0; i < response.size() and rows; i++, rows--)    // Exibe a combinação seguida da frequência enquanto nao atingir o fim de response ou o fim de row
        cout << relParent << " " << response[i].first << " (" << response[i].second << ")\n";

}

//* Exibe os dados requisitados pelo comando gera
void findRelation(MyMap <string, int>:: iterator it, MyVec<string> &query, bool mode) {

    if(mode) {  // Modo: Padrão
        pair<string, int> mostFreq; // Auxiliar que guarda a palavra mais frequente
    
        mostFreq.first = (*it).first;
        mostFreq.second = (*it).second;

        while (it != NULL) {
            if(mostFreq.second < (*it).second){ // Se for mais frequente substitui
                mostFreq.first = (*it).first;
                mostFreq.second = (*it).second;
            } else if (mostFreq.second == (*it).second) // Se for igual pega a menor lexograficamente
                if(mostFreq.first > (*it).first) 
                    mostFreq.first = (*it).first;
            
            it++;   // Incrementa o iterador
        }
        
        query.push_back(mostFreq.first); // Adiciona a palavra encontrada
    } else {  // Modo: Aleatório

        pair<string,int> freq [3] = {(*it),(*it),(*it)}; // Index 0 - Maior frequencia, Index 1 - Media, Index 2- baixa;

        while (it != NULL) {
            if(freq[0].second < (*it).second){ // Se for mais frequente substitui
                freq[0].first = (*it).first;
                freq[0].second = (*it).second;
            } else if (freq[0].second == (*it).second) // Se for igual ao mais frequente pega a menor lexograficamente
                if(freq[0].first > (*it).first) 
                    freq[0].first = (*it).first;
            else if (freq[2].second > (*it).second) {   // Se for menor que o menos frequente
                freq[2].first = (*it).first;
                freq[2].second = (*it).second;
            } else {    // Se for um meio termo
                freq[1].first = (*it).first;
                freq[1].second = (*it).second;
            }
            
            it++;   // Incrementa o iterador
        }
        
        query.push_back(freq[rand() % 3].first); // Adiciona umaa palavra no intervalo [0,3[
    }
}

//* Adequa a linha à formatação exigida no exercício
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
            if(formatedLine[formatedLine.size()-1] == ',') // Se o último caractere existente for ",", troca por newline
                formatedLine[formatedLine.size()-1] = '\n';
            else
                formatedLine += '\n';  // Senão apenas incrementa o newline
        }
    }

    return formatedLine;
}

//* Passagem da linha formatada para MyVec
bool identSentences(MyVec<MyVec<string>> &text, const string &lineFormated, bool needLine) {
    int index = 0;

    if (!text.size()) { // Se MyVector está vazio, adiciona o primeiro objeto newLine
        MyVec<string> newLine;
        text.push_back(newLine);
    } else
        index = text.size() - 1; // O índice é ajustado para a última linha alocada até o momento em text

    string word;

    for (int i = 0; i < lineFormated.size(); i++) {
        if (needLine) { // Se precisa de uma nova linha, aloca em text, incrementa o indice e altera a flag
            MyVec<string> newLine;
            text.push_back(newLine);

            index++;
            needLine = false;
        }

        if (lineFormated[i] == '\n' or lineFormated[i] == ',') { // aloca a ultima palavra formada na linha atual
            text[index].push_back(word);
            word = "";

            if (lineFormated[i] == '\n') // Ativa a flag de necessidade de nova linha
                needLine = true;
        } else if (i == lineFormated.size() - 1) { // Indica que a frase inserida até o momento acabou, porém a senetença não, insere a palavra na linha
            if (!word.size())
                word = lineFormated[i];
            else
                word += lineFormated[i];
            text[index].push_back(word);
        } else {
            if (!word.size()) // Se word está resetado, começa a receber as letras da nova palavra
                word = lineFormated[i];
            else
                word += lineFormated[i];
        }
    }
    return needLine; // Indica se a nova frase precisará ser escrita em uma nova linha ou não
}

//* Popular dicionário com os dados de MyVec
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

//* Formata comandos (mesmo princípio da func. identSentences para formatação do comando)
MyVec<string> formatQuery(const string &sentence) {
    MyVec<string> query;
    string word;

    for (int i = 0; i < sentence.size(); i++) {
        if ((sentence[i] == ' ' or sentence[i] == '\n') and word.size()) {
            query.push_back(word);
            word = "";
        } else {
            if (i == sentence.size() - 1) {
                if(!word.size())
                    word = sentence[i];
                
                else
                    word += sentence[i];
                
                query.push_back(word);
            } else
                word += sentence[i];
        }
    }

    return query;
}

//* Comando: Consulta
void search(const MyVec<string> &query, Dictionary &dic) {
    
    int rows = stoi(query[1]); // Converte o número de linhas desejadas para um valor inteiro
    int nParams = query.size() - 2; //Numero de palavras passadas

    switch (nParams) {
        case 1: // Caso seja passada uma palavra
            cout << query[2] << " (" << dic.lv1[query[2]] << ")\n"; // Exibe a frequência
            if (rows && dic.lv2[query[2]].size()) { // Se houver demanada de linhas a serem exibidas e existir uma sequencia no nível 2
                string aux = query[2];
                MyMap<string,int>:: iterator it = dic.lv2[query[2]].begin();

                helpSearch(it, aux, rows);
            }
            break;

        case 2: // Caso seja passada duas palavras (mesmo princípio do caso 1)
            cout << query[2] << " " << query[3] << " (" << dic.lv2[query[2]][query[3]] << ")\n"; 
            if (rows && dic.lv3[query[2]][query[3]].size()) {
                string aux = query[2]+" "+query[3];
                MyMap<string,int>:: iterator it = dic.lv3[query[2]][query[3]].begin();

                helpSearch(it, aux, rows);
            }
            break;

        case 3: // Caso seja passada três palavras
            cout << query[2] << " " << query[3] << " " << query[4] << " (" << dic.lv3[query[2]][query[3]][query[4]] << ")\n";   // Exibe a frequencia
            break;
    }
    
    cout << '\n';
}

//* Comando: Gera
void generate(MyVec<string> &query, Dictionary &dic) {
    int newWords = stoi(query[1]); // Palavras a serem adicionadas
    int numWords = query.size()-3; // Número de palavras existentes
    bool mode = false;
    
    if(query[2] ==  "padrao")
        mode = true;

    while(newWords){ // enquanto precisar de palavras
        MyMap <string, int>:: iterator it = dic.lv1.begin();
        switch (numWords) {
            case 0: // Caso tenha sido passado 0 palavras iniciais
                if (dic.lv1.size()) {
                    findRelation(it, query, mode);   // Busca e adiciona a melhor palavra para o contexto
                    numWords++; // Incrementa o número de palavras adicionadas
                }
                break;
            
            case 1: // Caso tenha sido passado 1 palavra inicial
                if (dic.lv2[query[3]].size()) {
                    it = dic.lv2[query[3]].begin();
                    findRelation(it, query, mode);
                    numWords++;
                } else if(dic.lv1.size()) {
                    findRelation(it, query, mode);
                    numWords++;
                }
                break;

            default: // Caso tenha sido passado +1 palavras iniciais
                    
                if (dic.lv3[query[query.size()-2]][query[query.size()-1]].size()) { // Se tiver palavras alocadas para a combinação no nível 3
                    it = dic.lv3[query[query.size()-2]][query[query.size()-1]].begin();
                    findRelation(it, query, mode);
                    numWords++;
                } else if (dic.lv2[query[query.size()-1]].size()) { // Se tiver palavras alocadas para a combinação no nível 2
                    it = dic.lv2[query[query.size()-1]].begin();
                    findRelation(it, query, mode);
                    numWords++;
                } else if(dic.lv1.size()) { // Se tiver palavras alocadas no nível 1
                    findRelation(it, query, mode);
                    numWords++;
                }
                break;
        }
        newWords--; //Decrementa o numero de palavras requisitadas
    }    

    for (int i = 3; i < query.size(); i++)  // Exibe o resultado
        if(query[i] != " ")
            cout << query[i] << ' ';

    cout << "\n\n";
}