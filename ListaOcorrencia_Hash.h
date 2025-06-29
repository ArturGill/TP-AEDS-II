#ifndef LISTAOCORRENCIA_HASH_H
#define LISTAOCORRENCIA_HASH_H

typedef struct ItemPal
{
    int id, qtde;
}ItemPal;


typedef struct OcorrenciaHash
{
    ItemPal item;
    struct OcorrenciaHash *prox;
}OcorrenciaHash;

typedef struct ListaOcorrencias 
{
    OcorrenciaHash *Primeiro, *Ultimo ;
}ListaOcorrenciasHash;

void FLOVaziaHash(ListaOcorrenciasHash *lista);
int insereOuAtuOcorrHash(ListaOcorrenciasHash *lista, int idDoc);
int obter_fji(ListaOcorrenciasHash* lista, int idDoc);
int obter_dj(ListaOcorrenciasHash* lista);
#endif