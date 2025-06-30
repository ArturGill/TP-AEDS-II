#ifndef ENTRADA_H
#define ENTRADA_H
#include "ListaOcorrencia_pat.h"
#include "ListaOcorrencia_Hash.h"


typedef struct TipoPatNo* TipoArvore;
typedef struct TipoLista TipoLista;
typedef unsigned TipoPesos[45][256];
typedef struct ListaArquivos 
{
    int qtd_arq;
    char nomes[100][100];
    int n_i[100];
}ListaArquivos;

typedef struct 
{
    char nome[100];
    ListaOcorrencias_pat *ocorrenciapat;
    ListaOcorrenciasHash *ocorrenciaHash;
}PalavraInd;

PalavraInd v[1000];
int total_p;

void IniciaPalavrapat(PalavraInd *p);
void IniciaPalavrahash(PalavraInd *p);
ListaArquivos leitura_arq(char *arq);
void InserePalavraIndice(const char *palavra_texto, int idDoc);
void token_palavras(PalavraInd *pal);
void ler_pocs(ListaArquivos *lista, TipoLista* Tabela, int M, TipoPesos p,TipoArvore *a);
void ImprimeIndiceInvertido();
int contar_palavras_unicas(ListaArquivos *lista, TipoPesos p_temp);
int achar_primo_inferior(int n);
int eh_primo(int n);
#endif