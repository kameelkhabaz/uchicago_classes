#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strlist.h"

strlist *strlist_new(char *w, strlist *n)
{
    /* Create a new heap-allocated strlist element:
     * - Each element stores a heap-allocated copy of w
     *   - And a pointer to the next strlist n */
    strlist *slst = (strlist *)malloc(sizeof(strlist));
    if (slst == NULL) {
        fprintf(stderr,"strlist_new: allocation error\n");
        exit(1);
    }
    slst->word = strdup(w);
    slst->next = n;
    return slst; 
}

/*
void strlist_free(strlist *lst)
{
    / Free the entire strlist and each word in the list 
    if (lst == NULL) {
        return;
    }
    free(lst->word);
    strlist_free(lst->next);
    free(lst);
}
*/

void strlist_free(strlist *lst)
{
    // iterative free
    strlist *curr = lst;
    while (lst != NULL) {
        lst = curr->next;
        free(curr->word);
        free(curr);
        curr = lst;
    }
}

void strlist_write(FILE *f, strlist *lst)
{
    /*  Write a list to a file (any readable format is acceptable) */
    if (lst == NULL) {
        fprintf(f,"NULL\n");
        return;
    }
    fprintf(f, "%s-> ",lst->word);
    strlist_write(f, lst->next);
}

strlist *strlist_join(strlist *l1, strlist *l2)
{
    if (l1 == NULL) {
        return l2;
    }
    strlist *curr = l1;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = l2;
    return l1;
}

/* Recursive solution 
strlist *strlist_from_file(FILE *f)
{
    * Read words from a file into a strlist
     * - Assume that each line of the file is a separate word
     * - Do not store the newline character (Hint: Use fscanf) *
    
    char word[100] = {0};
    int result = fscanf(f, "%s\n", word);
    if (result == EOF) 
        return NULL;
    // Attach new word to list of rest of file
    strlist *next = strlist_from_file(f);
    return strlist_new(word, next);
}
*/

/*
// Iterative solution
strlist *strlist_from_file(FILE *f) 
{
    char word[100] = {0};
    fscanf(f,"%s\n",word); // scan 1st line
    strlist *lst = strlist_new(word, NULL);

    while (fscanf(f,"%s\n",word) != EOF) {
        strlist *sline = strlist_new(word, NULL);
        strlist_join(lst,sline);
        memset(word,0,100);
    }
    return lst;
}
*/

// Iterative sol without helper
strlist *strlist_from_file(FILE *f)
{
    char buf[128] = {0};
    fscanf(f, "%s\n",buf);
    strlist *lst = strlist_new(buf,NULL);
    memset(buf,0,128);
    strlist *last = lst;
    while (fscanf(f,"%s\n",buf) != EOF) {
        strlist *node = strlist_new(buf,NULL);
        last->next = node;
        memset(buf,0,128);  
        last = last->next;
    }
    return lst;
}

unsigned int strlist_len(strlist *lst)
{   
    /* Compute the length of a strlist */
    if (lst == NULL) {
        return 0;
    }
    return 1 + strlist_len(lst->next);
}

unsigned int strlist_count(int(*test)(char*), strlist *lst)
{
    /* Count the number of words that pass the test */
    if (lst == NULL) {
        return 0;
    }
    return strlist_count(test,lst->next) + ((test)(lst->word) ? 1 : 0);
}

