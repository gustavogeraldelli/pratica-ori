/**
 * Trabalho prático de Organização e Recuperação da Informação
 * UFSCAR
 * 
 * @author Gustavo Gonçalves de Souza Geraldelli - 800523
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tabela_hash.h"
#include "Set.h"

/**
 * Retira letras maiúsculas, pontuações/espaços do começo do final da palavra
 * Letras, números e pontuações no meio de uma palavra são mantidos
 * 
 * @param str O texto a ser padronizado
 */
void padronizar(char *str);

/**
 * Função que lê o arquivo de dados, processa os tweets e insere os valores na tabela hash
 * 
 * @param qtd Quantidade de tweets a serem lidos (útil para testes), se qtd < 1, o arquivo completo será lido
 * @return Ponteiro para o índice já inicializado e armazenando os rrns de cada palavra
 */
tabela_hash* carregar_indice(int qtd);

/**
 * Função que processa a busca do usuário e retorna o Set de tweets conforme tal busca
 * 
 * @param t Ponteiro para o índice dos dados
 * @param busca Paraâmetro de busca do usuário
 * @return Ponteiro para um Set que contém todos os offsets dos tweets com as palavras da busca
 */
Set* processar_busca(tabela_hash *t, char *busca);

/**
 * Função auxiliar para recuperar apenas os offsets dos tweets que contem a palavra (evitando as colisões do hash)
 * 
 * @param offsets Ponteiro para o Set contendo os offsets dos tweets com o hash da palavra buscada
 * @param palavra Palavra usada na busca
 * @return Ponteiro para um Set que contém apenas os offsets dos tweets válidos
 */
Set* validar_tweets(Set *offsets, char *palavra);

int main() {
    tabela_hash *idx = carregar_indice(1000); // Quantidade para testes

    /**
     * NOTE: leitura total tomará muito tempo
     */
    //tabela_hash *idx = carregar_indice(-1);
    
    /**
     * NOTE: O programa só detecta 1 operador, ou seja, apenas "-- AND --" ou "-- OR --"
     * E assume-se que a entrada será formatada corretamente
     * Ex: "palavra" ou "palavra1 AND palavra2" ou "palavra1 OR palavra2"
     */
    while (1) {
        char busca[150];
        printf("Faça sua busca (ou pressione enter para sair): ");
        fgets(busca, sizeof(busca), stdin);
        if (busca[0] == '\n')
            break;
        busca[strcspn(busca, "\n")] = '\0';

        Set *offsets = processar_busca(idx, busca); // buscando offsets dos tweets que são compatíveis com a busca

        if (tamanhoSet(offsets) == 0) {
            printf(">>>>> Nenhum tweet foi encontrado!\n");
            continue;
        }

        printf(">>>>> %d tweets encontrados:\n", tamanhoSet(offsets));

        FILE *f = fopen("corpus.csv", "r");
        if (!f) {
            printf("Erro ao abrir o arquivo\n");
            exit(1);
        }
        
        for (beginSet(offsets); !endSet(offsets); nextSet(offsets)) {
            int offset;
            getItemSet(offsets, &offset);

            // le o registro do tweet com o devido offset obtido da tabela
            fseek(f, offset, SEEK_SET);
            char registro[500];
            fgets(registro, sizeof(registro), f);

            // obtendo o tweet (mensagem, de fato) do registro
            char *tweet = strtok(registro, ",");
            tweet = strtok(NULL, ",");
            tweet = strtok(NULL, ",");

            printf("\n%s\n", tweet);
            printf("==============================\n");
        }
        
        printf("\n");
        fclose(f);
        liberaSet(offsets);
    }
    
    free_tabela_hash(idx);
    return 0;
}

void padronizar(char *str) {
    int f = strlen(str) - 1;
    while (f >= 0 && !isalpha(str[f]) && !isdigit(str[f])) {
        f--;
    }
    str[f + 1] = '\0';

    int i = 0;
    while (i <= f && !isalpha(str[i]) && !isdigit(str[i])) {
        i++;
    }

    if (i > 0) {
        memmove(str, str + i, f - i + 1);
        str[f - i + 1] = '\0';
    }

    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

tabela_hash* carregar_indice(int qtd) {
    int tamanho = (qtd > 0) ? qtd : 1000000;
    tabela_hash *idx = new_tabela_hash(tamanho);

    FILE *f = fopen("corpus.csv", "r");
    if (!f) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    int cont = 0; // contador para determinar se a repetição irá parar

    while (1) {
        if (qtd > 0 && cont == qtd)
            break;
    
        char registro[500];
        int offset = ftell(f);

        if (!fgets(registro, sizeof(registro), f))
            break;

        if (offset == 0) // ignorar cabeçalho do arquivo
            continue;
        
        // obtendo o tweet (mensagem, de fato) do registro
        char *tweet = strtok(registro, ",");
        tweet = strtok(NULL, ",");
        tweet = strtok(NULL, ",");

        // separando cada palavra do tweet e adicionando no indice (tabela hash)
        char *palavra = strtok(tweet, " ");
        while (palavra) {
            padronizar(palavra); // padroniza palavras com letras maiúsculas, pontuações e espaços
            inserir_tabela_hash(idx, palavra, offset);
            palavra = strtok(NULL, " ");
        }

        cont++;
    }

    fclose(f);
    return idx;
}

Set* processar_busca(tabela_hash *t, char *busca) {
    Set *tweets = NULL;
    char palavras[2][50] = {0, 0};

    if (strstr(busca, " AND ")) {
        strcpy(palavras[0], strtok(busca, " AND "));
        strcpy(palavras[1], strtok(NULL, " AND "));
        padronizar(palavras[0]);
        padronizar(palavras[1]);

        // buscando os tweets que realmente contém a primeira palavra da busca (podem haver colisões)
        Set *offsets1 = buscar_tabela_hash(t, palavras[0]);
        Set *tweets_validos1 = validar_tweets(offsets1, palavras[0]);

        // buscando os tweets que realmente contém a segunda palavra da busca (podem haver colisões)
        Set *offsets2 = buscar_tabela_hash(t, palavras[1]);
        Set *tweets_validos2 = validar_tweets(offsets2, palavras[1]);

        tweets = interseccaoSet(tweets_validos1, tweets_validos2);
        liberaSet(tweets_validos1);
        liberaSet(tweets_validos2);
    }
    else if (strstr(busca, " OR ")) {
        strcpy(palavras[0], strtok(busca, " OR "));
        strcpy(palavras[1], strtok(NULL, " OR "));
        padronizar(palavras[0]);
        padronizar(palavras[1]);

        // buscando os tweets que realmente contém a primeira palavra da busca (podem haver colisões)
        Set *offsets1 = buscar_tabela_hash(t, palavras[0]);
        Set *tweets_validos1 = validar_tweets(offsets1, palavras[0]);

        // buscando os tweets que realmente contém a segunda palavra da busca (podem haver colisões)
        Set *offsets2 = buscar_tabela_hash(t, palavras[1]);
        Set *tweets_validos2 = validar_tweets(offsets2, palavras[1]);

        tweets = uniaoSet(tweets_validos1, tweets_validos2);
        liberaSet(tweets_validos1);
        liberaSet(tweets_validos2);
    }
    else {
        strcpy(palavras[0], busca);
        padronizar(palavras[0]);

        Set *offsets = buscar_tabela_hash(t, palavras[0]);
        Set *tweets_validos = validar_tweets(offsets, palavras[0]);

        tweets = tweets_validos;

    }

    return tweets;
}

Set* validar_tweets(Set *offsets, char *palavra) {
    FILE *f = fopen("corpus.csv", "r");
    if (!f) {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    Set *tweets_validos = criaSet();
    int offset;

    for (beginSet(offsets); !endSet(offsets); nextSet(offsets)) {
        getItemSet(offsets, &offset);
        fseek(f, offset, SEEK_SET);
        char registro[500];
        fgets(registro, sizeof(registro), f);
        // obtendo o tweet (mensagem, de fato) do registro
        char *tweet = strtok(registro, ",");
        tweet = strtok(NULL, ",");
        tweet = strtok(NULL, ",");
        padronizar(tweet);
        if (strstr(tweet, palavra))
            insereSet(tweets_validos, offset);
    }

    fclose(f);
    return tweets_validos;
}