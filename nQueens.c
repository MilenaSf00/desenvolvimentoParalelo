/*
 N Queens Program using two Stacks. Stacks are implemented using arrays.
 In Each of the solutions the Coordinates of the N-Queens are given (Row,Col).
 Note that the Rows and Colums are numbered between 1 - N
 */

# include <stdio.h>
# include <stdlib.h>
# include <time.h>

#define MAX 100 //Could this value be a problem for larger size of N?
//#define output 1 //Comment this line to only collect the execution time

typedef struct {
    int x, y;
} position;

int N = 0;

void SolveProblem(int n) {
    int counter1, counter2 = -1, counter3 = -1;
    static int counter = 0;
    int d[MAX][3] = {0};
    int *stack2 = (int *)malloc(n*sizeof(int));;
    position Position1, Position2, Position3;
    position *head1 = (position *)malloc(n*n*sizeof(position));
    for(counter1 = n-1; counter1 >= 0; counter1--){
        Position1.x = 0;
        Position1.y = counter1;
        head1[++counter2] = Position1;
    }
    while(counter2 >= 0){
        Position1 = head1[counter2--];
        while(counter3 >= 0 && Position1.x <= counter3){
            Position2.x = counter3;
            Position2.y = stack2[counter3--];
            d[Position2.y][0] = 0;
            d[Position2.x+Position2.y][1] = 0;
            d[Position2.x-Position2.y+n][2] = 0;
        }
        stack2[++counter3] = Position1.y;
        d[Position1.y][0] = 1;
        d[Position1.x+Position1.y][1] = 1;
        d[Position1.x-Position1.y+n][2] = 1;
        
        if(counter3 == n-1) {
            counter++;
            #ifdef output
                printf("\n SOLUTION # %d:", counter);
                for(counter1 = 0; counter1 <= counter3; counter1++)
                    printf("(%d,%d) ", counter1+1, stack2[counter1]+1);
            #endif
            Position2.x = counter3;
            Position2.y = stack2[counter3--];
            d[Position2.y][0] = 0;
            d[Position2.x+Position2.y][1] = 0;
            d[Position2.x-Position2.y+n][2] = 0;
        } else {
            for(counter1 = n-1; counter1 >= 0; counter1--) {
                if(d[counter1][0] == 0 && d[Position1.x+1+counter1][1] == 0 && d[n+Position1.x+1-counter1][2]==0) {
                    Position3.x = Position1.x+1;
                    Position3.y = counter1;
                    head1[++counter2] = Position3;
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    if(argc == 2)
        N = atoi(argv[1]);
    else {
        printf("Please! Use: %s size_of_queens\n", argv[0]);
        return 1;
    }
    SolveProblem(N);
    return 0;
}