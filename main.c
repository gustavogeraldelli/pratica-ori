/**
 * Trabalho prático de Organização e Recuperação da Informação
 * UFSCAR
 * 
 * @author Gustavo Gonçalves de Souza Geraldelli - 800523
 * @author
 * @author
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tabela_hash.h"
#include "lista.h"

/**
 * Retira letras maiúsculas, pontuações/espaços do começo do final da palavra
 * Letras, números e pontuações no meio de uma palavra são mantidos
 * 
 * @param str O texto a ser normalizado
 */
void normalizar(char *str) {
    int end = strlen(str) - 1;
    while (end >= 0 && !isalpha(str[end]) && !isdigit(str[end])) {
        end--;
    }
    str[end + 1] = '\0';

    int start = 0;
    while (start <= end && !isalpha(str[start]) && !isdigit(str[start])) {
        start++;
    }

    if (start > 0) {
        memmove(str, str + start, end - start + 1);
        str[end - start + 1] = '\0';
    }

    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

/**
 * Função teste carregando número limitado de tweets
 */
void carregar_indice(tabela_hash *idx) {
    FILE *f = fopen("../data/corpus.csv", "r");
    if (!f) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    for (int i = 0; i < 100; i++) {
        // lê o registro e recupera a mensagem do tweet
        char linha[500];
        int rrn = ftell(f);
        if (!fgets(linha, sizeof(linha), f))
            break;
        if (rrn == 0) continue; // ignorar cabeçalho do csv

        char *msg = strtok(linha, ",");
        msg = strtok(NULL, ",");
        msg = strtok(NULL, ",");
        
        // obtendo cada palavra do tweet e adicionando na lista invertida/tabela hash
        char *p = strtok(msg, " ");
        while (p) {
            normalizar(p); // padronizar palavras com letras maiusculas, pontuações e espaços
            insere(idx, p, rrn);
            p = strtok(NULL, " ");
        }
    }

    fclose(f);
}

void melhorar_carregar_indice(tabela_hash *idx) {
    FILE *f = fopen("../data/corpus.csv", "r");
    if (!f) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    /**
     * TODO: melhorar leitura
     * leitura em chunks, garantir que leu linha inteira (sem palavras quebradas)
     */
    char linha[500];
    while (1) {
        // lê o registro e recupera a mensagem do tweet
        int rrn = ftell(f);
        printf("rrn: %d\n", rrn);
        if (!fgets(linha, sizeof(linha), f))
            break;
        if (rrn == 0) continue; // ignorar cabeçalho do csv

        char *msg = strtok(linha, ",");
        msg = strtok(NULL, ",");
        msg = strtok(NULL, ",");
        
        // obtendo cada palavra do tweet e adicionando na lista invertida/tabela hash
        char *p = strtok(msg, " ");
        while (p) {
            normalizar(p); // padronizar palavras com letras maiusculas, pontuações e espaços
            insere(idx, p, rrn);
            p = strtok(NULL, " ");
        }
    }

    fclose(f);
}

int main() {
    tabela_hash *idx = new_tabela_hash(2000);

    FILE *f = fopen("../data/corpus.csv", "r");
    if (!f) {
        printf("Erro ao abrir arquivo\n");
        return 1;
    }

    carregar_indice(idx);

    char palavra[50];
    while (1) {
        printf("Digite uma palavra para buscar (ou digite '0' para sair): ");
        scanf("%s", palavra);
        if (palavra[0] == '0') break;

        normalizar(palavra);
        int tamanho = 0;
        int *rrns = busca(idx, palavra, &tamanho);

        /**
         * TODO: anagramas inválidos passam desse if
         */

        if (rrns == NULL) {
            printf("Não foi possível encontrar um tweet com essa palavra\n");
            continue;
        }

        FILE *f = fopen("../data/corpus.csv", "r");
        if (!f) {
            printf("Erro ao abrir arquivo\n");
            return 1;
        }

        printf("\n>>> %d tweets encontrados:\n", tamanho);
        for (int i = 0; i < tamanho; i++) {
            fseek(f, rrns[i], SEEK_SET);
            char linha[500];
            fgets(linha, sizeof(linha), f);
            char *msg = strtok(linha, ",");
            msg = strtok(NULL, ",");
            msg = strtok(NULL, ",");

            // em caso de colisão, verifica se a mensagem contém mesmo a palavra buscada
            char msg_aux[300];
            strcpy(msg_aux, msg);
            normalizar(msg_aux);
            if (!strstr(msg_aux, palavra))
                continue;

            printf("\n");
            printf("%s", msg);
            printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
        }

        printf("\n");
        fclose(f);
        free(rrns);
    }

    free_tabela_hash(idx);

    return 0;
}