#ifndef LISTA_H_
#define LISTA_H_

/**
 * Estrutura auxiliar para resolver o encadeamento de colisões na tabela
 */

typedef struct node node;
typedef struct lista lista;

/**
 * Inicializa a lista
 * 
 * @return Ponteiro para a lista criada
 */
lista* new_lista();

/**
 * Libera a memória alocada da lista
 * 
 * @param l Ponteiro para a lista
 */
void free_lista(lista *l);

/**
 * Insere um elemento no início da lista
 * 
 * @param l Ponteiro para a lista
 * @param dado Inteiro a ser armazenado na lista
 */
void insere_lista(lista *l, int dado);

/**
 * Remove um elemento no início da lista
 * 
 * @param l Ponteiro para a lista
 */
void remove_lista(lista *l);

/**
 * Retorna um vetor dos elementos (inteiros) armazenados pela lista
 * 
 * @param l Ponteiro para a lista
 * @param tam Ponteiro para um inteiro que armazena quantos elementos estão associados à chave
 * @return Vetor de elementos da lista
 */
int* recuperar_dados(lista *l, int *tam);

#endif