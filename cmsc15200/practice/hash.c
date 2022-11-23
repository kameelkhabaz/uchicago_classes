#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct slist slist;
typedef struct hash_table htbl;

struct slist {
    char *first;
    slist *rest;
};

struct hash_table {
    unsigned long int (*hash)(char *);
    unsigned int n_buckets;
    slist **buckets;
};


slist *slist_new(char *s, slist *rest)
{
    slist *lst = (slist *)malloc(sizeof(slist));
    if (lst == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    lst->first = strdup(s); // makes copy of string
    lst->rest = rest; // points to rest
    return lst;
}

void slist_free(slist *lst)
{
    while(lst != NULL) {
        slist *next = lst->rest;
        free(lst->first);
        free(lst);
        lst = next;
    }
}

int slist_len(slist *lst) 
{
    int len = 0;
    while (lst != NULL) {
        len++;
        lst = lst->rest;
    }
    return len;
}

void slist_show(slist *lst)
{
    while (lst != NULL) {
        printf("%s: ",lst->first);
        lst = lst->rest;
    }
    printf("(nil)\n");
}

htbl *htbl_new(unsigned long int(*hash)(char *), unsigned int n_buckets)
{
    slist **buckets = (slist **)malloc(sizeof(slist *) * n_buckets);
    if (buckets == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    for (int i = 0; i < n_buckets; i++) {
        buckets[i] = NULL; //MUST INITIALIZE NULL POINTER
    }
    htbl *tbl = (htbl *)malloc(sizeof(htbl));
    if (tbl == NULL) {
        fprintf(stderr,"malloc failed\n"); 
        exit(1);
    }
    tbl->hash = hash;
    tbl->n_buckets = n_buckets;
    tbl->buckets = buckets;
    return tbl; //RETURN CORRECT THING 
}

void htbl_free(htbl *t)
{
    for (int i = 0; i < t->n_buckets;i++) {
        slist_free(t->buckets[i]);
    }
    free(t->buckets);
    free(t);
}

void htbl_insert(htbl *t, char *s)
{
    // MUST CHECK NULL
    if (t == NULL || s == NULL) {
        fprintf(stderr,"null input");
        exit(1);
    }
    unsigned long int i = t->hash(s);
    i = i % t->n_buckets;
    slist *lst = slist_new(s,NULL);
    if (t->buckets[i] == NULL) {
        t->buckets[i] = lst;
    } else {
        slist *curr = t->buckets[i];
        while (curr->rest != NULL) {
            curr = curr->rest;
        }
        curr->rest = lst;
    }
	IT IS MORE EFFICIENT TO INSERT AT THE BEGINNING OF THE LIST
	t->buckets[I] = slist_new(s,t->buckets[I]);
}
int htbl_contains(htbl *t, char *s)
{
    // MUST CHECK NULL
    if (t == NULL || s == NULL) {
        fprintf(stderr,"null input");
        exit(1);
    }
    unsigned long int i = t->hash(s);
    i = i % t->n_buckets;
    slist *curr = t->buckets[i];
    while (curr != NULL) {
        if (strcmp(curr->first,s) == 0) {
            return 1;
        }
        curr = curr->rest;
    }
    return 0;
}
void htbl_show(FILE *f, htbl *t, int show_string)
{
    if (t == NULL) {
        fprintf(stderr,"NULL table \n");
        exit(1);
    }
    for (int i = 0; i < t->n_buckets; i++) {
        fprintf(f, "%d: ", i);
        if (show_string) {
            slist_show(t->buckets[i]);
            putc('\n',f);
        } else {
            fprintf(f, " (%d items)\n",slist_len(t->buckets[i]));
        }
    }
}

unsigned long int hash(char *s)
{
    int len = strlen(s);
    unsigned long int res = s[0];
    for (int i = 1; i < len; i++) {
        res += s[i] * 7 + 3 * i;
    }
    fprintf(stderr,"Hash # for %s is %lu\n",s,res);
    return res;
}

void delete(htbl *t, char *s)
{   
    if (t == NULL) {
        fprintf(stderr,"NULL table \n");
        exit(1);
    }
    unsigned long int i = t->hash(s);
    i = i % t->n_buckets;
    slist *curr = t->buckets[i];
    slist *prev = NULL;
    while (curr != NULL) {
        if (strcmp(curr->first, s) == 0) {
            if (prev == NULL) {
                // removing first item in list 
                t->buckets[i] = curr->rest;
            } else {
                // removing from middle of list
                prev->rest = curr->rest;
            }
            curr->rest = NULL;
            slist_free(curr); // free current node and string,
            // but do not free rest of list
            return; // end function once deleted item
        } 
        prev = curr;
        curr = curr->rest;
    }
    // item not in list or list is nuull
    fprintf(stderr,"%s not found in hash table\n",s);
    return; // here just return
}

int main()
{
    htbl *tbl = htbl_new(&hash,21);
    htbl_insert(tbl,"test");
    htbl_insert(tbl,"apple");
    htbl_insert(tbl,"banana");
    htbl_insert(tbl,"stuff");
    htbl_insert(tbl,"yes");
    htbl_insert(tbl,"kameel");
    htbl_insert(tbl,"khabaz");
    htbl_insert(tbl,"school");
    htbl_insert(tbl,"teacher");
    htbl_insert(tbl,"food");
    htbl_insert(tbl,"break");
    htbl_insert(tbl,"work");
    htbl_insert(tbl,"computer");
    htbl_insert(tbl,"science");
    htbl_insert(tbl,"bob");
    htbl_insert(tbl,"ancient");
    htbl_insert(tbl,"empire");
    htbl_insert(tbl,"dynamics");
    htbl_insert(tbl,"sort");
    htbl_insert(tbl,"done");
    htbl_insert(tbl,"home");
    htbl_insert(tbl,"physics");
    htbl_insert(tbl,"reading");
    
    printf("Table contains test: %d\n",htbl_contains(tbl,"test"));
    printf("Table contains tes: %d\n",htbl_contains(tbl,"tes"));
    printf("Table contains kameel: %d\n",htbl_contains(tbl,"kameel"));
    htbl_show(stdout,tbl,1);
    htbl_free(tbl);
    return 0;
}
