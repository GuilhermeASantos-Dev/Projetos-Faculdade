#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#define MAX_PRODUTOS 100

typedef struct frutas {
    char nome[50];
    float preco_kg;
    float preco_uni;
    int vendido_por_peso;
} frutas;

void gravaARQ(frutas *pF, int f) {
    FILE *arq = fopen("frutas.txt", "wb");
    if (arq == NULL) {
        puts("Erro na abertura do arquivo para gravação.");
        exit(1);
    }

    int frutas_cadastradas = fwrite(pF, sizeof(frutas), f, arq);
    if (frutas_cadastradas != f) {
        puts("Erro na escrita do arquivo.");
    } else {
        puts("Dados gravados com sucesso.");
    }

    fclose(arq);
}

int leArquivo(frutas *pF) {
    FILE *arq = fopen("frutas.txt", "rb");
    if (arq == NULL) {
        puts("Erro na abertura do arquivo para leitura.");
        return 0;
    }

    int frutas_lidas = fread(pF, sizeof(frutas), MAX_PRODUTOS, arq);
    fclose(arq);

    if (frutas_lidas == 0) {
        puts("Nenhum produto encontrado no arquivo.");
    }
    return frutas_lidas;
}

void cadastrar_produto(frutas *pF, int *i) {
    printf("\nCadastro de Produto\n");
    printf("Digite o nome do produto: ");
    scanf("%s", pF[*i].nome);

    printf("O produto é vendido por peso? (1 - Sim, 0 - Não): ");
    scanf("%d", &pF[*i].vendido_por_peso);

    if (pF[*i].vendido_por_peso) {
        printf("Digite o preço por quilo: ");
        scanf("%f", &pF[*i].preco_kg);
        pF[*i].preco_uni = 0;
    } else {
        printf("Digite o preço por unidade: ");
        scanf("%f", &pF[*i].preco_uni);
        pF[*i].preco_kg = 0;
    }

    (*i)++;
    printf("Produto cadastrado com sucesso!\n");
}

void listar_produtos(frutas produtos[], int num_produtos) {
    printf("Lista de produtos disponíveis:\n");
    for (int i = 0; i < num_produtos; i++) {
        printf("%d. %s\n", i + 1, produtos[i].nome);
        if (produtos[i].vendido_por_peso) {
            printf("Preço por Kg: R$ %.2f\n", produtos[i].preco_kg);
        } else {
            printf("Preço por Unidade: R$ %.2f\n", produtos[i].preco_uni);
        }
    }
}

void alterar_produto(frutas *produtos, int num_produtos) {
    int cod_produto;
    printf("Digite o código do produto que deseja alterar: ");
    scanf("%d", &cod_produto);

    if (cod_produto < 1 || cod_produto > num_produtos) {
        printf("Código inválido.\n");
        return;
    }

    frutas *produto = &produtos[cod_produto - 1];
    printf("Novo nome do produto: ");
    scanf("%s", produto->nome);

    printf("O produto é vendido por peso? (1 - Sim, 0 - Não): ");
    scanf("%d", &produto->vendido_por_peso);

    if (produto->vendido_por_peso) {
        printf("Digite o novo preço por quilo: ");
        scanf("%f", &produto->preco_kg);
        produto->preco_uni = 0;
    } else {
        printf("Digite o novo preço por unidade: ");
        scanf("%f", &produto->preco_uni);
        produto->preco_kg = 0;
    }

    printf("Produto alterado com sucesso!\n");
}

float realizar_venda(frutas produtos[], int num_produtos) {
    int cod_produto;
    float quantidade, total = 0;
    char continuar;

    do {
        listar_produtos(produtos, num_produtos);
        printf("Digite o código do produto: ");
        scanf("%d", &cod_produto);

        if (cod_produto < 1 || cod_produto > num_produtos) {
            printf("Produto inválido.\n");
            continue;
        }

        frutas produto_selecionado = produtos[cod_produto - 1];
        printf("Digite a quantidade (peso ou unidades): ");
        scanf("%f", &quantidade);

        float preco_total;
        if (produto_selecionado.vendido_por_peso) {
            preco_total = quantidade * produto_selecionado.preco_kg;
        } else {
            preco_total = quantidade * produto_selecionado.preco_uni;
        }
        printf("Total do produto %s: R$ %.2f\n", produto_selecionado.nome, preco_total);
        total += preco_total;

        printf("Deseja adicionar outro produto? (S/N): ");
        scanf(" %c", &continuar);

    } while (continuar == 'S' || continuar == 's');

    printf("Total da compra: R$ %.2f\n", total);
    return total;
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    frutas produtos[MAX_PRODUTOS];
    int num_produtos = leArquivo(produtos);  
    int opcao;
    

    do {
       printf("------ SEJA BEM VINDO AO HORTIFRUTI DO FUTURO ------"); 
	   printf("\n1. Cadastrar Produto\n");
        printf("2. Listar Produtos\n");
        printf("3. Realizar Venda\n");
        printf("4. Alterar Produto\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrar_produto(produtos, &num_produtos);
                gravaARQ(produtos, num_produtos); 
                break;
            case 2:
                listar_produtos(produtos, num_produtos);
                break;
            case 3:
                if (num_produtos == 0) {
                    printf("Nenhum produto cadastrado. Por favor, cadastre produtos antes de realizar a venda.\n");
                } else {
                    realizar_venda(produtos, num_produtos);
                }
                break;
            case 4:
                alterar_produto(produtos, num_produtos);
                gravaARQ(produtos, num_produtos); 
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 5);

    return 0;
}

/*Trata-se de um sistema básico para gerenciamento de um hortifruti, onde é possível cadastrar produtos, listar os itens disponíveis, realizar vendas e atualizar informações dos produtos.*/