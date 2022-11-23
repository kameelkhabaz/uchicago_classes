#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    // Make string multidimensional array
    char **names = (char **)malloc(5 * sizeof(char *));
    if (names == NULL) {
        fprintf(stderr,"malloc faild\n");
        exit(1);
    }
    for (int i = 0; i < 5; i++) {
        // allocate enough space for 9 letter word 
        names[i] = (char *)malloc(10);
        if (names[i] == NULL) {
            fprintf(stderr,"malloc failed\n");
            exit(1);
        }
        for (int j = 0; j < 10; j++) {
            names[i][j] = '\0';
        }
    }
    strcpy(names[0],"Hello");
    strcpy(names[1],"Yes");
    strcpy(names[2],"string");
    strcpy(names[3],"world");
    strcpy(names[4],"computers");
    
    for (int i = 0; i < 5; i++) {
        printf("Element %d in names is %s\n",i,names[i]);
    }

    // Now free it
    for (int i = 0; i < 5; i++) {
        free(names[i]);
    }
    free(names);
    char *names2[] = {"Hello","world","I","said","YES"};

    for (int i = 0; i < 5; i++) {
        printf("Element %d in names2 is %s\n",i,names2[i]);
    }

    return 0;
}
