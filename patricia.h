#include <stdio.h>
typedef char* TipoChave; /* a definir, dependendo da aplicacao */
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
    TipoChave Chave;
  } NO;
} TipoPatNo;

void DestruirArvore(TipoArvore a);
TipoDib Bit(TipoIndexAmp i, TipoChave k);
TipoArvore Insere(TipoChave k, TipoArvore *t);
TipoArvore InsereEntre(TipoChave k, TipoArvore *t, int i);
void Pesquisa(TipoChave k, TipoArvore t);
TipoArvore CriaNoExt(TipoChave k);
TipoArvore CriaNoInt(int i, TipoArvore *Esq,  TipoArvore *Dir);
short EExterno(TipoArvore p);