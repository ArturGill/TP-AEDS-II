#include <stdio.h>
#include "ListaOcorrencia.h"
#define tam_max 45
typedef char string[tam_max];
typedef struct TipoChave{
    char* chave;
    ListaOcorrencias ocorrencias;
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
void ImprimirEmOrdem_Patricia(TipoArvore a);
void DestruirArvore(TipoArvore a);
TipoDib Bit(TipoIndexAmp i, char* k);
TipoArvore Insere(char* k, TipoArvore *t,int id_doc);
TipoArvore InsereEntre(char* k, TipoArvore *t, int i,int id_doc);
void Pesquisa(char* k, TipoArvore t);
TipoArvore CriaNoExt(char* k);
TipoArvore CriaNoInt(int i, TipoArvore *Esq,  TipoArvore *Dir);
short EExterno(TipoArvore p);