#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
    for (int i = 0; i < n; ++i) {
        scanf("%lf", x++);
    }
}

void print_vector(double x[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%.4f ", x[i]);
    }
    printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%lf", &A[i][j]);
        }
    }
}

void print_mat(double A[][SIZE], int m, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%.4f ", A[i][j]);
        }
        printf("\n");
    }
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            double a = 0;
            int x = 0;
            while (x < p) {
                a += A[i][x] * B[x][j];
                x++;
            }
            AB[i][j] = a;
        }
    }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (A[i][i] == 0) return NAN;
            double multiply_by = A[j][i] / A[i][i];
            for (int k = 0; k < n; ++k) {
                A[j][k] -= A[i][k] * multiply_by;
            }
        }
    }

    double det = 1;
    for (int i = 0; i < n; ++i) {
        det *= A[i][i];
    }
    return det;
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

int swap_idx(double A[][SIZE], int idx[], int n, int from) {
    int biggest = from;
    for (int k = from + 1; k < n; ++k) {
        if (fabs(A[idx[biggest]][from]) < fabs(A[idx[k]][from])) biggest = k;
    }
    if (from != biggest){
        int tmp = idx[biggest];
        idx[biggest] = idx[from];
        idx[from] = tmp;
        return -1;
    }
    return 1;

}

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
    int idx[n];
    double tmp_array[n];
    for (int i = 0; i < n; ++i) {
        idx[i] = i;
        tmp_array[i] = b[i];
    }

    double det = 1;
    for (int i = 0; i < n - 1; ++i) {
        det *= swap_idx(A, idx, n, i);
        for (int j = i+1; j < n; ++j) {
            if (idx[i] != idx[j]) {
                if (fabs(A[idx[i]][i]) < eps) {
                    det = 0;
                    goto end;
                }
                double multiply_by = A[idx[j]][i] / A[idx[i]][i];
                for (int k = 0; k < n; ++k) A[idx[j]][k] -= A[idx[i]][k] * multiply_by;
                tmp_array[idx[j]] -= tmp_array[idx[i]] * multiply_by;

            }
        }
    }

    // determinant
    for (int i = 0; i < n; ++i) {
        det *= A[idx[i]][i];
    }

    // solving
    for (int i = n - 1; i >= 0; --i) {
        for (int j = n - 1; j > i; --j) {
            tmp_array[idx[i]] -= tmp_array[idx[j]] * A[idx[i]][j];
        }
        tmp_array[idx[i]] /= A[idx[i]][i];
    }

    // set correct order
    for (int i = 0; i < n; ++i) {
        x[i] = tmp_array[idx[i]];
    }
    end:
    return det;
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
    int idx[n];
    double C[n][n];
    for (int i = 0; i < n; ++i) {
        idx[i] = i;
        C[i][i] = 1;
    }

    double det = 1;
    for (int i = 0; i < n; ++i) {
        det *= swap_idx(A, idx, n, i);
        for (int j = 0; j < n; ++j) {
            if (idx[i] != idx[j]) {
                if (fabs(A[idx[i]][i]) < eps) {
                    det = 0;
                    goto end;
                }
                double multiply_by = A[idx[j]][i] / A[idx[i]][i];
                for (int k = 0; k < n; ++k) A[idx[j]][k] -= A[idx[i]][k] * multiply_by;

                for (int k = 0; k < n; ++k) C[idx[j]][k] -= C[idx[i]][k] * multiply_by;

            }
        }
    }

    // divide
    for (int i = 0; i < n; ++i) {
        double divide_by = A[idx[i]][i];
        for (int j = 0; j < n; ++j) {
            C[idx[i]][j] /= divide_by;
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            B[i][j] = C[idx[i]][j];
        }

    }
    // print_mat(A,n,n);

    // determinant
    for (int i = 0; i < n; ++i) {
        det *= A[idx[i]][i];
    }
    end:
    return det;
}

int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;

    int to_do;
    int m, n, p;

    scanf("%d", &to_do);

    switch (to_do) {
        case 1:
            scanf("%d %d %d", &m, &p, &n);
            read_mat(A, m, p);
            read_mat(B, p, n);
            mat_product(A, B, C, m, p, n);
            print_mat(C, m, n);
            break;
        case 2:
            scanf("%d", &n);
            read_mat(A, n, n);
            printf("%.4f\n", gauss_simplified(A, n));
//            print_mat(A, n, n);
            break;
        case 3:
            scanf("%d", &n);
            read_mat(A, n, n);
            read_vector(b, n);
            det = gauss(A, b, x, n, eps);
//            print_mat(A, n, n);
            printf("%.4f\n", det);
            if (det) print_vector(x, n);
            break;
        case 4:
            scanf("%d", &n);
            read_mat(A, n, n);
            det = matrix_inv(A, B, n, eps);
            printf("%.4f\n", det);
            if (det) print_mat(B, n, n);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

