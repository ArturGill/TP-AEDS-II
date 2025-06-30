#ifndef PATRICIA_H
#define PATRICIA_H

#define tam_max 45
#include "ListaOcorrencia_pat.h"
#include "entrada.h"

typedef char string[tam_max];
typedef struct TipoChave{
    char* chave;
    ListaOcorrencias_pat ocorrencia;
}TipoChave;

typedef unsigned char TipoIndexAmp;
typedef unsigned char TipoDib;
typedef enum {
  Interno, Externo
} TipoNo;

typedef struct TipoPatNo* TipoArvore; 
typedef struct TipoPatNo {
  TipoNo nt;
  union {
    struct {
      TipoIndexAmp Index;
      TipoArvore Esq, Dir;
    } NInterno ;
    TipoChave Chaveext;
  } NO;
} TipoPatNo;

typedef struct {
    char nome_arquivo[100];
    double relev;
} ResultadoBuscapat;

TipoArvore Pesquisa_no(string k, TipoArvore t,int* qunt_comp_busca);
void buscar_por_relevancia_patricia(const char *consulta, ListaArquivos *docs, TipoArvore a);
void ImprimirEmOrdem_Patricia(TipoArvore a);
void DestruirArvore(TipoArvore a);
TipoDib Bit(TipoIndexAmp i, char* k);
TipoArvore Insere(char* k, TipoArvore *t,int id_doc);
TipoArvore InsereEntre(char* k, TipoArvore *t, int i,int id_doc);
TipoArvore CriaNoExt(char* k,int id_doc);
TipoArvore CriaNoInt(int i, TipoArvore *Esq,  TipoArvore *Dir);
short EExterno(TipoArvore p);
int comparar_resultadospat(const void *a, const void *b);
void Imprime_quntd_comp_inserc_pat();
#endif