#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define RESET "\033[0m"
#define VERDE "\033[1;32m"
#define VERMELHO "\033[1;31m"
#define AMARELO "\033[1;33m"
#define CIANO "\033[1;36m"
#define MAGENTA "\033[1;35m"

static void setup_console_utf8(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}


void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}


int bissexto(int a) {
    return (a % 400 == 0) || (a % 4 == 0 && a % 100 != 0);
}

int diasNoMes(int m, int a) {
    static const int d[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return (m == 2 && bissexto(a)) ? 29 : d[m];
}

int dataValida(int d, int m, int a) {
    if (a < 1900) return 0;
    if (m < 1 || m > 12) return 0;
    return d >= 1 && d <= diasNoMes(m, a);
}


void imprimeVenda(Venda v){
    printf("ID: %d\n", v.id);
    printf("Vendedor: %s\n", v.vendedor);
    printf("Matrícula: %s\n", v.matricula);
    printf("Cliente: %s\n", v.cliente);
    printf("Data: %02d/%02d/%04d\n", v.data.dia, v.data.mes, v.data.ano);
    printf("Valor: R$ %.2f\n", v.valor);
}


void adicionarVenda(No **raiz){
    Venda novaVenda;
    int confirmar=0;
    limparTela();

    while(confirmar==0){
        printf("--- Você está no menu de Adicionar uma nova VENDA ---\n");
        printf("\nDigite o nome do VENDEDOR: ");
        limpar_buffer();
        fgets(novaVenda.vendedor, 51, stdin);
        novaVenda.vendedor[strcspn(novaVenda.vendedor, "\n")] = 0;

        printf("\nDigite o nome do CLIENTE: ");
        fgets(novaVenda.cliente, 51, stdin);
        novaVenda.cliente[strcspn(novaVenda.cliente, "\n")] = 0;

        int ok = 0;
        do {
            printf("\nDigite a data da venda (dd mm aaaa): ");
            if (scanf("%d %d %d",
                    &novaVenda.data.dia,
                    &novaVenda.data.mes,
                    &novaVenda.data.ano) != 3) {
                int ch; while ((ch = getchar()) != '\n' && ch != EOF);
                printf("Entrada inválida. Tente novamente.\n");
                continue;
            }

            if (dataValida(novaVenda.data.dia, novaVenda.data.mes, novaVenda.data.ano)) {
                ok = 1;
            } else {
                printf("Data inválida! Tente novamente.\n");
            }
        } while (!ok);

        printf("\nDigite o valor da transação (R$): ");
        scanf("%lf", &novaVenda.valor);

        novaVenda.id = gerarIdUnico(*raiz);
        obterMatriculaParaVendedor(*raiz, novaVenda.vendedor, novaVenda.matricula);

        printf("\nO ID gerado para a venda é: %d\n", novaVenda.id);

        printf("\n--- Você adicionou a VENDA com os seguintes dados: ---\n");
        imprimeVenda(novaVenda);
        printf("\n--- Você confirma as informações ou deseja refazer? (0-Refazer/1-Confirmar): ");
        scanf("%d", &confirmar);
    }

    *raiz = inserir(*raiz, novaVenda);

    printf("\n%s--- VENDA adicionada com sucesso! ---%s\n", VERDE, RESET);
    printf("\n--- Aperte qualquer tecla para continuar ---");

    getchar();

}


void listarVendas(No *raiz){
    limparTela();
    printf("\n--- Você está no menu de Listar VENDAS ---\n");
    printf("\n--- Deseja listar em qual ordem? ---\n");
    printf("\n--- (1)Ordem Crescente (por ID) ---");
    printf("\n--- (2)Ordem Decrescente (por ID) ---\n");
    int opcao;
    scanf("%d",&opcao);
    limparTela();

    if(opcao == 1){
        printf("\n--- Listagem em Ordem Crescente ---\n");
        cabecalhoCompleto();
        listarCrescente(raiz);
        printf("\n--- Pressione qualquer tecla para continuar ---\n");
        limpar_buffer();
        getchar();
    }else if(opcao == 2){
        printf("\n--- Listagem em Ordem Decrescente ---\n");
        cabecalhoCompleto();
        listarDecrescente(raiz);
        printf("\n--- Pressione qualquer tecla para continuar ---\n");
        limpar_buffer();
        getchar();
    }else{
        printf("\n%s--- Opção inválida! ---%s\n", VERMELHO, RESET);
        printf("\n--- Pressione qualquer tecla para continuar ---\n");
        limpar_buffer();
        getchar();
    }
}


void buscarVenda(No *raiz){
    limparTela();
    printf("--- Você está no menu de Buscar uma VENDA ---\n");
    printf("\nDigite parte do nome do VENDEDOR: ");
    char termo[51];
    limpar_buffer();
    fgets(termo, 51, stdin);
    termo[strcspn(termo, "\n")] = 0;

    printf("\n--- Buscando vendas do vendedor: %s ---\n", termo);
    cabecalhoCompleto();
    int qtd = buscarPorVendedor(raiz, termo);
    printf("\n--- %d venda(s) encontrada(s) ---\n", qtd);
    printf("\n--- Aperte qualquer tecla para continuar! ---\n");
    getchar();
}


void filtrarPorValor(No *raiz){
    limparTela();
    printf("\n--- Você está no menu de Filtrar VENDAS por Valor ---\n");
    double valorMin;
    printf("\nDigite o valor mínimo (R$): ");
    scanf("%lf", &valorMin);

    printf("\n--- Vendas com valor >= R$ %.2f ---\n", valorMin);
    cabecalhoResumido();
    int qtd = filtrarPorValorMin(raiz, valorMin);
    printf("\n--- %d venda(s) encontrada(s) ---\n", qtd);
    printf("\n--- Pressione qualquer tecla para continuar ---\n");
    limpar_buffer();
    getchar();
}


void exibirEstatisticas(No *raiz){
    limparTela();
    printf("\n--- Você está no menu de Estatísticas ---\n");

    int n = 0;
    double soma = 0.0, minv = 1e300, maxv = -1e300;
    acumulaEst(raiz, &n, &soma, &minv, &maxv);
    double media = (n > 0) ? (soma / n) : 0.0;

    printf("\n========= Estatísticas Gerais =========\n");
    printf("Quantidade de vendas: %d\n", n);
    printf("Soma total         : R$ %.2f\n", soma);
    printf("Média              : R$ %.2f\n", media);
    printf("Valor mínimo       : R$ %.2f\n", minv);
    printf("Valor máximo       : R$ %.2f\n", maxv);
    printf("\n--- Pressione qualquer tecla para continuar ---\n");
    limpar_buffer();
    getchar();
}


void removerVenda(No **raiz){
    limparTela();
    printf("--- Você está no menu de Remover uma VENDA ---\n");

    int id;
    printf("\nDigite o ID da venda que deseja remover: ");
    scanf("%d", &id);
    limpar_buffer();

    Venda *v = buscarPorId(*raiz, id);
    if (!v) {
        printf("\n%s--- Venda com ID %d não encontrada! ---%s\n", VERMELHO, id, RESET);
        printf("\n--- Pressione qualquer tecla para continuar ---\n");
        getchar();
        return;
    }

    printf("\n--- DADOS DA VENDA A SER REMOVIDA ---\n");
    imprimeVenda(*v);

    char conf[10];
    printf("\n%s--- Deseja realmente remover esta venda? (S/N): %s", AMARELO, RESET);
    fgets(conf, sizeof(conf), stdin);
    conf[strcspn(conf, "\n")] = 0;

    if (conf[0] == 'S' || conf[0] == 's') {
        int removido = 0;
        *raiz = remover(*raiz, id, &removido);
        if (removido)
            printf("\n%s--- Venda removida com sucesso! ---%s\n", VERDE, RESET);
        else
            printf("\n%s--- Erro ao remover venda! ---%s\n", VERMELHO, RESET);
    } else {
        printf("\n%s--- Remoção cancelada! ---%s\n", AMARELO, RESET);
    }

    printf("\n--- Pressione qualquer tecla para continuar ---\n");
    getchar();
}


int main(){

    setup_console_utf8();

    srand((unsigned)time(NULL));

    int opr=0;

    No* raiz = NULL;

    raiz = carregar_dados();
    printf("Pressione qualquer tecla para continuar...\n");
    getchar();


    do{
        limparTela();
        printf("\n          --- Menu de Opções ---\n");
        printf("\n--- (1)Adicionar nova venda ---");
        printf("\n--- (2)Listar todas as vendas ---");
        printf("\n--- (3)Buscar vendas por vendedor ---");
        printf("\n--- (4)Filtrar vendas por valor mínimo ---");
        printf("\n--- (5)Ver estatísticas gerais ---");
        printf("\n--- (6)Remover uma venda ---");
        printf("\n--- (7)Salvar dados ---");
        printf("\n--- (8)Finalizar Sistema ---\n");

        scanf("%d",&opr);

        switch(opr){
            case 1:
                adicionarVenda(&raiz);
                break;
            case 2:
                listarVendas(raiz);
                break;
            case 3:
                buscarVenda(raiz);
                break;
            case 4:
                filtrarPorValor(raiz);
                break;
            case 5:
                exibirEstatisticas(raiz);
                break;
            case 6:
                removerVenda(&raiz);
                break;
            case 7:
                salvar_dados(raiz);
                printf("Pressione qualquer tecla para continuar...\n");
                limpar_buffer();
                getchar();
                break;
            case 8:
                salvar_dados(raiz);
                liberar(raiz);
                printf("Sistema finalizado.\n");
                break;
            default:
                printf("%sOpção inválida! Tente novamente.%s\n", VERMELHO, RESET);
                printf("Pressione qualquer tecla para continuar...\n");
                limpar_buffer();
                getchar();
                break;
        }

    }while(opr!=8);

    return 0;

}
