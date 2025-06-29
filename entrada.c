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

int contar_palavras_unicas(ListaArquivos *lista, TipoPesos p_temp) {
    const int M_TEMP = 10007; // Um primo grande para a tabela temporária
    TipoLista* tabela_temp = (TipoLista*) malloc(M_TEMP * sizeof(TipoLista));
    Inicializahash(tabela_temp, M_TEMP);
    int n = 0; // Contador de palavras únicas

    for (int i = 0; i < lista->qtd_arq; i++) {
        FILE *arq = fopen(lista->nomes[i], "r");
        if (!arq) continue;

        PalavraInd p_temp_struct;
        while (fscanf(arq, "%s", p_temp_struct.nome) == 1) {
            token_palavras(&p_temp_struct);
            if (strlen(p_temp_struct.nome) > 0) {
                TipoApontador resultado = NULL;
                // Pesquisa na tabela temporária para ver se a palavra é nova
                PesquisaHash(p_temp_struct.nome, p_temp, tabela_temp, M_TEMP, &resultado);
                if (resultado == NULL) {
                    n++; // Se não encontrou, é uma palavra nova. Incrementa o contador.
                    TipoItem item_fantasma;
                    strcpy(item_fantasma.palavra, p_temp_struct.nome);
                    item_fantasma.Ocorrencia = NULL; // Não precisamos guardar ocorrências aqui
                    InsHash(item_fantasma, &tabela_temp[h(item_fantasma.palavra, p_temp, M_TEMP)]);
                }
            }
        }
        fclose(arq);
    }
    free(tabela_temp); 
    return n;
}

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

void ler_pocs(ListaArquivos *lista, TipoLista* Tabela, int M, TipoPesos p)
{
    TipoArvore a = NULL;
    int i;
    for(int i=0; i<lista->qtd_arq; i++)
    {
        lista->n_i[i] = 0;
    }

    for (i = 0; i < lista->qtd_arq; i++)
    {
        FILE *arq = fopen(lista->nomes[i], "r");
        if (!arq) 
        {
            printf("Erro ao abrir o arquivo: %s\n", lista->nomes[i]);
            continue;
        }
        const int M_TEMP_NI = 1009; // Tabela temporária para contar n_i
        TipoLista* tabela_ni = (TipoLista*) malloc(M_TEMP_NI * sizeof(TipoLista));
        Inicializahash(tabela_ni, M_TEMP_NI);
        TipoPesos p_ni;
        GeraPesos(p_ni);

        
        PalavraInd p_temp;
        int idDoc = i + 1;
        while (fscanf(arq, "%s", p_temp.nome) == 1)
        {
            token_palavras(&p_temp);
            if(strlen(p_temp.nome) > 0)
            {
                InserePalavraIndice(p_temp.nome, idDoc); 
                InsereHash(p_temp.nome, idDoc, p, Tabela, M);
                a = Insere(p_temp.nome, &a,idDoc);
                

            
                TipoApontador res_ni = NULL;
                PesquisaHash(p_temp.nome, p_ni, tabela_ni, M_TEMP_NI, &res_ni);
                if(res_ni == NULL){
                    lista->n_i[i]++;
                    TipoItem item_ni;
                    strcpy(item_ni.palavra, p_temp.nome);
                    item_ni.Ocorrencia = NULL;
                    InsHash(item_ni, &tabela_ni[h(item_ni.palavra, p_ni, M_TEMP_NI)]);
                }
            }
        }
        free(tabela_ni);
        fclose(arq);
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

int eh_primo(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    }
    return 1;
}

// Função para encontrar o maior primo menor ou igual a n
int achar_primo_inferior(int n) {
    if (n <= 1) return 2;
    for (int i = n; i > 1; i--) {
        if (eh_primo(i)) {
            return i;
        }
    }
    return 2;
}