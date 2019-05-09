#include <stdio.h>
#include <time.h>
#include <unistd.h>

void doCalculation(int n, int k);
long long Rcombination(int n, int k);
long long DPcombination(int n, int k);


int main(int argc, char * argv[]){
    int n = 0;
    int k = 0;
    int mode = 1;
    
    printf("*********MODE SELECT*********");
    do {
        if (mode < 1 || mode > 2)
            printf("\nThere is no such mode");
        printf("\n1. Sample mode (show output with sample input)\n");
        printf("2. User mode (show output with user typed input)\n");

        printf("Please select the mode you want to execute(1 or 2) : "); scanf("%d", &mode);
    } while (mode < 1 || mode > 2);

    //get n, k from user

    if (mode != 1) {
        do {
            if (k > n)
                printf("\nn should be bigger than k\n");
            printf("Type n: "); scanf("%d", &n);
            printf("Type k: "); scanf("%d", &k);
        } while (k > n);

        doCalculation(n, k);
    }
    else {
        int example[4][4] = {{20, 10}, {30, 15}, {32, 16}, {33, 17}};
        
        for (int num = 0; num < 4; num++){
            sleep(1);
            printf("When n : %d, k : %d\n", example[num][0], example[num][1]);
            doCalculation(example[num][0], example[num][1]);
        }
            
    }

    return 0;
}

void doCalculation(int n, int k){
    long long result1 = 0, result2 = 0;
    clock_t start, end;

    //recursive(divide & conquer) case
    start = clock();
    result1 = Rcombination(n, k);
    end = clock();
    
    printf("  - By recursive function(Divide and Conquer)\n\tAnswer : %lld\n\tTime taken : %f sec\n\n", result1, (float) (end - start)/CLOCKS_PER_SEC);


    //dynamic programming case
    start = clock();
    result2 = DPcombination(n, k);
    end = clock();
    
    printf("  - By dynamic programming\n\tAnswer : %lld\n\tTime taken : %f sec\n\n", result2, (float) (end - start)/CLOCKS_PER_SEC);
}


long long Rcombination(int n, int k){
    if (n == k || k == 0)
        return 1;
    else
        return Rcombination(n-1, k-1) + Rcombination(n-1, k);
}


long long DPcombination(int n, int k){
    long long result = 0;
    long long arr[n][n];

    for (int i = 0; i < n; i ++){
        for (int j = 0; j < n; j++){
            if (j > i)              //it indicates when k is larger than n (= exceptional case)
                continue;

            if (j == 0 || i == j)   //nC0 or nCn case
                arr[i][j] = 1;
            else                    //nCk = n-1Ck-1 + n-1Ck
                arr[i][j] = arr[i - 1][j - 1]  + arr[i - 1][j];
        }
    }

    result = arr[n - 1][k - 1] + arr[n - 1][k];
    return result;
}