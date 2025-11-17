#ifndef ARVORE_H
#define ARVORE_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 50
#define MAX_MATR 8 /* "V" + "ddd" + '\0' = 5; 8 dá folga */

#define RESET "\033[0m"
#define VERDE "\033[1;32m"
#define VERMELHO "\033[1;31m"
#define AMARELO "\033[1;33m"

typedef struct {
  int dia, mes, ano;
} Data;

typedef struct {
  int id;
  char vendedor[MAX_NOME + 1];
  char matricula[MAX_MATR];
  char cliente[MAX_NOME + 1];
  Data data;
  double valor;
} Venda;

typedef struct No {
  Venda venda;
  struct No *esq, *dir;
} No;

static void tolower_str(const char *src, char *dst, size_t n) {
  size_t i = 0;
  for (; i + 1 < n && src[i]; ++i)
    dst[i] = (char)tolower((unsigned char)src[i]);
  dst[i] = '\0';
}

static int containsIgnoreCase(const char *hay, const char *needle) {
  char a[MAX_NOME + 1], b[MAX_NOME + 1];
  tolower_str(hay, a, sizeof(a));
  tolower_str(needle, b, sizeof(b));
  return strstr(a, b) != NULL;
}

static int iguaisIgnoreCase(const char *a, const char *b) {
  while (*a && *b) {
    char ca = (char)tolower((unsigned char)*a);
    char cb = (char)tolower((unsigned char)*b);
    if (ca != cb)
      return 0;
    ++a;
    ++b;
  }
  return *a == '\0' && *b == '\0';
}

static void cabecalhoCompleto(void) {
  printf("\nID   | Vendedor            | Matrícula | Cliente             | "
         "Data       |      Valor\n");
  printf("-----+---------------------+-----------+---------------------+-------"
         "-----+------------\n");
}

static void cabecalhoResumido(void) {
  printf("\nID   | Cliente             | Data       |      Valor\n");
  printf("-----+---------------------+------------+------------\n");
}

static void imprimirVendaCompleta(const Venda *v) {
  printf("%4d | %-21s | %-9s | %-19s | %02d/%02d/%04d | %10.2f\n", v->id,
         v->vendedor, v->matricula, v->cliente, v->data.dia, v->data.mes,
         v->data.ano, v->valor);
}

static void imprimirVendaResumida(const Venda *v) {
  printf("%4d | %-19s | %02d/%02d/%04d | %10.2f\n", v->id, v->cliente,
         v->data.dia, v->data.mes, v->data.ano, v->valor);
}

static void gerarMatricula(char out[MAX_MATR]) {
  int n = rand() % 1000; /* 000..999 */
  out[0] = 'V';
  sprintf(out + 1, "%03d", n); /* "V042" */
}

static int buscarMatriculaPorVendedor(No *r, const char *vendedor,
                                      char matricula[MAX_MATR]) {
  if (!r)
    return 0;
  if (buscarMatriculaPorVendedor(r->esq, vendedor, matricula))
    return 1;
  if (iguaisIgnoreCase(r->venda.vendedor, vendedor)) {
    strcpy(matricula, r->venda.matricula);
    return 1;
  }
  return buscarMatriculaPorVendedor(r->dir, vendedor, matricula);
}

static void obterMatriculaParaVendedor(No *raiz, const char *vendedor,
                                       char matricula[MAX_MATR]) {
  if (buscarMatriculaPorVendedor(raiz, vendedor, matricula))
    return; /* já havia uma matrícula para esse vendedor */
  gerarMatricula(matricula);
}

static No *novoNo(Venda v) {
  No *n = (No *)malloc(sizeof(No));
  if (!n) {
    printf("Falha de memória!\n");
    exit(1);
  }
  n->venda = v;
  n->esq = n->dir = NULL;
  return n;
}

static No *minNo(No *r) {
  while (r && r->esq)
    r = r->esq;
  return r;
}

static No *inserir(No *raiz, Venda v) {
  if (raiz == NULL)
    return novoNo(v);
  if (v.id < raiz->venda.id)
    raiz->esq = inserir(raiz->esq, v);
  else if (v.id > raiz->venda.id)
    raiz->dir = inserir(raiz->dir, v);
  else {
    v.id++;
    raiz->dir = inserir(raiz->dir, v);
  }
  return raiz;
}

static No *remover(No *raiz, int id, int *removido) {
  if (!raiz)
    return NULL;
  if (id < raiz->venda.id)
    raiz->esq = remover(raiz->esq, id, removido);
  else if (id > raiz->venda.id)
    raiz->dir = remover(raiz->dir, id, removido);
  else {
    *removido = 1;
    if (!raiz->esq) {
      No *t = raiz->dir;
      free(raiz);
      return t;
    } else if (!raiz->dir) {
      No *t = raiz->esq;
      free(raiz);
      return t;
    } else {
      No *m = minNo(raiz->dir);
      raiz->venda = m->venda;
      raiz->dir = remover(raiz->dir, m->venda.id, removido);
    }
  }
  return raiz;
}

static void liberar(No *r) {
  if (!r)
    return;
  liberar(r->esq);
  liberar(r->dir);
  free(r);
}

static int idExiste(No *r, int id) {
  if (!r)
    return 0;
  if (r->venda.id == id)
    return 1;
  if (id < r->venda.id)
    return idExiste(r->esq, id);
  else
    return idExiste(r->dir, id);
}

static int gerarIdUnico(No *raiz) {
  int id;
  int tentativas = 0;
  do {
    id = 1000 + (rand() % 9000); /* gera entre 1000 e 9999 */
    tentativas++;
    if (tentativas > 10000) {
      /* fallback: se muitas tentativas, busca sequencial */
      for (id = 1000; id <= 9999; id++) {
        if (!idExiste(raiz, id))
          break;
      }
      break;
    }
  } while (idExiste(raiz, id));
  return id;
}

static Venda *buscarPorId(No *r, int id) {
  if (!r)
    return NULL;
  if (r->venda.id == id)
    return &r->venda;
  if (id < r->venda.id)
    return buscarPorId(r->esq, id);
  else
    return buscarPorId(r->dir, id);
}

static void listarCrescente(No *r) {
  if (!r)
    return;
  listarCrescente(r->esq);
  imprimirVendaCompleta(&r->venda);
  listarCrescente(r->dir);
}

static void listarDecrescente(No *r) {
  if (!r)
    return;
  listarDecrescente(r->dir);
  imprimirVendaCompleta(&r->venda);
  listarDecrescente(r->esq);
}

static int buscarPorVendedor(No *r, const char *nome) {
  if (!r)
    return 0;
  int achou = 0;
  achou += buscarPorVendedor(r->esq, nome);
  if (containsIgnoreCase(r->venda.vendedor, nome)) {
    imprimirVendaCompleta(&r->venda);
    achou++;
  }
  achou += buscarPorVendedor(r->dir, nome);
  return achou;
}

static int filtrarPorValorMin(No *r, double minValor) {
  if (!r)
    return 0;
  int cont = 0;
  cont += filtrarPorValorMin(r->esq, minValor);
  if (r->venda.valor >= minValor) {
    imprimirVendaResumida(&r->venda);
    cont++;
  }
  cont += filtrarPorValorMin(r->dir, minValor);
  return cont;
}

static void acumulaEst(No *r, int *n, double *soma, double *minv,
                       double *maxv) {
  if (!r)
    return;
  acumulaEst(r->esq, n, soma, minv, maxv);
  (*n)++;
  *soma += r->venda.valor;
  if (r->venda.valor < *minv)
    *minv = r->venda.valor;
  if (r->venda.valor > *maxv)
    *maxv = r->venda.valor;
  acumulaEst(r->dir, n, soma, minv, maxv);
}

static void percorrerESalvar(No *r, FILE *arq) {
  if (!r)
    return;
  percorrerESalvar(r->esq, arq);
  fprintf(arq, "%d;%s;%s;%s;%d;%d;%d;%.2f\n", r->venda.id, r->venda.vendedor,
          r->venda.matricula, r->venda.cliente, r->venda.data.dia,
          r->venda.data.mes, r->venda.data.ano, r->venda.valor);
  percorrerESalvar(r->dir, arq);
}

static void salvar_dados(No *raiz) {
  FILE *arq = fopen("vendas.txt", "w");
  if (arq == NULL) {
    printf("%sErro ao abrir o arquivo para salvar!%s\n", VERMELHO, RESET);
    return;
  }

  percorrerESalvar(raiz, arq);

  fclose(arq);
  printf("%sDados salvos com sucesso!%s\n", VERDE, RESET);
}

static No *carregar_dados(void) {
  FILE *arq = fopen("vendas.txt", "r");
  if (arq == NULL) {
    printf("%sArquivo de dados não encontrado. Iniciando um novo sistema.%s\n", AMARELO, RESET);
    return NULL;
  }

  No *raiz = NULL;
  Venda tempVenda;

  while (fscanf(arq, "%d;%[^;];%[^;];%[^;];%d;%d;%d;%lf\n", &tempVenda.id,
                tempVenda.vendedor, tempVenda.matricula, tempVenda.cliente,
                &tempVenda.data.dia, &tempVenda.data.mes, &tempVenda.data.ano,
                &tempVenda.valor) == 8) {
    raiz = inserir(raiz, tempVenda);
  }

  fclose(arq);
  printf("%sDados carregados com sucesso!%s\n", VERDE, RESET);
  return raiz;
}

#endif /* ARVORE_H */
