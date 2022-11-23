#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ilist ilist;
struct ilist {
    int first;
    ilist *rest;
};

typedef struct lstlsts lstlsts;
struct lstlsts {
    int size;
    ilist **arr;
};

ilist *ilist_new(int first, ilist *rest)
{
    ilist *lst = (ilist *)malloc(sizeof(ilist));
    if (lst == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    lst->first = first;
    lst->rest = rest; // shallow
    return lst;
}

void ilist_free(ilist *lst)
{
    if (lst == NULL) {
        return;
    }
    ilist_free(lst->rest);
    free(lst);
}

void ilist_show(ilist *lst)
{
    while (lst != NULL) {
        printf("%d: ",lst->first);
        lst = lst->rest;
    }
    printf("nil\n");
}

int mypow(int a, unsigned int b)
{
    int r = 1;
    for (int i = 0; i < b; i++) {
        r *= a;
    }
    return r;
}

int sum_cubes(int n) 
{
    if (n == 0) {
        return 0;
    }
    return mypow(n,3) + sum_cubes(n-1);
}

ilist *lst_cpy(ilist *lst)
{
    // deep copy of ilist
    if (lst == NULL) {
        return NULL;
    }
    ilist *cpy = ilist_new(lst->first, NULL);
    ilist *cpy_curr = cpy;
    lst = lst->rest;    
    while (lst != NULL) {
        cpy_curr->rest = ilist_new(lst->first,NULL);
        cpy_curr = cpy_curr->rest;
        lst = lst->rest;
    }
    return cpy;
}

ilist **arrlists_new(int size)
{
    ilist **arr = (ilist **)malloc(sizeof(ilist *) * size);
    if (arr == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        arr[i] = NULL;
    }
    return arr;
}

lstlsts *lstlsts_new(int size, ilist **arr)
{
    lstlsts *lsts = (lstlsts *)malloc(sizeof(lstlsts));
    if (lsts == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    lsts->size = size;
    if (arr == NULL) { // want empty array of strlists
        lsts->arr = arrlists_new(1);
    } else {
        lsts->arr = arr; //shallow copy
    }
    return lsts;
}

void lstlsts_free(lstlsts *l)
{
    for (int i = 0; i < l->size; i++) {
        ilist_free(l->arr[i]);
    }
    free(l->arr);
    free(l);
}

void lstlsts_show(lstlsts *l)
{
    printf("List of %d lists\n",l->size);
    for (int i = 0; i < l->size; i++) {
        printf("List %d is ", i);
        ilist_show(l->arr[i]);
    }
}

lstlsts *sublists(ilist *lst)
{
    if (lst == NULL) {
        return lstlsts_new(1,NULL); // empty list
    }
    lstlsts *propsubs = sublists(lst->rest);
    int new_size = propsubs->size * 2;
    ilist **arr = arrlists_new(new_size);
    for (int i = 0; i < new_size / 2; i++) {
        arr[i] = lst_cpy(propsubs->arr[i]);
    }
    for (int i = propsubs->size; i < new_size; i++) {
        arr[i] = ilist_new(lst->first, lst_cpy(propsubs->arr[i-propsubs->size]));
    }
    lstlsts *subs = lstlsts_new(new_size,arr);
    lstlsts_free(propsubs);
    return subs;
}

struct itree {
    int root;
    struct itree *lsub;
    struct itree *rsub;
};
typedef struct itree itree;

itree *itree_new(int root, itree *lsub, itree *rsub)
{
    itree *t = (itree *)malloc(sizeof(itree));
    if (t == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    t->root = root;
    t->lsub = lsub;
    t->rsub = rsub;
    return t;
}

int min_depth(itree *t)
{
    if (t == NULL) {
        return -1;
    }
    // BASE CASE IS NO CHILDREN
    if (t->lsub == NULL && t->rsub == NULL) {
        return 1;
    }
    int ldepth = min_depth(t->lsub);    
    int rdepth = min_depth(t->rsub);
    int mdepth;
    if (ldepth == -1) {
        mdepth = rdepth;
    } else if (rdepth == -1) {
        mdepth = ldepth;
    } else {
        mdepth = ldepth < rdepth ? ldepth : rdepth;
    }
    return mdepth + 1;
}

int main()
{
    printf("Sum of first 0 cubes is %d\n",sum_cubes(0));    
    printf("Sum of first 1 cubes is %d\n",sum_cubes(1));    
    printf("Sum of first 2 cubes is %d\n",sum_cubes(2));    
    printf("Sum of first 3 cubes is %d\n",sum_cubes(3));    
    ilist *lst = ilist_new(1,ilist_new(3,ilist_new(6,ilist_new(0,NULL))));
    ilist_show(lst);
    ilist *lst2 = lst_cpy(lst);
    ilist_free(lst);
    ilist_show(lst2);   
    lstlsts *subs = sublists(lst2);
    lstlsts_show(subs);
    lstlsts_free(subs);
    
    itree *t = itree_new(1,NULL,NULL);
    itree *t2 = itree_new(3,NULL,itree_new(2,NULL,NULL));
    itree *t3 = itree_new(3,itree_new(1,itree_new(5,NULL,NULL),NULL),
            itree_new(2,itree_new(4,NULL,NULL),itree_new(9,NULL,NULL)));
    printf("Min depth of t1: expect 1 got %d\n",min_depth(t));
    printf("Min depth of t2: expect 1 got %d\n",min_depth(t2));
    printf("Min depth of t3: expect 3 got %d\n",min_depth(t3));

    return 0;
}


