#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "entrada.h"
#include "hash.h"
#include "patricia.h"


int main() {
    TipoPesos p_temp;
    GeraPesos(p_temp);
    TipoArvore a = NULL;
    ListaArquivos entrada;
    entrada = leitura_arq("entrada.txt");

    int l = contar_palavras_unicas(&entrada, p_temp);
    int M = achar_primo_inferior(l);
    TipoLista* Tabela_h = (TipoLista*) malloc(M * sizeof(TipoLista));
    if(Tabela_h == NULL) {
        printf("Falha critica de alocacao de memoria!\n");
        return 1;
    }
    Inicializahash(Tabela_h, M);
    TipoPesos p_principal;
    GeraPesos(p_principal);


    
    int op;
    char termo_busca[200];
  
        do {
        printf("Escolha uma opcao:\n");
        printf("1. Receber arquivos de entrada\n");
        printf("2. Ler e construir os indices invertidos\n");
        printf("3. Imprimir os indices invertidos\n");
        printf("4. Buscar termos (Hash)\n");
        printf("5. Buscar termos (Patricia)\n");
        printf("6. Comparacoes insercao\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar(); 

        switch (op)
        {
            case 1:
                entrada = leitura_arq("arquivos/entrada.txt");
                if (entrada.qtd_arq > 0){
                    printf("Arquivos carregados (%d):\n", entrada.qtd_arq);
                }
                break;

            case 2:
                ler_pocs(&entrada, Tabela_h, M, p_principal,&a);
                break;

            case 3:
                printf("Indice Invertido (Hash - ordenado):\n");
                ImprimeOrdenadohash(Tabela_h, M);
                ImprimirTotalCompInsercaohash();
                printf("Indice Invertido (Patricia- ordenado):\n");
                ImprimirEmOrdem_Patricia(a);
                break;

            case 4:
                printf("Digite os termos para a busca: ");
                if (fgets(termo_busca, sizeof(termo_busca), stdin)) {
                    termo_busca[strcspn(termo_busca, "\n")] = '\0';  // remove o '\n'
                    buscar_por_relevancia_hash(termo_busca, &entrada, Tabela_h, M, p_principal);
                }
                break;
            case 5:
                printf("Digite os termos para a busca: ");
                if (fgets(termo_busca, sizeof(termo_busca), stdin)) {
                    termo_busca[strcspn(termo_busca, "\n")] = '\0';
                    buscar_por_relevancia_patricia(termo_busca, &entrada, a);
                }
                break;
            
            case 6:
                ImprimirTotalCompInsercaohash();
                Imprime_quntd_comp_inserc_pat();
                break;
            case 0:
                printf("Programa encerrado\n");
                break;

            default:
                printf("Erro. Escolha novamente\n");
        }

        printf("\n");
    } while(op != 0);
    
    return 0;
}