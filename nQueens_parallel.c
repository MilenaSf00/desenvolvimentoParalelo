#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define MAX 100

typedef struct {
    int x, y;
} position;

int N = 0;
int globalCounter = 0; // contador global de soluções

void SolveProblemParallel(int n, int firstCol) {
    int counter1, counter2 = -1, counter3 = -1;
    int localCounter = 0;

    int d[MAX][3] = {0};
    int *stack2 = (int *)malloc(n * sizeof(int));
    position Position1, Position2, Position3;
    position *head1 = (position *)malloc(n * n * sizeof(position));

    // inicia a thread com a primeira rainha em (0, firstCol)
    Position1.x = 0;
    Position1.y = firstCol;
    head1[++counter2] = Position1;

    while (counter2 >= 0) {
        Position1 = head1[counter2--];

        while (counter3 >= 0 && Position1.x <= counter3) {
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

        if (counter3 == n - 1) {
            localCounter++;
            Position2.x = counter3;
            Position2.y = stack2[counter3--];
            d[Position2.y][0] = 0;
            d[Position2.x + Position2.y][1] = 0;
            d[Position2.x - Position2.y + n][2] = 0;
        } else {
            for (counter1 = n - 1; counter1 >= 0; counter1--) {
                if (d[counter1][0] == 0 && d[Position1.x + 1 + counter1][1] == 0 && d[n + Position1.x + 1 - counter1][2] == 0) {
                    Position3.x = Position1.x + 1;
                    Position3.y = counter1;
                    head1[++counter2] = Position3;
                }
            }
        }
    }

    // atualiza contador global com atomic
    #pragma omp atomic
    globalCounter += localCounter;

    free(stack2);
    free(head1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Use: %s size_of_queens [num_threads]\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    int numThreads = (argc == 3) ? atoi(argv[2]) : 4; // default 4 threads

    omp_set_num_threads(numThreads);

    clock_t inicio = clock();

    // Paraleliza pelo laço da primeira linha
    #pragma omp parallel for schedule(dynamic)
    for (int col = 0; col < N; col++) {
        SolveProblemParallel(N, col);
    }

    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("Total de soluções para N=%d: %d\n", N, globalCounter);
    printf("Tempo SolveProblem Parallel = %.3f segundos\n", tempo);

    return 0;
}
