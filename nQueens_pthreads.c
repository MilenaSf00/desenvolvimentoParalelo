#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX 100

typedef struct {
    int x, y;
} position;

int N = 0;
int contadorGlobal = 0;          
pthread_mutex_t counterMutex;   

typedef struct {
    int primeiraColuna; 
} ThreadData;

void SolveProblemThread(int n, int primeiraColuna) {
    int counter1, counter2 = -1, counter3 = -1;
    int localCounter = 0;

    int d[MAX][3] = {0};
    int *stack2 = (int *)malloc(n * sizeof(int));
    position Position1, Position2, Position3;
    position *head1 = (position *)malloc(n * n * sizeof(position));

    // Inicializa a thread com a primeira rainha em (0, primeiraColuna)
    Position1.x = 0;
    Position1.y = primeiraColuna;
    head1[++counter2] = Position1;

    while(counter2 >= 0) {
        Position1 = head1[counter2--];

        while(counter3 >= 0 && Position1.x <= counter3) {
            Position2.x = counter3;
            Position2.y = stack2[counter3--];
            d[Position2.y][0] = 0;
            d[Position2.x + Position2.y][1] = 0;
            d[Position2.x - Position2.y + n][2] = 0;
        }

        stack2[++counter3] = Position1.y;
        d[Position1.y][0] = 1;
        d[Position1.x + Position1.y][1] = 1;
        d[Position1.x - Position1.y + n][2] = 1;

        if(counter3 == n - 1) {
            localCounter++;
            Position2.x = counter3;
            Position2.y = stack2[counter3--];
            d[Position2.y][0] = 0;
            d[Position2.x + Position2.y][1] = 0;
            d[Position2.x - Position2.y + n][2] = 0;
        } else {
            for(counter1 = n - 1; counter1 >= 0; counter1--) {
                if(d[counter1][0] == 0 && d[Position1.x + 1 + counter1][1] == 0 && d[n + Position1.x + 1 - counter1][2] == 0) {
                    Position3.x = Position1.x + 1;
                    Position3.y = counter1;
                    head1[++counter2] = Position3;
                }
            }
        }
    }

    // Atualiza contador global protegido por mutex
    pthread_mutex_lock(&counterMutex);
    contadorGlobal += localCounter;
    pthread_mutex_unlock(&counterMutex);

    free(stack2);
    free(head1);
}

// Função que será passada para pthread_create
void* threadFuncao(void* arg) {
    ThreadData *data = (ThreadData*) arg;
    SolveProblemThread(N, data->primeiraColuna);
    free(data);
    return NULL;
}

int main(int argc, char **argv) {
    if(argc != 3) {
        printf("Use: %s size_of_queens num_threads\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    int numThreads = atoi(argv[2]);

    pthread_t *threads = malloc(sizeof(pthread_t) * numThreads);
    pthread_mutex_init(&counterMutex, NULL);

    clock_t inicio = clock();

    for(int i = 0; i < numThreads; i++) {
        ThreadData *data = malloc(sizeof(ThreadData));
        data->primeiraColuna = i;  
        pthread_create(&threads[i], NULL, threadFuncao, data);
    }

    for(int i = 0; i < numThreads; i++)
        pthread_join(threads[i], NULL);

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("Total de soluções para N=%d: %d\n", N, contadorGlobal);
    printf("Tempo SolveProblem Parallel (Pthreads) = %.3f segundos\n", tempo);

    pthread_mutex_destroy(&counterMutex);
    free(threads);
    return 0;
}
