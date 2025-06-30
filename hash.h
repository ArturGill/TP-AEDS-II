#ifndef HASH_H
#define HASH_H

#include "ListaOcorrencia_Hash.h"
#include "entrada.h"


#define N 45     
#define TAMALFABETO 256


typedef char Palavras[N];
typedef unsigned TipoPesos[N][TAMALFABETO];

typedef struct TipoItem {
  Palavras palavra;
  ListaOcorrenciasHash *Ocorrencia;
} TipoItem;

typedef unsigned int TipoIndice;
typedef struct TipoCelula* TipoApontador;

typedef struct TipoCelula {
  TipoItem Item;
  TipoApontador Prox;
} TipoCelula;

typedef struct TipoLista {
  TipoCelula *Primeiro, *Ultimo;
} TipoLista;

struct ListaArquivos;

extern TipoPesos p;
extern TipoApontador i;

typedef struct {
    char nome_arquivo[100];
    double relev;
} ResultadoBusca;



void FLVaziahash(TipoLista *Lista);
short Vaziahash(TipoLista Lista);
void InsHash(TipoItem x, TipoLista *Lista);
void GeraPesos(TipoPesos p);
TipoIndice h(Palavras Chave, TipoPesos p, int M);
void Inicializahash(TipoLista* Tabela, int M);
int PesquisaHash(Palavras Ch, TipoPesos p, TipoLista* T, int M, TipoApontador *Resultado);
void InsereHash(const char *palavra, int idDoc, TipoPesos p, TipoLista* T, int M);
void Imprime(TipoLista* Tabela, int M);
int Compara (const void *a, const void *b);
void ImprimeOrdenadohash(TipoLista* Tabela, int M);
void buscar_por_relevancia_hash(const char* consulta, struct ListaArquivos *docs, TipoLista* T, int M, TipoPesos p);
void ImprimirTotalCompInsercaohash();

#endif