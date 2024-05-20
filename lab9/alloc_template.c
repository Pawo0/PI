#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

// 1
////////////////////////////////////////////////////////////

int get(int cols, int row, int col, const int *A) {
}

void set(int cols, int row, int col, int *A, int value) {
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            int sum = 0;
            for (int k = 0; k < colsA; ++k) {
                sum += (A[i * colsA + k] * B[k * colsB + j]);
            }
            AB[i * colsB + j] = sum;
        }
    }
}

void read_mat(int rows, int cols, int *t) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            scanf("%d", t + i * cols + j);
        }
    }
}

void print_mat(int rows, int cols, int *t) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", t[i * cols + j]);
        }
        printf("\n");
    }
}

// 2
/////////////////////////////////////////////////////////////

int read_ints_from_line(char *c_buf, int *int_buf) {
    int digit;
    int i = 0, j = 0;
    while ((digit = getc(stdin)) != EOF && digit != '\n') {
        if (digit == ' ') {
            int_buf[j] = atoi(c_buf);
            j++;
//            printf("%d ", atoi(c_buf));

            memset(c_buf, 0, BUF_SIZE);
            i = 0;
        } else {
            c_buf[i] = (char) digit;
            i++;
        }
    }
    if (i > 0) {
        int_buf[j] = atoi(c_buf);
        j++;
//        printf("%d ", atoi(c_buf));
        memset(c_buf, 0, BUF_SIZE);
    }
    int_buf[j++] = INT_MAX;

//    printf("\n");
    return j;
}

int read_int_lines_cont(int *ptr_array[]) {
    int i = 0;
    char c_buffer[BUF_SIZE] = {0};
    int i_buffer[BUF_SIZE] = {0};
    while (!feof(stdin)) {
        int len = read_ints_from_line(c_buffer, i_buffer);
        ptr_array[i] = malloc(len * sizeof(int));
        memcpy(ptr_array[i], i_buffer, len * sizeof(int));
        i++;
    }
    memset(c_buffer, 0, BUF_SIZE);
    memset(i_buffer, 0, BUF_SIZE * sizeof(int));
    return i;
}

void write_int_line_cont(int *ptr_array[], int n) {
    int i = 0;
    while (ptr_array[n][i] != INT_MAX) {
        printf("%d ", ptr_array[n][i]);
        i++;
    }
}

// 3
///////////////////////////////////////////////////////////

int read_char_lines(char *array[]) {
    char buffer[BUF_SIZE] = {0};
    int i = 0, j = 0;
    int digit;
    while ((digit = getc(stdin)) != EOF) {
        if (digit != '\n') {
            buffer[i] = (char) digit;
            i++;
        } else {
            buffer[i] = '\0';
            i++;
            array[j] = malloc(i * sizeof(char));
            memcpy(array[j], buffer, i);
            j++;
            i = 0;
        }
    }
    if (i > 0) {
        buffer[i] = '\0';
        i++;
        array[j] = malloc(i * sizeof(char));
        memcpy(array[j], buffer, i);
        j++;
    }
    return j;
}

void write_char_line(char *array[], int n) {
    printf("%s", array[n]);
}

void delete_lines(char *array[], int n) {
    for (int i = 0; i < n; ++i) {
        free(array[i]);
    }
}

// 4
////////////////////////////////////////////////

typedef struct {
    int *values;
    int len;
    double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
    char c_buffer[BUF_SIZE] = {0};
    int i_buffer[BUF_SIZE] = {0};
    int cnt = 0;
    line_type x;
    while (!feof(stdin)) {
        x.len = read_ints_from_line(c_buffer, i_buffer) - 1;
        x.values = malloc(x.len * sizeof(int));
        memcpy(x.values, i_buffer, x.len * sizeof(int));
        int sum = 0;
        for (int i = 0; i < x.len; ++i) {
            sum += i_buffer[i];
        }
        x.average = (double) sum / x.len;
        lines_array[cnt] = x;
        cnt++;

        memset(c_buffer, 0, BUF_SIZE);
        memset(i_buffer, 0, BUF_SIZE * sizeof(int));
//        for (int i = 0; i < x.len; ++i) {
//            printf("%d ",x.values[i]);
//        }
//        printf("\n%d %f\n\n", x.len, x.average);

    }
    return cnt;
}

void write_int_line(line_type lines_array[], int n) {
    line_type x = lines_array[n];
    for (int i = 0; i < x.len; ++i) {
        printf("%d ", x.values[i]);
    }
    printf("\n%.2f", x.average);
}

void delete_int_lines(line_type array[], int line_count) {
}

int cmp(const void *a, const void *b) {
    line_type *x = (line_type *) a;
    line_type *y = (line_type *) b;
    if (x->average < y->average) return -1;
    if (x->average > y->average) return 1;
    else return 0;
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array, line_count, sizeof(line_type), cmp);
}

// 5
///////////////////////////////////////////////////////////

typedef struct {
    int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    for (int i = 0; i < n_triplets; ++i) {
        scanf("%d", &triplet_array[i].r);
        scanf("%d", &triplet_array[i].c);
        scanf("%d", &triplet_array[i].v);
    }
    return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
    triplet *x = (triplet *) t1;
    triplet *y = (triplet *) t2;
    if (x->r < y->r) return -1;
    if (x->r > y->r) return 1;
    if (x->c < y->c) return -1;
    if (x->c > y->c) return 1;
    return 0;

}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);
    memset(R, 0, (rows + 1) * sizeof(triplet));
    for (int i = 0; i < n_triplets; i++) {
        C[i] = triplet_array[i].c;
        V[i] = triplet_array[i].v;
        for (int k = triplet_array[i].r + 1; k < rows + 1; k++)
            R[k] += 1;
    }
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    for (int i = 0; i < rows; i++) {
        y[i] = 0;
        for (int j = R[i]; j <= R[i + 1] - 1; j++) {
            y[i] += V[j] * x[C[j]];
        }
    }
}

void read_vector(int *v, int n) {
    for(int i = 0; i < n; i++){
        scanf("%i", &v[i]);
    }
}

void write_vector(int *v, int n) {
    for(int i = 0; i < n; i++)
        printf("%i ", v[i]);

    printf("\n");
}

// auxiliary
////////////////////////////////////////////////////////////

int read_int(void) {
    char c_buf[BUF_SIZE];
    fgets(c_buf, BUF_SIZE, stdin);
    return (int) strtol(c_buf, NULL, 10);
}

int main(void) {
    int to_do = read_int();

    int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
    int n, lines_counter, rowsA, colsA, rowsB, colsB;
    int rows, cols, n_triplets;
    char *char_lines_array[TAB_SIZE] = {NULL};
    int continuous_array[TAB_SIZE];
    int *ptr_array[TAB_SIZE];
    triplet triplet_array[TAB_SIZE];
    int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
    int x[TAB_SIZE], y[TAB_SIZE];
    line_type int_lines_array[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d", &rowsA, &colsA);
            read_mat(rowsA, colsA, A);
            scanf("%d %d", &rowsB, &colsB);
            read_mat(rowsB, colsB, B);
            prod_mat(rowsA, colsA, colsB, A, B, AB);
            print_mat(rowsA, colsB, AB);
            break;
        case 2:
            n = read_int() - 1; // we count from 1 :)
            ptr_array[0] = continuous_array;
            read_int_lines_cont(ptr_array);
            write_int_line_cont(ptr_array, n);
            break;
        case 3:
            n = read_int() - 1;
            int cnt = read_char_lines(char_lines_array);
            write_char_line(char_lines_array, n);
            delete_lines(char_lines_array, cnt);
            break;
        case 4:
            n = read_int() - 1;
            lines_counter = read_int_lines(int_lines_array);
            sort_by_average(int_lines_array, lines_counter);
            write_int_line(int_lines_array, n);
            delete_int_lines(int_lines_array, lines_counter);
            break;
        case 5:
            scanf("%d %d %d", &rows, &cols, &n_triplets);
            n_triplets = read_sparse(triplet_array, n_triplets);
            read_vector(x, cols);
            make_CSR(triplet_array, n_triplets, rows, V, C, R);
            multiply_by_vector(rows, V, C, R, x, y);
            write_vector(V, n_triplets);
            write_vector(C, n_triplets);
            write_vector(R, rows + 1);
            write_vector(y, rows);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}

