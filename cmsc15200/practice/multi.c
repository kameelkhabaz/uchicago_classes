#include <stdlib.h>
#include <stdio.h>

int ***make_3D(int size) 
{
    // make 3D array
    // pointer to 2D arrays
    // pointer to pointers to 1D arrays
    // pointer to pointers to pointers to ints
    int ***X = (int ***)malloc(sizeof(int **) * size);
    if (X == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        X[i] = (int **)malloc(sizeof(int *) * size);
        if (X[i] == NULL) {
            fprintf(stderr,"malloc failed\n");
            exit(1);
        }
        for (int j = 0; j < size; j++) {
            X[i][j] = (int *)malloc(sizeof(int) * size);
            if (X[i][j] == NULL) {
                fprintf(stderr,"malloc failed\n");
                 exit(1);
            }
            for (int k = 0; k < size; k++) {
                X[i][j][k] = 0; // MUST INITIALIZE EACH ELEMENT
            }
        }
    }
    return X;
}

void free_3D(int ***X, int size)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            free(X[i][j]);
        }
        free(X[i]);
    }
    free(X);
}

int main()
{
    int ***X = make_3D(3);
    free_3D(X,3);
    return 0;
}
            
        
