#ifndef ENTRADA_H
#define ENTRADA_H

#include "hash.h"
#include "patricia.h"

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

void IniciaPalavra (PalavraInd *p);
ListaArquivos leitura_arq(char *arq);
void InserePalavraIndice(const char *palavra_texto, int idDoc);
void token_palavras(PalavraInd *pal);
void ler_pocs(ListaArquivos *lista, TipoLista* Tabela, int M, TipoPesos p);
void ImprimeIndiceInvertido();
int contar_palavras_unicas(ListaArquivos *lista, TipoPesos p_temp);
int achar_primo_inferior(int n);
int eh_primo(int n);
#endif