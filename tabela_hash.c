#include <stdlib.h>
#include <string.h>
#include "tabela_hash.h"
#include "Set.h"

typedef struct tabela_hash {
    int tamanho;
    Set **valores;
} tabela_hash;

int hash(char *chave, int tamanho) {
    int valor = 7;
    for (int i = 0; i < strlen(chave); i++)
        valor = 31 * valor + (int)chave[i];
    return ((valor & 0x7FFFFFFF) & 0x7FFFFFFF) % tamanho;
}

tabela_hash* new_tabela_hash(int tamanho) {
    tabela_hash *t = (tabela_hash*)malloc(sizeof(tabela_hash));
    if (t) {
        t->tamanho = tamanho;
        t->valores = (Set**)malloc(sizeof(Set*) * tamanho);
        for (int i = 0; i < tamanho; i++)
            t->valores[i] = NULL;
    }
    return t;
}

void free_tabela_hash(tabela_hash *t) {
    if (t) {
        for (int i = 0; i < t->tamanho; i++)
            liberaSet(t->valores[i]);
        free(t->valores);
        free(t);
    }
}

void inserir_tabela_hash(tabela_hash *t, char *chave, int valor) {
    if (!t) return;
    int pos = hash(chave, t->tamanho);
    if (!t->valores[pos])
        t->valores[pos] = criaSet();
    insereSet(t->valores[pos], valor);
}

Set* buscar_tabela_hash(tabela_hash *t, char *chave) {
    if (!t) return NULL;
    int pos = hash(chave, t->tamanho);
    return t->valores[pos];
}