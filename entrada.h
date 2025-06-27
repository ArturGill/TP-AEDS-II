#ifndef ENTRADA_H
#define ENTRADA_H

#include "ListaOcorrencia.h"
#include "hash.h"

typedef struct 
{
    int qtd_arq;
    char nomes[100][100];
}ListaArquivos;

typedef struct 
{
    char nome[100];
    ListaOcorrencias *ocorrencia;
}PalavraInd;

PalavraInd v[1000];
int total_p;

void IniciaPalavra (PalavraInd *p);
ListaArquivos leitura_arq(char *arq);
void InserePalavraIndice(const char *palavra_texto, int idDoc);
void token_palavras(PalavraInd *pal);
void ler_pocs(ListaArquivos *lista);
void ImprimeIndiceInvertido();

#endif