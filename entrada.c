#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>
#include "patricia.h"
#include "entrada.h"

PalavraInd v[1000];
int total_p = 0;

void IniciaPalavra(PalavraInd *p)
{
    p->ocorrencia = (ListaOcorrencias*)malloc(sizeof(ListaOcorrencias));
    FLOVazia(p->ocorrencia);
}

ListaArquivos leitura_arq(char *arq)
{
    int i;
    ListaArquivos lista;
    lista.qtd_arq = 0;

    FILE *entrada = fopen(arq, "r");
    if (!entrada)
    {
        printf("Erro ao abrir os arquivo(s).\n");
        exit(1);
    }

    fscanf(entrada, "%d", &lista.qtd_arq);
    fgetc(entrada);
    for (i = 0; i < lista.qtd_arq; i++)
    {
        fscanf(entrada, "%s", lista.nomes[i]);
    }
    fclose(entrada);
    return (lista);
}

// void Insere(PalavraInd *pal)
// {
//     int i;
//     for (i = 0; i < total_p; i++)
//     {
//         if (strcmp(v[i].nome, pal->nome) == 0 && v[i].ocorrencia->Primeiro->item.id == pal->ocorrencia->Primeiro->item.id)
//         {
//             v[i].ocorrencia->Primeiro->item.qtde++;
//             return;
//         }
//     }
//     strcpy(v[total_p].nome, pal->nome);
//     v[total_p].ocorrencia->Primeiro->item.id = pal->ocorrencia->Primeiro->item.id;
//     v[total_p].ocorrencia->Primeiro->item.qtde = 1;
//     total_p++;
// }

void InserePalavraIndice(const char *p, int idDoc) { // teria que passar a patrica e uma hash para a insercao final
    int i;
    for (i = 0; i < total_p; i++) {
        if (strcmp(v[i].nome, p) == 0) { // Palavra já existe no índice geral 
            insereOuAtualizaOcorrencia(v[i].ocorrencia, idDoc);
            return; 
        }
    }
    if (total_p < 1000) { // adiciona palavra nova
        strcpy(v[total_p].nome, p);
        IniciaPalavra(&v[total_p]);
        insereOuAtualizaOcorrencia(v[total_p].ocorrencia, idDoc);
        total_p++;
    }
}

void token_palavras(PalavraInd *pal)
{
    int i, j = 0;
    char c;
    for (i = 0; pal->nome[i] != '\0'; i++)
    {
        c = pal->nome[i];
        if (c >= 'A' && c <= 'Z')
            c += 32; // minúscula
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
        {
            pal->nome[j++] = c;
        }
    }
    pal->nome[j] = '\0';
}

void ler_pocs(ListaArquivos *lista)
{
    TipoArvore a = NULL;
    int i;
    for (i = 0; i < lista->qtd_arq; i++)
    {
        FILE *arq = fopen(lista->nomes[i], "r");
        if (!arq) 
        {
            printf("Erro ao abrir o arquivo: %s\n", lista->nomes[i]);
            continue;
        }
        char (*palavras_unicas)[100] = malloc(10000 * sizeof(char[100]));
        if (palavras_unicas == NULL) {
            printf("Falha ao alocar memoria!\n");
            continue;
        }
        int tot_unicas=0;
        PalavraInd p_temp;
        int idDoc = i + 1;
        while (fscanf(arq, "%s", p_temp.nome) == 1)
        {
            token_palavras(&p_temp);
            if(strlen(p_temp.nome) > 0)
            {
                InserePalavraIndice(p_temp.nome, idDoc); 
                InsereHash(p_temp.nome, idDoc, p, Tabela);
                a = Insere(p_temp.nome, &a,idDoc);
                

            
                int encontrada = 0;
                for(int k = 0; k < tot_unicas; k++)
                {
                    if(strcmp(palavras_unicas[k], p_temp.nome) == 0)
                    {
                        encontrada = 1;
                        break;
                    }
                }
                if(!encontrada && tot_unicas < 10000){
                    strcpy(palavras_unicas[tot_unicas],p_temp.nome);
                    tot_unicas++;
                }
            }            
        }
        
        fclose(arq);
        lista->n_i[i] = tot_unicas;
        free(palavras_unicas);
    }
    ImprimirEmOrdem_Patricia(a);
}

void ImprimeIndiceInvertido() {
    printf("\n--- Conteudo do Indice Invertido (vetor) ---\n");
    if (total_p == 0) {
        printf("Indice invertido vazio.\n");
        return;
    }
    // Percorre o vetor 'v' 
    for (int i = 0; i < total_p; i++) {
        printf("%s -> ", v[i].nome);

        if (v[i].ocorrencia != NULL) {
            Ocorrencia *atual = v[i].ocorrencia->Primeiro;
            // Percorre a lista encadeada de ocorrências para a palavra atual
            while (atual != NULL) {
                printf("<%d, %d> ", atual->item.id, atual->item.qtde);
                atual = atual->prox;
            }
        } 
        else {
            printf("Nenhuma ocorrencia (erro logico ou palavra ainda sem doc)");
        }
        printf("\n"); // Nova linha para a próxima palavra
    }
    printf("----------------------------------\n");
}