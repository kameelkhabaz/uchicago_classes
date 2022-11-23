#include <stdlib.h>
#include <stdio.h>

int **create_array(unsigned int rows, unsigned int cols) 
{
    // Makes array with random integer values
    int **arr = (int **)malloc(sizeof(int *) * rows);
    if (arr == NULL) {
        fprintf(stderr,"malloc: allocation error\n");
        exit(1);
    }
    // Allocate array for each row
    for (int i = 0; i < rows; i++) {
        int *a = (int *)malloc(sizeof(int) * cols);
        if (a == NULL) {
            fprintf(stderr,"malloc: allocation error\n");
            exit(1);
        }
        for (int j = 0; j < cols; j++) 
            a[j] = rand() % 5;
        arr[i] = a;
    }
    return arr;
}

int **create_triangle(unsigned int rows)
{
    int **arr = (int **)malloc(sizeof(int *) * rows);
    if (arr == NULL) {
        fprintf(stderr,"malloc: allocation error\n");
        exit(1);
    }
    // Allocate array for each row
    for (int i = 0; i < rows; i++) {
        //NOTICE HOW VAR DEF INSIDE 
        int *a = (int *)malloc(sizeof(int) * (rows - i));
        if (a == NULL) {
            fprintf(stderr,"malloc: allocation error\n");
            exit(1);
        }
        for (int j = 0; j < rows - i; j++) 
            a[j] = j + 1;
        arr[i] = a;
    }
    return arr;
}

void free_array(int **matrix, unsigned int rows)
{
    for (int i = 0; i < rows; i++)
        free(matrix[i]);

    free(matrix);
}

void print_array(int **matrix, unsigned int rows, unsigned int cols)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}


void print_triangle(int **matrix, unsigned int rows)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows - i; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
void scale(int **matrix, unsigned int rows, unsigned int cols, int scale_by)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // *(*(a + i) + j)
            matrix[i][j] *= scale_by;
        }
    }   
}

int search(int **matrix, unsigned int rows, unsigned int cols,
           int search_for, int *found_row, int *found_col)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == search_for) {
                *found_row = i;
                *found_col = j;         
                return 1;
            }
        }
    }
    *found_row = -1;
    *found_col = -1;
    return 0;
}

int *sum_rows(int **matrix, unsigned int rows, unsigned int cols)
{
    int *sum_rows = (int *)malloc(sizeof(int) * rows);
    if (sum_rows == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }   

    for (int i = 0; i < rows; i++) {
        int sum = 0;
        for (int j = 0; j < cols; j++) {
           sum += matrix[i][j];
        }
        sum_rows[i] = sum;
    }
    return sum_rows;
} 
    
int main()
{
    int **m2 = create_array(2,3);
    printf("Make array 2 row 3 cols\n");
    print_array(m2,2,3);
    printf("\n");
    scale(m2,2,3,3);
    print_array(m2,2,3);
    printf("\n");


    int **matrix = create_array(3,3);
    print_array(matrix, 3, 3);
    printf("\n");
    scale(matrix,3,3,2);    
    print_array(matrix,3,3);
    int *srows = sum_rows(matrix, 3, 3);
    printf("Sum of rows are %d\n%d\n%d\n",srows[0],srows[1],srows[2]);
    int row, col;
    int found = search(matrix, 3, 3, 2, &row, &col);
    if (found) {
        printf("found %d at (%u, %u)\n", 2, row, col); 
    } else {
        printf("%d was not found\n",2);
    }
    found = search(matrix, 3, 3, 5, &row, &col);
    if (!found) {
        printf("%d was not found\n", 5);
    }
    free_array(matrix,3);
    
    int **triangle = create_triangle(3);
    print_triangle(triangle,3);
    free_array(triangle,3);
    return 0;
}
