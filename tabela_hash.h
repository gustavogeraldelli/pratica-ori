#ifndef TABELA_HASH_H_
#define TABELA_HASH_H_

#include "Set.h"

/**
 * Tabela hash com encadeamento para colisões
 * (Se comporta como um indice invertida)
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
 * @param chave String usada para calcular o índice da tabela hash
 * @param valor Inteiro a ser armazenado na tabela hash (indice invertido)
 */
void inserir_tabela_hash(tabela_hash *t, char *chave, int valor);

/**
 * Busca o vetor de elementos associados a certa chave
 * 
 * @param t Ponteiro para a tabela
 * @param chave String usada para o índice/chave da tabela hash
 * @param tam Ponteiro para um inteiro que armazenará quantos elementos estão associados à chave
 * @return Estrutura que contém os valores associados à chave
 */
Set* buscar_tabela_hash(tabela_hash *t, char *chave);

#endif