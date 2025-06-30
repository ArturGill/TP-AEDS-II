#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include "patricia.h" 

static int total_comp_insercaopat = 0;


TipoDib Bit(TipoIndexAmp i, string k) {
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

TipoArvore CriaNoExt(string k,int id_doc) {
  TipoArvore p = (TipoArvore)malloc(sizeof(TipoPatNo));
  p->nt = Externo;
  p->NO.Chaveext.chave = (char*)malloc(strlen(k) + 1);
  strcpy(p->NO.Chaveext.chave, k);
  FLOVaziaPat(&p->NO.Chaveext.ocorrencia);
  insereOuAtuOcorrPat(&p->NO.Chaveext.ocorrencia, id_doc);
  return p;
}


TipoArvore InsereEntre(string k, TipoArvore *t, int i,int id_doc) {
  TipoArvore p;
  if (EExterno(*t) || i < (*t)->NO.NInterno.Index) {
    p = CriaNoExt(k,id_doc);
    if (Bit(i, k))
      return CriaNoInt(i, t, &p);
    else
      return CriaNoInt(i, &p, t);
    } else {
    if (Bit((*t)->NO.NInterno.Index, k))
      (*t)->NO.NInterno.Dir = InsereEntre(k, &(*t)->NO.NInterno.Dir, i,id_doc);
    else
      (*t)->NO.NInterno.Esq = InsereEntre(k, &(*t)->NO.NInterno.Esq, i,id_doc);
    return *t;
  }
}

TipoArvore Insere(string k, TipoArvore *t,int id_doc) {
    
  if (*t == NULL) return CriaNoExt(k,id_doc);

  TipoArvore p = *t;
  while (!EExterno(p)) {
    total_comp_insercaopat+=1;
    if (Bit(p->NO.NInterno.Index, k)){
      p = p->NO.NInterno.Dir;
    }
    else{
      p = p->NO.NInterno.Esq;
    }
  }

  if (strcmp(k, p->NO.Chaveext.chave) == 0) {
    insereOuAtuOcorrPat(&p->NO.Chaveext.ocorrencia,id_doc);
    return *t;
  }

  int i = 1;
  while ((Bit(i, k) == Bit(i, p->NO.Chaveext.chave))) {  
    i++;
  }

  return InsereEntre(k, t, i,id_doc);
}
void DestruirArvore(TipoArvore a) {
  if (a == NULL) return;
  if (EExterno(a)) {
    free(a->NO.Chaveext.chave);
    free(a);
  } else {
    DestruirArvore(a->NO.NInterno.Esq);
    DestruirArvore(a->NO.NInterno.Dir);
    free(a);
  }
}
void ImprimirEmOrdem_Patricia(TipoArvore a) {

  if (a == NULL) return;

  if (EExterno(a)) {
    printf("%s -> ", a->NO.Chaveext.chave);
    Ocorrencia_pat *atual = a->NO.Chaveext.ocorrencia.Primeiro;
    while (atual != NULL) {
      printf("<%d, %d> ",atual->item.qtde, atual->item.id );
      atual = atual->prox;
    }
    printf("\n"); 
  } else {
    ImprimirEmOrdem_Patricia(a->NO.NInterno.Esq);
    ImprimirEmOrdem_Patricia(a->NO.NInterno.Dir);
  }
}
TipoArvore Pesquisa_no(string k, TipoArvore t,int* qunt_comp_busca) {
  if (t == NULL) {
    return NULL; 
  }
  (*qunt_comp_busca)+=1;
  if (EExterno(t)) {
    if (strcmp(k, t->NO.Chaveext.chave) == 0) {
      return t; 
    } else {
      return NULL; 
    }
  }
  if (Bit(t->NO.NInterno.Index, k) == 0) {
    return Pesquisa_no(k, t->NO.NInterno.Esq,qunt_comp_busca);
  } else {
    return Pesquisa_no(k, t->NO.NInterno.Dir,qunt_comp_busca);
  }
}
void buscar_por_relevancia_patricia(const char *consulta, ListaArquivos *docs, TipoArvore a) {
    char copia_consulta[200];
    strncpy(copia_consulta, consulta, sizeof(copia_consulta) - 1);
    copia_consulta[sizeof(copia_consulta) - 1] = '\0';

    char *termos_consulta[20];
    int qtd_termos_consulta = 0;
    char *token = strtok(copia_consulta, " \t\n");
    while (token != NULL && qtd_termos_consulta < 20) {
        int i, j = 0;
        char c;
        for (i = 0; token[i] != '\0'; i++) {
            c = token[i];
            if (c >= 'A' && c <= 'Z') c += 32;
            if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
                token[j++] = c;
            }
        }
        token[j] = '\0';
        if (strlen(token) > 0) {
            termos_consulta[qtd_termos_consulta++] = token;
        }
        token = strtok(NULL, " \t\n");
    }

    if (qtd_termos_consulta == 0) {
        printf("Nenhum termo válido na consulta.\n");
        return;
    }

    int n = docs->qtd_arq;
    ResultadoBuscapat resultados[n];
    int resultados_count = 0;
    int qunt_comp_pesquisa = 0;

    for (int i = 0; i < n; i++) {
        double soma_pesos_w = 0.0;
        int n_i = docs->n_i[i];
        int doc_id = i + 1;

        if (n_i == 0) continue;

        for (int j = 0; j < qtd_termos_consulta; j++) {
          const char *termo_j = termos_consulta[j];
            
          TipoArvore no_palavra = Pesquisa_no((char*)termo_j, a,&qunt_comp_pesquisa);

          if (no_palavra != NULL) {
            int f_ji = obter_fjiPat(&no_palavra->NO.Chaveext.ocorrencia, doc_id);

            if (f_ji > 0) {
              int d_j = obter_djPat(&no_palavra->NO.Chaveext.ocorrencia);
              if (d_j > 0) { 
                double w_ji = (double)f_ji * log2((double)n / d_j);
                soma_pesos_w += w_ji;
              }
            }
          }
        }
        if (soma_pesos_w > 0) {
          double r_i = (1.0 / n_i) * soma_pesos_w;
          strcpy(resultados[resultados_count].nome_arquivo, docs->nomes[i]);
          resultados[resultados_count].relev = r_i;
          resultados_count++;
        }
    }
    qsort(resultados, resultados_count, sizeof(ResultadoBuscapat), comparar_resultadospat);

    if (resultados_count == 0) {
        printf("Nenhum documento relevante encontrado para a consulta.\n");
    } else {
        for (int i = 0; i < resultados_count; i++) {
            printf("%d. %s \n", i + 1, resultados[i].nome_arquivo);
        }
        printf("Quantidade de comparacoes na pesquisa: %d",qunt_comp_pesquisa);
    }
}
int comparar_resultadospat(const void *a, const void *b){
  ResultadoBuscapat *resA = (ResultadoBuscapat *)a;
  ResultadoBuscapat *resB = (ResultadoBuscapat *)b;
  if (resA->relev < resB->relev)
    return 1;
  if (resA->relev > resB->relev)
    return -1;
  return 0;
}

void Imprime_quntd_comp_inserc_pat(){
  printf("Total de comparacoes em todas as insercoes patricia: %d\n",total_comp_insercaopat);
}
