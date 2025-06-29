#ifndef LISTAOCORRENCIA_PAT_H
#define LISTAOCORRENCIA_PAT_H


typedef struct ItemPal_pat
{
    int id, qtde;
}ItemPal_pat;


typedef struct Ocorrencia_pat
{
    ItemPal_pat item;
    struct Ocorrencia_pat *prox;
}Ocorrencia_pat;

typedef struct ListaOcorrencias_pat 
{
    Ocorrencia_pat *Primeiro, *Ultimo ;
}ListaOcorrencias_pat;

void FLOVaziaPat(ListaOcorrencias_pat *lista);
int insereOuAtuOcorrPat(ListaOcorrencias_pat *lista, int idDoc);
int obter_fjiPat(ListaOcorrencias_pat* lista, int idDoc);
int obter_djPat(ListaOcorrencias_pat* lista);
#endif