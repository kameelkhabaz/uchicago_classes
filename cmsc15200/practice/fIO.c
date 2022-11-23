#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void getwords(FILE *f)
{
    char buff[256] = {0};
    while (fscanf(f,"%s",buff) != EOF) {
        if (strlen(buff) == 5) {
            printf("%s is 5 letters!\n",buff);
        }
        memset(buff,'\0',256);
    }
}

void getwords2(FILE *f)
{
    char buff[256] = {0};
    char c; 
    int i = 0;
    while ((c = getc(f)) != EOF) {
        if (c == '\n' || c == ' ' || c == '\t') {
            // can just use index
            if (i == 5) {
                printf("%s is 5 letters\n", buff);
            }
            memset(buff,'\0',256);
            i = 0;
        } else {
            buff[i] = c;
            i++;
        }
    }
}

char upper(char c)
{
    if (c >= 'a' && c <= 'z') {
        c = c - 'a' + 'A';
    }
    return c;
}

char lower(char c)
{
    if (c >= 'A' && c <= 'Z') {
        c = c - 'A' + 'a';
    }
    return c;
}

void fdiff(char *f1, char *f2)
{
    // ISSUE IS THAT FSCANF STOP AT FIRST WHITESPACE 
    // SO IT WILL READ TEXT WORD BY WORD NOT LINE BY LINE 
    FILE *fp1 = fopen(f1,"r");
    FILE *fp2 = fopen(f2,"r");
    char buf1[256] = {0};
    char buf2[256] = {0};
    while (fscanf(fp1,"%s",buf1) != EOF) {
        if (fscanf(fp2,"%s",buf2) == EOF) {
            printf("File 2 ended\n");
            printf("File 1 has %s\n",buf1);
            fclose(fp1); fclose(fp2); exit(1);
        }
        if (strcmp(buf1, buf2) != 0) {
            printf("FILE 1 %s\n",buf1);
            printf("FILE 2 %s\n",buf2);
            fclose(fp1); fclose(fp2); exit(1);
        }   
        memset(buf1,'\0',256);
        memset(buf2,'\0',256);
    }
    if (fscanf(fp2,"%s",buf2) != EOF) {
        printf("File 1 ended, file 2 haas\n");
        printf("%s\n",buf2);
        fclose(fp1); fclose(fp2); exit(1);
    }
    printf("Files are same\n");
    fclose(fp1); fclose(fp2);
}

char *readline(FILE *f) 
{
    // must check EOF beforehand
    char buf[256] = {0};
    char c; int i = 0;
    while ((c = getc(f)) != '\n') {
        buf[i++] = c;
    }
    return strdup(buf);
}

void flinediff(char *f1, char *f2)
{
    FILE *fp1 = fopen(f1,"r");
    FILE *fp2 = fopen(f2,"r");
    char c;
    while ((c = getc(fp1)) != EOF) {
        ungetc(c,fp1);
        if ((c = getc(fp2)) == EOF) {
            printf("File 2 ended\n");
            char *s = readline(fp1);
            printf("File 1 has %s\n", s);
            free(s);
            fclose(fp1); fclose(fp2); exit(1);
        } else {
            ungetc(c,fp2);
        }
        char *s1 = readline(fp1);
        char *s2 = readline(fp2);
        if (strcmp(s1,s2) != 0) {
            printf("FILE 1 %s\n",s1);
            printf("FILE 2 %s\n",s2);
            free(s1); free(s2);
            fclose(fp1); fclose(fp2); exit(1);
        }   
        free(s1);
        free(s2);
    }
    if ((c = getc(fp2)) != EOF) {
        printf("File 1 ended, file 2 has not\n");
        ungetc(c,fp2);
        char *s = readline(fp2);
        printf("%s\n",s); 
        free(s);
        fclose(fp1); fclose(fp2); exit(1);
    }
    printf("Files are same\n");
    fclose(fp1); fclose(fp2);
}

    
/*    
int main(int argc, char *argv[])
{
    // ARG IS CHAR *ARGV[] ARRAY OF CHAR *
    if (argc < 2) {
        fprintf(stderr,"too few arguments\n");
        exit(1);
    }
    char c; // MUST DECLARE VAR
    if (strcmp(argv[1],"upper") == 0) {
        while ((c = getc(stdin)) != EOF) {
            putc(upper(c),stdout);
    
    
int main(int argc, char *argv[])
{
    // ARG IS CHAR *ARGV[] ARRAY OF CHAR *
    if (argc < 2) {
        fprintf(stderr,"too few arguments\n");
        exit(1);
    }
    char c; // MUST DECLARE VAR
    if (strcmp(argv[1],"upper") == 0) {
        while ((c = getc(stdin)) != EOF) {
            putc(upper(c),stdout);
        }
    } else if ( strcmp(argv[1],"lower") == 0) {
        while ((c = getc(stdin)) != EOF) {
            putc(lower(c),stdout);
        }
    }
    
    return 0;
} 
*/

int main(int argc, char *argv[])
{
    // comparing 2 files
    if (argc < 3) {
        fprintf(stderr,"too few arguments\n");
        exit(1);
    }
    flinediff(argv[1],argv[2]);
    return 0;
}

