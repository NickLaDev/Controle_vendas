# 🛒 Sistema de Gerenciamento de Vendas

[![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://www.learn-c.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-blue?style=for-the-badge)]()

Sistema completo de gerenciamento de vendas desenvolvido em C puro, utilizando **Árvore Binária de Busca (ABB)** como estrutura de dados principal. Projeto acadêmico com foco em boas práticas de programação e experiência do usuário.

---

## 📋 Índice

- [Características](#-características)
- [Estrutura de Dados](#-estrutura-de-dados)
- [Funcionalidades](#-funcionalidades)
- [Compilação e Execução](#-compilação-e-execução)
- [Exemplos de Uso](#-exemplos-de-uso)
- [Arquitetura do Projeto](#-arquitetura-do-projeto)
- [Diferenciais Implementados](#-diferenciais-implementados)
- [Autor](#-autor)

---

## ✨ Características

- 🌳 **Árvore Binária de Busca** para gerenciamento eficiente de dados
- 💾 **Persistência automática** de dados em arquivo texto
- 🎨 **Interface colorida** com códigos ANSI
- ✅ **Validação robusta** de datas (anos bissextos, dias do mês)
- 🔒 **Confirmação de ações destrutivas** (remoção de vendas)
- 🔢 **IDs únicos aleatórios** (1000-9999)
- 📊 **Estatísticas completas** (média, mínimo, máximo, soma)
- 🔍 **Busca por vendedor** com suporte a pesquisa parcial
- 🖥️ **Multiplataforma** (Linux e Windows)

---

## 🌳 Estrutura de Dados

O sistema utiliza uma **Árvore Binária de Busca (ABB)** para armazenar e gerenciar as vendas. A escolha dessa estrutura proporciona:

- **Inserção**: O(log n) em média
- **Busca**: O(log n) em média
- **Remoção**: O(log n) em média
- **Listagem ordenada**: O(n) através de percurso in-order

```c
typedef struct No {
    Venda venda;
    struct No *esq, *dir;
} No;

typedef struct {
    int id;
    char vendedor[51];
    char matricula[8];
    char cliente[51];
    Data data;
    double valor;
} Venda;
```

---

## 🚀 Funcionalidades

### 1. **Adicionar Nova Venda**
- Entrada de dados do vendedor, cliente, data e valor
- Validação completa de datas
- Geração automática de ID único
- Sistema de confirmação antes de salvar

### 2. **Listar Vendas**
- Listagem em ordem crescente (por ID)
- Listagem em ordem decrescente (por ID)
- Visualização formatada em tabela

### 3. **Buscar Vendas por Vendedor**
- Busca por nome parcial (case-insensitive)
- Exibição de todas as vendas do vendedor encontrado

### 4. **Filtrar por Valor Mínimo**
- Listar vendas acima de um valor especificado
- Útil para análise de vendas de alto valor

### 5. **Estatísticas Gerais**
- Quantidade total de vendas
- Soma total de valores
- Valor médio das vendas
- Valor mínimo e máximo

### 6. **Remover Venda**
- Busca por ID
- Exibição dos dados antes da remoção
- Confirmação obrigatória (S/N)

### 7. **Salvar Dados**
- Salvamento manual em arquivo `vendas.txt`
- Salvamento automático ao sair do sistema

---

## 🔧 Compilação e Execução

### Linux/Unix

```bash
# Compilar
gcc -Wall -Wextra -O2 projeto_vendas.c -o projeto_vendas

# Executar
./projeto_vendas
```

### Windows

```bash
# Compilar
gcc -Wall -Wextra -O2 projeto_vendas.c -o projeto_vendas.exe

# Executar
projeto_vendas.exe
```

### Requisitos
- GCC 7.0 ou superior
- Sistema operacional: Linux, macOS ou Windows

---

## 📸 Exemplos de Uso

### Menu Principal

```
          --- Menu de Opções ---

--- (1)Adicionar nova venda ---
--- (2)Listar todas as vendas ---
--- (3)Buscar vendas por vendedor ---
--- (4)Filtrar vendas por valor mínimo ---
--- (5)Ver estatísticas gerais ---
--- (6)Remover uma venda ---
--- (7)Salvar dados ---
--- (8)Finalizar Sistema ---
```

### Exemplo de Listagem

```
ID   | Vendedor            | Matrícula | Cliente             | Data       |      Valor
-----+---------------------+-----------+---------------------+------------+------------
5432 | João Silva          | V123      | Maria Santos        | 15/11/2025 |    1500.50
7821 | Pedro Costa         | V456      | Ana Lima            | 20/11/2025 |    2300.75
```

### Estatísticas

```
========= Estatísticas Gerais =========
Quantidade de vendas: 5
Soma total         : R$ 8500.25
Média              : R$ 1700.05
Valor mínimo       : R$ 450.00
Valor máximo       : R$ 2300.75
```

---

## 📁 Arquitetura do Projeto

```
Projeto2/
├── projeto_vendas.c    # Arquivo principal com interface e menu
├── arvore.h            # Implementação da ABB e operações
├── vendas.txt          # Arquivo de persistência de dados
├── README.md           # Documentação do projeto
└── MELHORIAS.md        # Detalhes das melhorias implementadas
```

### Módulos Principais

**`projeto_vendas.c`**
- Interface do usuário
- Menu principal
- Funções de interação (adicionar, listar, buscar, etc.)
- Validação de entrada

**`arvore.h`**
- Estruturas de dados (ABB, Venda, Data)
- Operações na árvore (inserir, remover, buscar)
- Persistência de dados (carregar/salvar)
- Funções auxiliares (validação, formatação)

---

## 🎯 Diferenciais Implementados

### ✅ Funcionalidades Avançadas

1. **ID Aleatório com Collision Detection**
   - Geração de IDs entre 1000-9999
   - Verificação automática de duplicatas
   - Fallback para busca sequencial quando necessário

2. **Validação Completa de Datas**
   - Suporte a anos bissextos
   - Validação de dias por mês
   - Verificação de anos válidos (>= 1900)

3. **Sistema de Matrícula Inteligente**
   - Matrícula única por vendedor (formato: V + 3 dígitos)
   - Reutilização automática quando vendedor já existe

4. **Persistência Robusta**
   - Formato CSV delimitado por ponto-e-vírgula
   - Carregamento automático ao iniciar
   - Salvamento automático ao sair
   - Opção de salvamento manual

5. **Interface Amigável**
   - Mensagens coloridas contextuais
   - Feedback visual para todas as operações
   - Sistema de confirmação para ações destrutivas
   - Validação de entrada com tratamento de erros

### 🎨 Qualidade do Código

- ✅ Código limpo e legível
- ✅ Nomenclatura consistente
- ✅ Tratamento adequado de erros
- ✅ Gerenciamento correto de memória
- ✅ Modularização apropriada
- ✅ Comentários quando necessário

---

## 🔍 Formato do Arquivo de Dados

O arquivo `vendas.txt` utiliza o seguinte formato:

```
ID;Vendedor;Matrícula;Cliente;Dia;Mês;Ano;Valor
```

Exemplo:
```
5432;João Silva;V123;Maria Santos;15;11;2025;1500.50
7821;Pedro Costa;V456;Ana Lima;20;11;2025;2300.75
3214;Carlos Souza;V789;Beatriz Alves;25;11;2025;980.00
```

---

## 🛠️ Tecnologias Utilizadas

- **Linguagem**: C (padrão C99)
- **Estrutura de Dados**: Árvore Binária de Busca
- **Compilador**: GCC
- **Controle de Versão**: Git
- **Sistema Operacional**: Linux/Windows

---

## 📚 Conceitos Aplicados

- Estruturas de Dados (ABB)
- Alocação Dinâmica de Memória
- Recursão
- Manipulação de Arquivos
- Validação de Entrada
- Algoritmos de Busca e Ordenação
- Modularização de Código

---

## 🎓 Contexto Acadêmico

Este projeto foi desenvolvido como parte da disciplina de **Estrutura de Dados** do curso de Ciência da Computação. O objetivo principal é demonstrar o domínio de árvores binárias de busca e suas aplicações práticas em sistemas reais.

### Requisitos Atendidos

- ✅ Implementação de ABB
- ✅ Operações de inserção, remoção e busca
- ✅ Percursos in-order (crescente e decrescente)
- ✅ Persistência de dados
- ✅ Interface de usuário funcional
- ✅ Validação de dados
- ✅ Documentação completa

---

## 📝 Licença

Este projeto está sob a licença MIT. Veja o arquivo `LICENSE` para mais detalhes.

---

## 👤 Autor

**Desenvolvido com dedicação e atenção aos detalhes**

- 🎓 Curso: Ciência da Computação
- 📚 Disciplina: Estrutura de Dados
- 🏫 Instituição: PUC

---

## 🤝 Contribuições

Sugestões e melhorias são sempre bem-vindas! Sinta-se à vontade para:

1. Fazer um fork do projeto
2. Criar uma branch para sua feature (`git checkout -b feature/MinhaFeature`)
3. Commit suas mudanças (`git commit -m 'Adiciona nova feature'`)
4. Push para a branch (`git push origin feature/MinhaFeature`)
5. Abrir um Pull Request

---

## 📧 Contato

Para dúvidas ou sugestões sobre o projeto, entre em contato através do GitHub.

---

<div align="center">

**⭐ Se este projeto foi útil para você, considere dar uma estrela!**

Desenvolvido com ❤️ e muito ☕

</div>
