#ifndef TABELA_HASH_H_
#define TABELA_HASH_H_

#include "lista.h"

/**
 * Tabela hash com encadeamento para colisões
 * (Se comporta como uma lista invertida)
 */

typedef struct tabela_hash tabela_hash;

/**
 * Inicializa a tabela
 * 
 * @param size Inteiro que indica o tamanho da tabela a ser criada
 * @return Ponteiro para a tabela criada
 */
tabela_hash* new_tabela_hash(int size);

/**
 * Libera a memória alocada da tabela
 * 
 * @param t Ponteiro para a tabela
 */
void free_tabela_hash(tabela_hash *t);

/**
 * Insere um elemento na tabela
 * 
 * @param t Ponteiro para a tabela
 * @param palavra String usada para o índice/chave da tabela hash
 * @param rrn Inteiro a ser armazenado na tabela hash (lista invertida)
 */
void insere(tabela_hash *t, char *palavra, int rrn);

/**
 * Busca o vetor de elementos associados a certa chave
 * 
 * @param t Ponteiro para a tabela
 * @param palavra String usada para o índice/chave da tabela hash
 * @param tam Ponteiro para um inteiro que armazena quantos elementos estão associados à chave
 * @return Vetor de elementos da lista
 */
int* busca(tabela_hash *t, char *palavra, int *tam);

#endif