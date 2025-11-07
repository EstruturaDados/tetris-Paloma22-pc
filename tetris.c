
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5   // tamanho máximo da fila
#define TAM_PILHA 3  // tamanho máximo da pilha

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // tipo da peça: 'I', 'O', 'T', 'L'
    int id;     // identificador único da peça
} Peca;

// Estrutura da fila de peças (circular)
typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int qtd;
} Fila;

// Estrutura da pilha de peças reservadas
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// ---------- PROTÓTIPOS ----------
void inicializarFila(Fila *fila);
void inicializarPilha(Pilha *pilha);
int filaCheia(Fila *fila);
int filaVazia(Fila *fila);
int pilhaCheia(Pilha *pilha);
int pilhaVazia(Pilha *pilha);
void enfileirar(Fila *fila, Peca p);
Peca desenfileirar(Fila *fila);
void empilhar(Pilha *pilha, Peca peca);
Peca desempilhar(Pilha *pilha);
void exibirEstado(Fila *fila, Pilha *pilha);
Peca gerarPeca(int id);
void trocarPecaAtual(Fila *fila, Pilha *pilha);
void trocaMultipla(Fila *fila, Pilha *pilha);

// ---------- FUNÇÃO PRINCIPAL ----------
int main() {
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    srand((unsigned)time(NULL));

    int idAtual = 0;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(idAtual++));
    }

    int opcao;

    do {
        printf("\n===================================\n");
        printf("   ESTADO ATUAL DO JOGO\n");
        printf("===================================\n");
        exibirEstado(&fila, &pilha);

        printf("\nOpcoes:\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para pilha (reservar)\n");
        printf("3 - Usar peca reservada (topo da pilha)\n");
        printf("4 - Trocar peca atual (frente da fila com topo da pilha)\n");
        printf("5 - Trocar 3 primeiras da fila com 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entrada invalida. Tente novamente.\n");
            opcao = -1;
        }

        switch (opcao) {
            case 1: { // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\nPeca jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enfileirar(&fila, gerarPeca(idAtual++)); // mantém fila cheia
                } else {
                    printf("Fila vazia! Nenhuma peca disponivel.\n");
                }
                break;
            }
            case 2: { // Reservar peça
                if (filaVazia(&fila)) {
                    printf("Fila vazia! Nao ha peca para reservar.\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("Pilha cheia! Nao e possivel reservar mais pecas.\n");
                } else {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("\nPeca reservada: [%c %d]\n", reservada.nome, reservada.id);
                    enfileirar(&fila, gerarPeca(idAtual++)); // gera nova peça
                }
                break;
            }
            case 3: { // Usar peça reservada
                if (pilhaVazia(&pilha)) {
                    printf("Nenhuma peca reservada para usar.\n");
                } else {
                    Peca usada = desempilhar(&pilha);
                    printf("\nPeca reservada usada: [%c %d]\n", usada.nome, usada.id);
                }
                break;
            }
            case 4: { // Trocar peça atual
                trocarPecaAtual(&fila, &pilha);
                break;
            }
            case 5: { // Troca múltipla
                trocaMultipla(&fila, &pilha);
                break;
            }
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                if (opcao != -1) printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}

// ---------- IMPLEMENTAÇÕES ----------

// Inicializa fila
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->qtd = 0;
}

// Inicializa pilha
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// Verificações
int filaCheia(Fila *fila) { return fila->qtd == TAM_FILA; }
int filaVazia(Fila *fila) { return fila->qtd == 0; }
int pilhaCheia(Pilha *pilha) { return pilha->topo == TAM_PILHA - 1; }
int pilhaVazia(Pilha *pilha) { return pilha->topo == -1; }

// Enfileirar (lógica circular)
void enfileirar(Fila *fila, Peca p) {
    if (filaCheia(fila)) return;
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->pecas[fila->tras] = p;
    fila->qtd++;
}

// Desenfileirar (retorna peça removida)
Peca desenfileirar(Fila *fila) {
    Peca removida = {'?', -1};
    if (filaVazia(fila)) return removida;
    removida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->qtd--;
    return removida;
}

// Empilhar
void empilhar(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) return;
    pilha->pecas[++pilha->topo] = peca;
}

// Desempilhar
Peca desempilhar(Pilha *pilha) {
    Peca removida = {'?', -1};
    if (pilhaVazia(pilha)) return removida;
    removida = pilha->pecas[pilha->topo--];
    return removida;
}

// Trocar peça atual (fila frente ↔ pilha topo)
void trocarPecaAtual(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("Nao e possivel trocar! Fila ou pilha vazia.\n");
        return;
    }
    int idx = fila->frente;
    Peca temp = fila->pecas[idx];
    fila->pecas[idx] = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = temp;
    printf("\nTroca realizada entre a frente da fila e o topo da pilha!\n");
}

// Troca múltipla (3 da fila ↔ 3 da pilha)
void trocaMultipla(Fila *fila, Pilha *pilha) {
    if (fila->qtd < 3 || pilha->topo < 2) {
        printf("Nao ha pecas suficientes para troca multipla.\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        int idx = (fila->frente + i) % TAM_FILA;
        Peca temp = fila->pecas[idx];
        fila->pecas[idx] = pilha->pecas[pilha->topo - i];
        pilha->pecas[pilha->topo - i] = temp;
    }
    printf("\nAção: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// Exibir estado completo
void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\nFila de pecas\t");
    if (filaVazia(fila)) printf("(vazia)");
    else {
        for (int i = 0; i < fila->qtd; i++) {
            int idx = (fila->frente + i) % TAM_FILA;
            printf("[%c %d] ", fila->pecas[idx].nome, fila->pecas[idx].id);
        }
    }

    printf("\nPilha de reserva (Topo -> Base)\t");
    if (pilhaVazia(pilha)) printf("(vazia)");
    else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
        }
    }
    printf("\n");
}

// Gera nova peça
Peca gerarPeca(int id) {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = id;
    return p;
}


