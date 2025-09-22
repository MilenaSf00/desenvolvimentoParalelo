#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int tamanhos[] = {16, 18, 20};
    int tamanhoNumero = sizeof(tamanhos) / sizeof(tamanhos[0]);
    int repeticoes = 5;  

    for (int i = 0; i < tamanhoNumero; i++) {
        int N = tamanhos[i];
        double somaTempos = 0.0;

        printf("**** Execução N = %d **** \n", N);

        for (int j = 0; j < repeticoes; j++) {
            char comando[100];
            clock_t inicio, fim;

           
            sprintf(comando, "..\\nQueens %d > NUL", N);

         

            inicio = clock();
            system(comando);
            fim = clock();

            double tempoSegundo = (double)(fim - inicio) / CLOCKS_PER_SEC;
            somaTempos += tempoSegundo;

            printf("Execução %d: %.3f segundos\n", j+1, tempoSegundo);
        }

        double media = somaTempos / repeticoes;
        printf("Média para N=%d: %.3f segundos\n\n", N, media);
    }

    return 0;
}
