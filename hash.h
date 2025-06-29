#ifndef HASH_H
#define HASH_H

#include "ListaOcorrencia_Hash.h"

#define M 7
#define N 100      
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

typedef TipoLista TipoDicionario[M];
struct ListaArquivos;

extern TipoDicionario Tabela;
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
TipoIndice h(Palavras Chave, TipoPesos p);
void Inicializahash(TipoDicionario T);
TipoApontador PesquisaHash(Palavras Ch, TipoPesos p, TipoDicionario T);
void InsereHash(const char *palavra, int idDoc, TipoPesos p, TipoDicionario T);
void Imp(TipoLista Lista);
void Imprime(TipoDicionario Tabela);
int Compara (const void *a, const void *b);
void ImprimeOrdenadohash(TipoDicionario Tabela);
void buscar_por_relevancia_hash(const char* consulta, struct ListaArquivos *docs, TipoDicionario T, TipoPesos p);

#endif