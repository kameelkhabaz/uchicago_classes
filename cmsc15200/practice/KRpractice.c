#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void popl(FILE *f)
{
    // Exercise 1-12 KR
    char buf[256] = {0};
    char c;
    int wrd_idx = 0;
    while ( (c = getc(f)) != EOF) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (wrd_idx != 0) {
                // finished word
                printf("%s\n",buf);
                memset(buf,'\0',256);
                wrd_idx = 0;
            } // if wrd_idx == 0 don't do anything
        } else {
            buf[wrd_idx++] = c; // set equal, then increment wrd idx
        }
    }
}

int common_word_helper(char *w, char **cwords,int lowi, int highi) 
{
    if (lowi > highi) { //DONT BE AN IDIOT
        return 0;
    }
    int midi = (lowi + highi) / 2;
    int cmp = strcmp(w,cwords[midi]);
    if (cmp == 0) {
        return 1;
    } else if (cmp < 0) {
        return common_word_helper(w,cwords,lowi,midi - 1);
    } else {
        return common_word_helper(w, cwords, midi + 1, highi);
    }
}

int common_word(char *w)
{
    // part of Exerise 6-3
    if (w == NULL) {
        fprintf(stderr,"null string\n");
        exit(1);
    }
    char *cwords[] = {"a","an","and","the","to","two"};
    return common_word_helper(w, cwords,0,5);
}

int main()
{
    popl(stdin);
    printf("a is a common word: %d\n",common_word("a"));    
    printf("an is a common word: %d\n",common_word("an"));    
    printf("and is a common word: %d\n",common_word("and"));    
    printf("the is a common word: %d\n",common_word("the"));    
    printf("to is a common word: %d\n",common_word("to"));    
    printf("two is a common word: %d\n",common_word("two"));    
    printf("blob is a common word: %d\n",common_word("blob"));    
    printf("y is a common word: %d\n",common_word("y"));    
    printf("b is a common word: %d\n",common_word("b"));    
    return 0;
}

