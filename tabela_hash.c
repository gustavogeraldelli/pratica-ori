#include "tabela_hash.h"
#include "lista.h"
#include <stdlib.h>
#include <string.h>

typedef struct tabela_hash {
    int size;
    lista **dados;
} tabela_hash;

int hash(char *chave, int size) {
    int valor = 7;
    for (int i = 0; i < strlen(chave); i++)
        valor = 31 * valor + (int)chave[i];
    return ((valor & 0x7FFFFFFF) & 0x7FFFFFFF) % size;
}

tabela_hash* new_tabela_hash(int size) {
    tabela_hash *t = (tabela_hash*)malloc(sizeof(tabela_hash));
    if (t) {
        t->size = size;
        t->dados = (lista**)malloc(sizeof(lista*) * size);
        for (int i = 0; i < size; i++)
            t->dados[i] = NULL;
    }
    return t;
}

void free_tabela_hash(tabela_hash *t) {
    if (t) {
        for (int i = 0; i < t->size; i++)
            free_lista(t->dados[i]);
        free(t->dados);
        free(t);
    }
}

void insere(tabela_hash *t, char *palavra, int rrn) {
    if (!t) return;
    int pos = hash(palavra, t->size);
    if (t->dados[pos] == NULL)
        t->dados[pos] = new_lista();
    insere_lista(t->dados[pos], rrn);
}

int* busca(tabela_hash *t, char *palavra, int *tam) {
    if (!t) return NULL;
    int pos = hash(palavra, t->size);
    return recuperar_dados(t->dados[pos], tam);
}