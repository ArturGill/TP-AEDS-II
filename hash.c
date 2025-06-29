#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "hash.h"
#include "entrada.h"

TipoDicionario Tabela;
TipoPesos p;
TipoApontador i;

static int total_comp_insercaohash = 0;

void FLVaziahash(TipoLista *Lista)
{
  Lista->Primeiro = (TipoCelula *)malloc(sizeof(TipoCelula));
  Lista->Ultimo = Lista->Primeiro;
  Lista->Primeiro->Prox = NULL;
}

short Vaziahash(TipoLista Lista)
{
  return (Lista.Primeiro == Lista.Ultimo);
}
void InsHash(TipoItem x, TipoLista *Lista)
{
  Lista->Ultimo->Prox = (TipoCelula *)malloc(sizeof(TipoCelula));
  Lista->Ultimo = Lista->Ultimo->Prox;
  Lista->Ultimo->Item = x;
  Lista->Ultimo->Prox = NULL;
}

void GeraPesos(TipoPesos p)
{
  int i, j;
  struct timeval semente;
  gettimeofday(&semente, NULL);
  srand((int)(semente.tv_sec + 1000000 * semente.tv_usec));
  for (i = 0; i < N; i++)
    for (j = 0; j < TAMALFABETO; j++)
      p[i][j] = 1 + (int)(10000.0 * rand() / (RAND_MAX + 1.0));
}

TipoIndice h(Palavras palavra, TipoPesos p)
{
  int i;
  unsigned int Soma = 0;
  int comp = strlen(palavra);
  for (i = 0; i < comp; i++)
    Soma += p[i][(unsigned int)palavra[i]];
  return (Soma % M);
}

void Inicializahash(TipoDicionario T)
{
  int i;
  for (i = 0; i < M; i++)
    FLVaziahash(&T[i]);
}

int PesquisaHash(Palavras Ch, TipoPesos p, TipoDicionario T, TipoApontador *Resultado)
{
  int comparacao = 0;
  TipoIndice i;
  TipoApontador Ap;
  i = h(Ch, p);
  *Resultado = NULL;
  if (Vaziahash(T[i]))
  {
    return comparacao;
  }

  Ap = T[i].Primeiro->Prox;
  while (Ap != NULL)
  {
    comparacao ++;
    if (strcmp(Ch, Ap->Item.palavra) == 0)
    {
      *Resultado = Ap;
      return comparacao;
    }
    Ap = Ap->Prox;
  }
  return comparacao;
}

void InsereHash(const char *palavra, int idDoc, TipoPesos p, TipoDicionario T)
{
  TipoApontador Posicao = NULL;
  int comparacao = PesquisaHash((char*)palavra, p, T, &Posicao);
  total_comp_insercaohash += comparacao;

  if (Posicao != NULL)
  {
    insereOuAtualizaOcorrencia(Posicao->Item.Ocorrencia, idDoc);
  }
  else
  { // Palavra nova, precisa ser inserida
    TipoItem novoItem;
    strcpy(novoItem.palavra, palavra);
    // Aloca e inicializa a lista de ocorrências para a nova palavra
    novoItem.Ocorrencia = (ListaOcorrencias *)malloc(sizeof(ListaOcorrencias));
    FLOVazia(novoItem.Ocorrencia);
    // Insere a primeira ocorrência (id do documento)
    insereOuAtualizaOcorrencia(novoItem.Ocorrencia, idDoc);
    // Insere o novo item (palavra + lista de ocorrências) na hash
    InsHash(novoItem, &T[h(novoItem.palavra, p)]);
  }
}
void ImprimirTotalCompInsercao()
{
  printf("Total comparacoes na insercao da hash: %d\n", total_comp_insercaohash);
}

void Imp(TipoLista Lista)
{
  TipoApontador Aux;
  Aux = Lista.Primeiro->Prox;
  while (Aux != NULL)
  {
    printf("%.*s ", N, Aux->Item.palavra);
    Aux = Aux->Prox;
  }
}

void Imprime(TipoDicionario Tabela)
{
  int i;
  for (i = 0; i < M; i++)
  {
    printf("%d: ", i);
    if (!Vaziahash(Tabela[i]))
      Imp(Tabela[i]);
    putchar('\n');
  }
}

int Compara(const void *a, const void *b)
{
  const TipoItem *itemA = (const TipoItem *)a;
  const TipoItem *itemB = (const TipoItem *)b;
  return strncmp(itemA->palavra, itemB->palavra, N);
}

void ImprimeOrdenadohash(TipoDicionario Tabela)
{
  int i;
  TipoApontador Aux;
  int totalDeItens = 0;

  for (i = 0; i < M; i++)
  {
    if (!Vaziahash(Tabela[i]))
    {
      Aux = Tabela[i].Primeiro->Prox;
      while (Aux != NULL)
      {
        totalDeItens++;
        Aux = Aux->Prox;
      }
    }
  }
  if (totalDeItens == 0)
  {
    printf("A tabela está vazia.\n");
    return;
  }
  TipoItem *todosOsItens = malloc(totalDeItens * sizeof(TipoItem));
  if (todosOsItens == NULL)
  {
    printf("Falha ao alocar memoria para ordenacao.\n");
    return;
  }
  int k = 0;
  for (i = 0; i < M; i++)
  {
    if (!Vaziahash(Tabela[i]))
    {
      Aux = Tabela[i].Primeiro->Prox;
      while (Aux != NULL)
      {
        todosOsItens[k] = Aux->Item;
        k++;
        Aux = Aux->Prox;
      }
    }
  }
  qsort(todosOsItens, totalDeItens, sizeof(TipoItem), Compara);
  printf("--- Indice Invertido (Hash) ---\n");
  for (i = 0; i < totalDeItens; i++)
  {
    printf("%s -> ", todosOsItens[i].palavra);
    if (todosOsItens[i].Ocorrencia != NULL)
    {
      Ocorrencia *atual = todosOsItens[i].Ocorrencia->Primeiro;
      while (atual != NULL)
      {
        printf("<%d, %d> ", atual->item.id, atual->item.qtde);
        atual = atual->prox;
      }
    }
    printf("\n");
  }
  printf("----------------------------------\n");
  free(todosOsItens);
}

// Função de comparação para o qsort, para ordenar os resultados por relevancia (decrescente)
int comparar_resultados(const void *a, const void *b)
{
  ResultadoBusca *resA = (ResultadoBusca *)a;
  ResultadoBusca *resB = (ResultadoBusca *)b;
  if (resA->relev < resB->relev)
    return 1;
  if (resA->relev > resB->relev)
    return -1;
  return 0;
}

void buscar_por_relevancia_hash(const char *consulta, struct ListaArquivos *docs, TipoDicionario T, TipoPesos p)
{
  // 1. Processar os termos da consulta
  char copia_consulta[200];
  strcpy(copia_consulta, consulta);

  char *termos_consulta[20]; // Suporta até 20 termos na consulta
  int qtd_termos_consulta = 0;
  char *token = strtok(copia_consulta, " ");
  while (token != NULL && qtd_termos_consulta < 20)
  {
    // usando a mesma logica de tokenizaçao(nao da para usar a funçao)
    int i, j = 0;
    char c;
    for (i = 0; token[i] != '\0'; i++)
    {
      c = token[i];
      if (c >= 'A' && c <= 'Z')
        c += 32; // Converte para minúscula
      if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
      {
        token[j++] = c;
      }
    }
    token[j] = '\0';
    if (strlen(token) > 0)
    {
      termos_consulta[qtd_termos_consulta++] = token;
    }

    token = strtok(NULL, " ");
  }

  int V = docs->qtd_arq;
  ResultadoBusca resultados[V];
  int resultados_count = 0;
  int total_comp_busca_hash = 0;
  // 2. Calcular a relevância r(i) para cada documento i
  for (int i = 0; i < V; i++)
  {
    double soma_pesos_w = 0.0;
    int n_i = docs->n_i[i]; // Número de termos distintos no documento i
    int doc_id = i + 1;

    if (n_i == 0)
      continue; // Pula documentos vazios

    // Para cada termo j na consulta
    for (int j = 0; j < qtd_termos_consulta; j++)
    {
      const char *termo_j = termos_consulta[j];
      TipoApontador no_palavra = NULL;

      // Pesquisa o termo na hash
      PesquisaHash((char *)termo_j, p, T, &no_palavra);

      if (no_palavra != NULL)
      {
        // f_ji = frequência do termo j no documento i
        int f_ji = obter_fji(no_palavra->Item.Ocorrencia, doc_id);

        if (f_ji > 0)
        {
          // d_j = número de documentos que contêm o termo j
          int d_j = obter_dj(no_palavra->Item.Ocorrencia);

          // w_ji = f_ji * log2(N / d_j)
          double w_ji = (double)f_ji * log2((double)N / d_j);
          soma_pesos_w += w_ji;
        }
      }
    }

    // r(i) = (1 / n_i) * soma_pesos_w
    if (soma_pesos_w > 0)
    {
      double r_i = (1.0 / n_i) * soma_pesos_w;
      strcpy(resultados[resultados_count].nome_arquivo, docs->nomes[i]);
      resultados[resultados_count].relev = r_i;
      resultados_count++;
    }
  }
  for (int j = 0; j < qtd_termos_consulta; j++) {
      const char *termo_j = termos_consulta[j];
      TipoApontador temp = NULL;
      int comp_termo = PesquisaHash((char*)termo_j, p, T, &temp);
      total_comp_busca_hash += comp_termo;
  }
  printf("Total comparacoes para busca: %d\n", total_comp_busca_hash);

  // 3. Ordenar os resultados por relevância
  qsort(resultados, resultados_count, sizeof(ResultadoBusca), comparar_resultados);
  if (resultados_count == 0)
  {
    printf("Nenhum documento relevante encontrado.\n");
  }
  else
  {
    printf("Documentos encontrados (em ordem de relevancia):\n");
    for (int i = 0; i < resultados_count; i++)
    {
      printf("%d. %s \n", i + 1, resultados[i].nome_arquivo);
    }
  }
}
