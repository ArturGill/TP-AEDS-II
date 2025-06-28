#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "patricia.h" 


TipoDib Bit(TipoIndexAmp i, TipoChave k) {
  if (i == 0) return 0;

  int byte_index = (i - 1) / 8;
  int bit_index = (i - 1) % 8;

  unsigned char c;
  if (byte_index >= strlen(k)) {
    c = 0; 
  } else {
    c = k[byte_index];
  }

  return (c & (1 << (7 - bit_index))) != 0;
}
short EExterno(TipoArvore p) {
  return (p->nt == Externo);
}

TipoArvore CriaNoInt(int i, TipoArvore *Esq, TipoArvore *Dir) {
  TipoArvore p = (TipoArvore)malloc(sizeof(TipoPatNo));
  p->nt = Interno;
  p->NO.NInterno.Index = i;
  p->NO.NInterno.Esq = *Esq;
  p->NO.NInterno.Dir = *Dir;
  return p;
}

TipoArvore CriaNoExt(TipoChave k) {
  TipoArvore p = (TipoArvore)malloc(sizeof(TipoPatNo));
  p->nt = Externo;
  p->NO.Chave = (TipoChave)malloc(strlen(k) + 1);
  strcpy(p->NO.Chave, k);
  return p;
}


void Pesquisa(TipoChave k, TipoArvore t) {
  if (t == NULL) {
    printf("Elemento nao encontrado\n");
    return;
  }
  if (EExterno(t)) {
    if (strcmp(k, t->NO.Chave) == 0){
      printf("Elemento encontrado\n");
    }
    else{
      printf("Elemento nao encontrado\n");
    }
    return;
  }
  if (Bit(t->NO.NInterno.Index, k) == 0)
    Pesquisa(k, t->NO.NInterno.Esq);
  else
    Pesquisa(k, t->NO.NInterno.Dir);
}

TipoArvore InsereEntre(TipoChave k, TipoArvore *t, int i) {
  TipoArvore p;
  if (EExterno(*t) || i < (*t)->NO.NInterno.Index) {
    p = CriaNoExt(k);
    if (Bit(i, k))
      return CriaNoInt(i, t, &p);
    else
      return CriaNoInt(i, &p, t);
  } else {
    if (Bit((*t)->NO.NInterno.Index, k))
      (*t)->NO.NInterno.Dir = InsereEntre(k, &(*t)->NO.NInterno.Dir, i);
    else
      (*t)->NO.NInterno.Esq = InsereEntre(k, &(*t)->NO.NInterno.Esq, i);
    return *t;
  }
}

TipoArvore Insere(TipoChave k, TipoArvore *t) {
  if (*t == NULL) return CriaNoExt(k);

  TipoArvore p = *t;
  while (!EExterno(p)) {
    if (Bit(p->NO.NInterno.Index, k)){
      p = p->NO.NInterno.Dir;
    }
    else{
      p = p->NO.NInterno.Esq;
    }
  }

  if (strcmp(k, p->NO.Chave) == 0) {
    printf("Erro: chave \"%s\" ja esta na arvore\n", k);
    return *t;
  }

  int i = 1;
 while ((Bit(i, k) == Bit(i, p->NO.Chave))) {
    i++;
}

  return InsereEntre(k, t, i);
}
void DestruirArvore(TipoArvore a) {
  if (a == NULL) return;
  if (EExterno(a)) {
    free(a->NO.Chave);
    free(a);
  } else {
    DestruirArvore(a->NO.NInterno.Esq);
    DestruirArvore(a->NO.NInterno.Dir);
    free(a);
  }
}