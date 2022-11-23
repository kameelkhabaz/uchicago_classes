#include <stdio.h>
#include <ctype.h>

int main()
{
    int c = getc(stdin);
    int i = 0;
    // read byte by byte from stdin
    while (c != EOF) {
        if (isgraph(c)) 
            fprintf(stdout,"%3d\t%3d\t%c\n",i,c,c);
        else
            fprintf(stdout,"%3d\t%3d   ...\n",i,c);
        c = getc(stdin);
        ++i;
    }
    return 0;
}

