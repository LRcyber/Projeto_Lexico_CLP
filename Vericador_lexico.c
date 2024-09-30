#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <locale.h>
#define MAX_PILHA 100

// Definindo tipos de tokens
typedef enum {
    TOKEN_IDENTIFICADOR,
    TOKEN_NUMERO,
    TOKEN_OPERADOR,
    TOKEN_PARENTESE,
    TOKEN_COLCHETE,
    TOKEN_CHAVE,
    TOKEN_DESCONHECIDO,
    TOKEN_FIM
} TipoToken;

// Estrutura para armazenar um token
typedef struct {
    TipoToken tipo;
    char valor[100];
} Token;

// Estrutura da pilha
typedef struct {
    char dados[MAX_PILHA];
    int topo;
} Pilha;

// Inicializar pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verificar se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Adicionar elemento na pilha
void empilhar(Pilha *p, char c) {
    if (p->topo < MAX_PILHA - 1) {
        p->dados[++(p->topo)] = c;
    }
}

// Remover elemento da pilha
char desempilhar(Pilha *p) {
    if (!pilhaVazia(p)) {
        return p->dados[(p->topo)--];
    }
    return '\0';
}

// Verificar se os símbolos de abertura e fechamento são correspondentes
int simbolosCorrespondem(char abertura, char fechamento) {
    return (abertura == '(' && fechamento == ')') ||
           (abertura == '{' && fechamento == '}') ||
           (abertura == '[' && fechamento == ']');
}

// Função que identifica se o caractere é um operador
int ehOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Função que verifica se é um símbolo de agrupamento
int ehSimboloAgrupamento(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']';
}

// Função que imprime o tipo do token
void imprimeToken(Token token) {
    switch (token.tipo) {
        case TOKEN_IDENTIFICADOR:
            printf("Identificador: %s\n", token.valor);
            break;
        case TOKEN_NUMERO:
            printf("Número: %s\n", token.valor);
            break;
        case TOKEN_OPERADOR:
            printf("Operador: %s\n", token.valor);
            break;
        case TOKEN_PARENTESE:
        case TOKEN_COLCHETE:
        case TOKEN_CHAVE:
            printf("Símbolo de agrupamento: %s\n", token.valor);
            break;
        case TOKEN_DESCONHECIDO:
            printf("Desconhecido: %s\n", token.valor);
            break;
        case TOKEN_FIM:
            printf("Fim da entrada\n");
            break;
    }
}

// Função que realiza a análise léxica e verifica o balanceamento dos símbolos
void analiseLexica(const char* codigo) {
    int i = 0;
    Pilha pilhaSimbolos;
    Token token;
    inicializarPilha(&pilhaSimbolos);
    int balanceado = 1; // Variável para armazenar se os símbolos estão balanceados

    while (codigo[i] != '\0') {
        // Ignorar espaços em branco
        if (isspace(codigo[i])) {
            i++;
            continue;
        }

        // Identificar identificadores (variáveis ou palavras-chave)
        if (isalpha(codigo[i])) {
            int j = 0;
            while (isalnum(codigo[i])) {
                token.valor[j++] = codigo[i++];
            }
            token.valor[j] = '\0';
            token.tipo = TOKEN_IDENTIFICADOR;
            imprimeToken(token);
            continue;
        }

        // Identificar números
        if (isdigit(codigo[i])) {
            int j = 0;
            while (isdigit(codigo[i])) {
                token.valor[j++] = codigo[i++];
            }
            token.valor[j] = '\0';
            token.tipo = TOKEN_NUMERO;
            imprimeToken(token);
            continue;
        }

        // Identificar operadores
        if (ehOperador(codigo[i])) {
            token.valor[0] = codigo[i];
            token.valor[1] = '\0';
            token.tipo = TOKEN_OPERADOR;
            imprimeToken(token);
            i++;
            continue;
        }

        // Identificar símbolos de agrupamento (parênteses, colchetes, chaves)
        if (ehSimboloAgrupamento(codigo[i])) {
            token.valor[0] = codigo[i];
            token.valor[1] = '\0';

            // Verificação de abertura e fechamento
            if (codigo[i] == '(' || codigo[i] == '{' || codigo[i] == '[') {
                empilhar(&pilhaSimbolos, codigo[i]);
            } else if (codigo[i] == ')' || codigo[i] == '}' || codigo[i] == ']') {
                if (!pilhaVazia(&pilhaSimbolos)) {
                    char abertura = desempilhar(&pilhaSimbolos);
                    if (!simbolosCorrespondem(abertura, codigo[i])) {
                        balanceado = 0;
                    }
                } else {
                    balanceado = 0;
                }
            }

            // Definir o tipo de token
            if (codigo[i] == '(' || codigo[i] == ')') token.tipo = TOKEN_PARENTESE;
            else if (codigo[i] == '{' || codigo[i] == '}') token.tipo = TOKEN_CHAVE;
            else if (codigo[i] == '[' || codigo[i] == ']') token.tipo = TOKEN_COLCHETE;

            imprimeToken(token);
            i++;
            continue;
        }

        // Qualquer outro caractere será desconhecido
        token.valor[0] = codigo[i];
        token.valor[1] = '\0';
        token.tipo = TOKEN_DESCONHECIDO;
        imprimeToken(token);
        i++;
    }

    // Fim da entrada
    token.tipo = TOKEN_FIM;
    imprimeToken(token);

    // Verificar se todos os símbolos de abertura têm fechamento
    if (!pilhaVazia(&pilhaSimbolos)) {
        balanceado = 0;
    }

    if (balanceado) {
        printf("Símbolos de agrupamento estão balanceados.\n");
    } else {
        printf("Erro: Símbolos de agrupamento não estão balanceados.\n");
    }
}

int main() {
    const char* codigo = "x = 10 + (y - 5) * [3 + 2] - {z};";
    setlocale(LC_ALL, "Portuguese");
    analiseLexica(codigo);
    return 0;
}
