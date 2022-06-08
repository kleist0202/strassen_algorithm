#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


int** zero_matrix(const int n);
int** random_matrix(const int n);
void print_matrix(int** A, const int n);
int** add_matrices(int** A, int** B, const int n);
int** sub_matrices(int** A, int** B, const int n);
int** mul_matrices(int** A, int** B,  const int n);
void free_2d_array(int** array);
bool are_equal(int** A, int** B, const int n);
int** strassen(int** A, int** B, const int n);

// #define TEST_EQUALITY

int main(){

    srand(time(NULL));

    int N;
    FILE* f;

    printf("Enter size of matrices A and B: ");

    scanf("%i", &N);

    if ((N & (N - 1)) != 0) {
        printf("Size of matrix must be number which is a power of 2\n");
        exit(0);
    }

    printf("A");
    int** A = random_matrix(N);
    print_matrix(A, N);

    printf("\n");

    printf("B");
    int** B = random_matrix(N);
    print_matrix(B, N);

    printf("\n");

    clock_t start = clock();
    int** C = strassen(A, B, N);
    clock_t end = clock();

    printf("C");
    print_matrix(C, N);
    printf("\n");

    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("Time needed to calculate the product of the matrices a and b is: %f\n", seconds);
    
    int** D = mul_matrices(A, B, N);

    #ifdef TEST_EQUALITY
    if (are_equal(C, D, N))
        printf("Running test: Algorithm works correctly.\n");
    else
        printf("Running test: Algorithm doesn't work properly!\n");
    #endif /* TEST_EQUALITY */

    // free matrices
    free_2d_array(A);
    free_2d_array(B);
    free_2d_array(C);
    free_2d_array(D);

    // writing result to file
    f = fopen("./data/results", "a");

    if(f == NULL) {
        printf("'results' file can't be opened\n");
        exit(1);
    }

    fprintf(f, "%i,%f\n", N, seconds);
    fclose(f);

    return 0;
}

int** zero_matrix(const int n){
    int* values = (int*)calloc(n*n, sizeof(int));

    int** rows = (int**)malloc(n*sizeof(int*));

    for (int i = 0; i < n; i++)
        rows[i] = values + i*n; 

    return rows;
}

int** random_matrix(const int n){
    int* values = (int*)calloc(n*n, sizeof(int));

    for (int i = 0; i < n*n; i++)
        values[i] = rand() % 50;
 
    int** rows = (int**)malloc(n*sizeof(int*));

    for (int i = 0; i < n; i++)
        rows[i] = values + i*n; 

    return rows;
}

void print_matrix(int** A, const int n){
    for(int i = 0; i < n; i++){
        
        printf("\n");

        for(int j = 0; j < n; j++){
            printf("%i ", A[i][j]);
        }
    }
    printf("\n");
}

int** add_matrices(int** A, int** B, const int n){
    int** C = zero_matrix(n);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            C[i][j] = A[i][j] + B[i][j];
        } 
    }
    return C;
}

int** sub_matrices(int** A, int** B, const int n){
    int** C = zero_matrix(n);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            C[i][j] = A[i][j] - B[i][j];
        } 
    }
    return C;
}

int** mul_matrices(int** A, int** B,  const int n){
    int** C = zero_matrix(n);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

bool are_equal(int** A, int** B, const int n){
    for (int i=0; i < n; i++)
        for (int j=0; j < n; j++)
            if (A[i][j] != B[i][j])
                return false;
    return true;
}

void free_2d_array(int** array){
    free(*array);
    free(array);
}

int** strassen(int** A, int** B, const int n){

    int** C = zero_matrix(n);

    if (n == 2){
        C[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
        C[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];
        C[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
        C[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];
    }
    else {
        int m = n / 2;

        int** A11 = zero_matrix(m);
        int** A12 = zero_matrix(m);
        int** A21 = zero_matrix(m);
        int** A22 = zero_matrix(m);

        int** B11 = zero_matrix(m);
        int** B12 = zero_matrix(m);
        int** B21 = zero_matrix(m);
        int** B22 = zero_matrix(m);

        // dzielenie macierzy A i B na cztery podmacierze
        for (int i = 0; i < m; i++){
            for (int j = 0; j < m; j++){
                A11[i][j] = A[i][j];
                B11[i][j] = B[i][j];
            }
        }
        for (int i = 0; i < m; i++){
            for (int j = m; j < n; j++){
                A12[i][j-m] = A[i][j];
                B12[i][j-m] = B[i][j];
            }
        }
        for (int i = m; i < n; i++){
            for (int j = 0; j < m; j++){
                A21[i-m][j] = A[i][j];
                B21[i-m][j] = B[i][j];
            }
        }
        for (int i = m; i < n; i++){
            for (int j = m; j < n; j++){
                A22[i-m][j-m] = A[i][j];
                B22[i-m][j-m] = B[i][j];
            }
        }

        //obliczanie macierzy M1 - M7
        int** S1 = add_matrices(A11, A22, m);  // A11 + A22
        int** S2 = add_matrices(B11, B22, m);  // B11 + B22
        int** S3 = add_matrices(A21, A22, m);  // A21 + A22
        int** S4 = sub_matrices(B12, B22, m);  // B12 - B22
        int** S5 = sub_matrices(B21, B11, m);  // B21 - B11
        int** S6 = add_matrices(A11, A12, m);  // A11 + A12
        int** S7 = sub_matrices(A21, A11, m);  // A21 - A11
        int** S8 = add_matrices(B11, B12, m);  // B11 + B12
        int** S9 = sub_matrices(A12, A22, m);  // A12 - A22
        int** S10 = add_matrices(B21, B22, m); // B21 + B22

        int** M1 = strassen(S1, S2, m);        // (A11 + A22) * (B11 + B22)
        int** M2 = strassen(S3, B11, m);       // (A21 + A22) * B11
        int** M3 = strassen(A11, S4, m);       //  A11 * (B12 - B22)
        int** M4 = strassen(A22, S5, m);       //  A22 * (B21 - B11)
        int** M5 = strassen(S6, B22, m);       // (A11 * A12) * B22
        int** M6 = strassen(S7, S8, m);        // (A21 - A11) * (B11 + B12)
        int** M7 = strassen(S9, S10, m);       // (A12 - A22) * (B21 + B22)

        //obliczanie macierzy Cij
        int** S11 = add_matrices(M1, M4, m);   // M1 + M4
        int** S12 = sub_matrices(S11, M5, m);  // (M1 + M4) - M5
        int** S13 = sub_matrices(M1, M2, m);   // M1 - M2
        int** S14 = add_matrices(S13, M3, m);  // (M1 - M2) + M3

        int** C11 = add_matrices(S12, M7, m);  // M1 + M4 - M5 + M7
        int** C12 = add_matrices(M3, M5, m);   // M3 + M5
        int** C21 = add_matrices(M2, M4, m);   // M2 + M4
        int** C22 = add_matrices(S14, M6, m);  // M1 - M2 + M3 + M6

        //skladanie podmacierzy Cij w jedna macierz C
        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++)
                C[i][j] = C11[i][j];
        for (int i = 0; i < m; i++)
            for (int j = m; j < n; j++)
                C[i][j] = C12[i][j-m];
        for (int i = m; i < n; i++)
            for (int j = 0; j < m; j++)
                C[i][j] = C21[i-m][j];
        for (int i = m; i < n; i++)
            for (int j = m; j < n; j++)
                C[i][j] = C22[i-m][j-m];

        //zwalnianie pamieci
        free_2d_array(S1);
        free_2d_array(S2);
        free_2d_array(S3);
        free_2d_array(S4);
        free_2d_array(S5);
        free_2d_array(S6);
        free_2d_array(S7);
        free_2d_array(S8);
        free_2d_array(S9);
        free_2d_array(S10);
         
        free_2d_array(S11);
        free_2d_array(S12);
        free_2d_array(S13);
        free_2d_array(S14);

        free_2d_array(M1);
        free_2d_array(M2);
        free_2d_array(M3);
        free_2d_array(M4);
        free_2d_array(M5);
        free_2d_array(M6);
        free_2d_array(M7);

        free_2d_array(A11);
        free_2d_array(A12);
        free_2d_array(A21);
        free_2d_array(A22);
        
        free_2d_array(B11);
        free_2d_array(B12);
        free_2d_array(B21);
        free_2d_array(B22);

        free_2d_array(C11);
        free_2d_array(C12);
        free_2d_array(C21);
        free_2d_array(C22);

    }

    return C; 

}
