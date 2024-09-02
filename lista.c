#include "lista.h"
#include <stdlib.h>

typedef struct node {
    int dado;
    struct node* prox;
} node;

typedef struct lista {
    struct node *head;
} lista;

node* new_node(int dado) {
    node *n = (node*)malloc(sizeof(node));
    if (n) {
        n->dado = dado;
        n->prox = NULL;
    }
    return n;
}

lista* new_lista() {
    lista *l = (lista*)malloc(sizeof(lista));
    if (l)
        l->head = NULL;
    return l;
}

int vazia(lista *l) {
    return !l || l->head == NULL;
}

void free_lista(lista *l) {
    if (!l) return;
    while (!vazia(l))
        remove_lista(l);
    free(l);
}

void insere_lista(lista *l, int dado) {
    node *n = new_node(dado);
    if (!l || !n) return;
    node *aux = l->head;
    while (aux) {
        if (aux->dado == dado) return;
        aux = aux->prox; // se uma palavra se repete em um tweet, não repetir rrn
    }
    n->prox = l->head;
    l->head = n;
}

void remove_lista(lista *l) {
    if (!l || vazia(l)) return;
    node *n = l->head;
    l->head = l->head->prox;
    free(n);
}

int tamanho_lista(lista *l) {
    if (!l) return 0;
    int tam = 0;
    node *n = l->head;
    while (n) {
        tam++;
        n = n->prox;
    }
    return tam;
}

int* recuperar_dados(lista *l, int *tam) {
    if (!l) return NULL;
    int tamanho = tamanho_lista(l);
    int* dados = (int*)malloc(sizeof(int) * tamanho);
    if (!dados) return NULL;
    node *n = l->head;

    if (tam)
        *tam = tamanho;

    for (int i = 0; i < tamanho; i++) {
        dados[i] = n->dado;
        n = n->prox;
    }
    return dados;
}