// TAD fila de números inteiros
// Carlos Maziero - DINF/UFPR, Out 2024
//
// Definição da interface do TAD - NÃO ALTERAR

#ifndef fila
#define fila

// estrutura de um item da fila
struct item_t
{
  int valor ;			// valor do item
  struct item_t *ant ;		// item anterior
  struct item_t *prox ;	// próximo item
} ;

// estrutura de uma fila
struct fila_t
{
  struct item_t *prim ;	// primeiro item
  struct item_t *ult ;		// último item
  int tamanho ;		// número de itens da fila
} ;

// Cria uma fila vazia.
// Retorno: ponteiro p/ a fila ou NULL em erro.
struct fila_t *fila_cria ();

// Remove todos os itens da fila e libera a memória.
// Retorno: NULL.
struct fila_t *fila_destroi (struct fila_t *lst);

// Nas operações insere/retira/consulta/procura, a fila inicia na
// posição 0 (primeiro item) e termina na posição TAM-1 (último item).

// Insere o item na fila na posição indicada;
// se a posição for além do fim da fila ou for -1, insere no fim.
// Retorno: número de itens na fila após a operação ou -1 em erro.
int fila_insere (struct fila_t *lst, int item);

// Retira o item da fila da posição indicada.
// se a posição for -1, retira do fim.
// Retorno: número de itens na fila após a operação ou -1 em erro.
int fila_retira (struct fila_t *lst, int *item);

// Informa o valor do item na posição indicada, sem retirá-lo.
// se a posição for -1, consulta do fim.
// Retorno: número de itens na fila ou -1 em erro.
int fila_consulta (struct fila_t *lst, int *item, int pos);

// Informa a posição da 1ª ocorrência do valor indicado na fila.
// Retorno: posição do valor ou -1 se não encontrar ou erro.
int fila_procura (struct fila_t *lst, int valor);

// Informa o tamanho da fila (o número de itens presentes nela).
// Retorno: número de itens na fila ou -1 em erro.
int fila_tamanho (struct fila_t *lst);

// Imprime o conteúdo da fila do inicio ao fim no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois, sem newline.
void fila_imprime (struct fila_t *lst);

#endif
