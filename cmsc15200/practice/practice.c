#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "practice.h"


// Enums
int same_direction(struct rotate rotations[], int num_rotations)
{
    // in rotations, the degree is an unsigned int. To add unsigned
    // plus signed, better to CAST unsigned to signed
    int deg = 0; // total rotation in degrees, counterclockwise
    for (int i = 0; i < num_rotations; i++) {
        if (rotations[i].direction == CLOCKWISE) {
            deg -= (int) rotations[i].degrees;
        } else if (rotations[i].direction == COUNTERCLOCKWISE) {
            deg += (int) rotations[i].degrees;
        } else {
            fprintf(stderr,"invalid direction\n");
            exit(1);
        }
    }
    return (deg % 360) == 0;
}

// Unions
void print_contents(struct wallet w)
{
    if (w.denomination == CNY) {
        printf("Wallet contains CNY: %d jiao and %d yuan\n",
                w.money.cny.jiao, w.money.cny.yuan);
    } else if (w.denomination == GBP) {
        printf("Wallet contains GBP: %d pence and %d pounds\n",
                w.money.gbp.pence, w.money.gbp.pounds);
    } else if (w.denomination == USD) {
        printf("Wallet contains USD: %d cents and %d dollars\n", 
                w.money.usd.cents, w.money.usd.dollars);
    } else {
        fprintf(stderr,"Invalid denomination\n");
        exit(1);
    }
}

/*
struct wallet trade_in_coins(struct wallet w)
{
    struct wallet nw;
    if (w.denomination == CNY) {
        nw.denomination = CNY;
        int nyuan = w.money.cny.jiao / 10;
        nw.money.cny.yuan = nyuan + w.money.cny.yuan;
        nw.money.cny.jiao = w.money.cny.jiao % 10;
    } else if (w.denomination == GBP) {
        nw.denomination = GBP;
        int npounds = w.money.gbp.pence / 100;
        nw.money.gbp.pounds = npounds + w.money.gbp.pounds;
        nw.money.gbp.pence = w.money.gbp.pence % 100;
    } else if (w.denomination == USD) {
        nw.denomination = USD;
        int ndollars = w.money.usd.cents / 100;
        nw.money.usd.dollars = ndollars + w.money.usd.dollars;
        nw.money.usd.cents = w.money.usd.cents % 100;
    } else {
        fprintf(stderr,"Invalid denomination\n");
        exit(1);
    }
    return nw;
}
*/

// now do with switch and pointer to w for practice
struct wallet trade_in_coins(struct wallet *w)
{
    struct wallet nw;
    nw.denomination = w->denomination;
    switch (w->denomination) {
        case CNY:
            nw.money.cny.yuan = w->money.cny.jiao/10 + w->money.cny.yuan;
            nw.money.cny.jiao = w->money.cny.jiao % 10;
            return nw;
        case GBP:
            nw.money.gbp.pounds = w->money.gbp.pence/100 + w->money.gbp.pounds;
            nw.money.gbp.pence = w->money.gbp.pence % 100;
            return nw;
        case USD:
            nw.money.usd.dollars = w->money.usd.cents/100 + w->money.usd.dollars;
            nw.money.usd.cents = w->money.usd.cents % 100;
            return nw;
        default: 
            fprintf(stderr,"invalid denomination\n");
            exit(1);
    }
}

// Trees

inttree *inttree_new(int val, inttree *left, inttree *right)
{
    inttree *t = (inttree *)malloc(sizeof(inttree));
    if (t == NULL) {
        fprintf(stderr,"failed malloc\n");
        exit(1);
    }
    t->val = val;
    t->left = left;
    t->right = right;
    return t;
}

void inttree_free(inttree *t)
{
    if (t == NULL) {
        return;
    }
    inttree_free(t->left);
    inttree_free(t->right);
    free(t);
}

void inttree_show(inttree *t)
{
    if (t == NULL) {
        return;
    }
    printf("Root %d\n",t->val);
    printf("Left Subtree: \n");
    inttree_show(t->left);
    printf("Right Subtree: \n");
    inttree_show(t->right);
}

// CAREFUL CAREFUL
// THESE FUNCS YOU NEED TO RETURN TOTAL # STEPS YOU TRAVERSEDD IN TREE
// 
int preorder(inttree *t, int target, int *num_steps)
{
    // preorder: root, then left subtree, then right subtree
    if (t == NULL) {
        *num_steps = 0; // checked 0 nodes for empty tree;
        return 0;
    }
    // if num_steps not gauranteed to be 0 must first SET TO VALUE 
    *num_steps = 1; // checked target
    if (t->val == target) {
        return 1;
    }
    int add_steps; // no need set to 0 since set for null case
    int in_left = preorder(t->left, target, &add_steps);
    *num_steps += add_steps; // checked left
    if (in_left != 0) {
        return 1;
    }
    int add_steps_right; // no need set to 0 since set for null case
    int in_right = preorder(t->right, target, &add_steps_right);
    *num_steps += add_steps_right; // checked left 
    return in_right; // total number steps traversed
}

int inorder(inttree *t, int target, int *num_steps)
{
    // Inorder: left subtree, then root, then right subtree
    if (t == NULL) {
        *num_steps = 0; // checked 0 nodes for empty tree;
        return 0;
    }
    int add_steps; // no need set to 0 since set for null case
    int in_left = inorder(t->left, target, &add_steps);
    *num_steps = add_steps; // checked left 
    if (in_left != 0) {
        return 1;
    }
    *num_steps += 1; // checked target
    if (t->val == target) {
        return 1;
    }
    int add_steps_right; // no need set to 0 since set for null case
    int in_right = inorder(t->right,target,&add_steps_right);
    *num_steps += add_steps_right; // checked right 
    return in_right;
}

int postorder(inttree *t, int target, int *num_steps)
{
    // postorder: first left subtree, then right subtree, then root
    if (t == NULL) {
        *num_steps = 0; // checked 0 nodes for empty tree;
        return 0;
    }
    int add_steps; // no need set to 0 since set for null case
    int in_left = postorder(t->left, target, &add_steps);
    *num_steps = add_steps; // must SET it equal here
    if (in_left != 0) {
        return 1;
    }
    int add_steps_right; // no need set to 0 since set for null case
    int in_right = postorder(t->right,target,&add_steps_right);
    *num_steps += add_steps_right; // here increment
    if (in_right != 0) {
        return 1;
    }
    *num_steps += 1;
    return t->val == target;   
}

// here we must ASSUME NUMSTEPS = 0 before run
int preorder_a0(inttree *t, int target, int *num_steps)
{
    if (t == NULL) {
        return 0;
    }
    (*num_steps)++; // MUST DEREFERENCE POINTER
    //BE VERY VERY CAREFUL WHEN USING SHORTCUTS IN C
    //BETTER NOT TO DO ON EXAM SINCE U MESSED UP HERE
    //NEED PARENTHESES TO DEREFERENCE FIRST
    if (t->val == target) {
        return 1;
    }
    return preorder_a0(t->left, target, num_steps) || preorder_a0(t->right,
                        target,num_steps);
}

int inorder_a0(inttree *t, int target, int *num_steps)
{
    if (t == NULL) {
        return 0;
    }
    if (inorder_a0(t->left,target,num_steps) == 1) {
        return 1;
    }
    // increment for current node ONLY AFTER PREVIOUS NODE
    // IN ORDER - CHECK LEFT THEN CURRENT THEN RIGHT
    (*num_steps)++;
    if (t->val == target) {
        return 1;
    }
    return inorder_a0(t->right,target,num_steps);
}

int postorder_a0(inttree *t, int target, int *num_steps)
{
    if (t == NULL) {
        return 0;
    }
    // running function in if will increment num_steps
    if (postorder_a0(t->left,target,num_steps) == 1) {
        return 1;
    }
    if (postorder_a0(t->right,target,num_steps) == 1) {
        return 1;
    }
    // only increment AFTER YOU CHECK
    (*num_steps)++; // incrememnt by 1 for current step
    return t->val == target;
}

int valid_path(inttree *t, enum directions directions[], int num_directions) 
{
    // iterative solution is possible since we take one path through tree
    if (t == NULL) {
        fprintf(stderr,"null tree\n");
        exit(1);
    }
    
    for (int i = 0; i < num_directions; i++) {
        if (t == NULL) {
           return 0;
        }
        if (directions[i] == LEFT) {
            t = t->left;
        } else if (directions[i] == RIGHT) {
            t = t->right;
        } else {
            fprintf(stderr,"Invalid direction\n");
            exit(1);
        }
    }
    if (t == NULL || t->left != NULL || t->right != NULL) {
        return 0;
    }
    return 1;
}
 
int valid_path_r(inttree *t, enum directions directions[], int num_directions) 
{
    // recursive solution in which each call is 1 step, can also add in 
    // a separate parameter to helper function for step number 
    if (t == NULL) {
        return 0;
    }
    if (t->left == NULL && t->right == NULL) {
        if (num_directions == 0) {
            return 1;
        } else {
            return 0;
        }
    }
    // no more steps but we're not at a leaf yet
    if (num_directions == 0) {  
        return 0;
    }
    // take next step
    if (directions[0] == LEFT) {
        // MUST PASS POINTER (can also do directions + 1) for next direction
        return valid_path_r(t->left,directions + 1, num_directions - 1);
    } else if (directions[0] == RIGHT) {
        return valid_path_r(t->right,&directions[1],num_directions - 1);
    } else {
        fprintf(stderr,"invalid direction\n");
        exit(1);
    }
}
    
strlist *strlist_new(char *word, strlist *next) 
{
    strlist *l = (strlist *)malloc(sizeof(strlist));
    if (l == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    l->word = strdup(word);
    l->next = next;
    return l; // DO NOT FORGET TO RETURN
}

void strlist_free(strlist *lst)
{
    if (lst == NULL)
        return;
    strlist_free(lst->next);
    free(lst);
}
void strlist_show(strlist *lst)
{
    if (lst == NULL) { 
        printf("(nil)\n");
        return;
    }
    printf("%s: ",lst->word);
    strlist_show(lst->next);
}

int update(strlist *lst, char *old, char *news)
{
    int s = 0;
    // pick better var namign
    while (lst != NULL) {
        if (strcmp(lst->word, old) == 0) {
            s = 1;
            free(lst->word);
            lst->word = strdup(news);
        }
        lst = lst->next;
    }
    return s; //NO IF STATEMENT NEEDED
}

int update_r(strlist *lst, char *old, char *news)
{
    if (lst == NULL) {
        return 0; // must return int
    }
    
    if (strcmp(lst->word, old) == 0) {
        free(lst->word);
        lst->word = strdup(news);
        return 1;
    }
    return update_r(lst->next,old,news);
}

/*
strlist *insert(strlist *lst, char *news)
{
    if (lst == NULL) {
        return strlist_new(news,NULL);
    }
    // check
    if (strcmp(news,lst->word) < 0) {
        return strlist_new(news,lst);
    }
    strlist *prev = lst;
    strlist *curr = lst->next;
    while (curr != NULL) {
        // INSERT WHEN WORD BEFORE CURRENT ELEM
        if (strcmp(news,curr->word) < 0) {
            strlist *add = strlist_new(news,curr);
            prev->next = add;
            return lst;
        } else if (strcmp(curr->word,news) == 0) {
            // MUST CHECK IF STRING ALREADY IN LIST IDIOT IDIOT IDIOT
            fprintf(stderr,"%s already in list\n",news);
            exit(1);
        }
        prev = curr;
        curr = curr->next;
    }
    strlist *add = strlist_new(news,NULL);
    prev->next = add;
    return lst;
}
*/

// better
strlist *insert(strlist *lst, char *news)
{
    if (lst == NULL) {
        // we still insert if list is NULL
        return strlist_new(news,NULL);
    }
    strlist *prev = NULL;
    strlist *curr = lst;
    while (curr != NULL) {
        int cmp = strcmp(news, curr->word);
        if (cmp == 0) {
            fprintf(stderr,"word already in list\n");
            exit(1);
        }
        if (cmp < 0) {
            if (prev == NULL) {
                return strlist_new(news,curr);
            } else {
                prev->next = strlist_new(news,curr);
                return lst;
            }
        }
        prev = curr;
        curr = curr->next;
    }
    // insert at end
    prev->next= strlist_new(news, NULL);
    return lst;
}

strlist *insert_r(strlist *lst, char *news)
{
    if (lst == NULL) {
        return strlist_new(news, NULL);
    }
    if (strcmp(lst->word,news) > 0) {
        return strlist_new(news,lst);
    }
    // DONT CHANGE CURRENT ELEMENT
    // REPLACE THE NEXT ELEMENT WITH THE INSERTED LIST
    lst->next = insert(lst->next, news);
    return lst; 
}

strlist *insert_pos(strlist *lst, char *news, int *pos)
{
    *pos = 0;
    if (lst == NULL) {
        return strlist_new(news,NULL);
    }
    strlist *prev = NULL;
    strlist *curr = lst;
    while (strcmp(news,curr->word) > 0) {
        *pos += 1;
        if (curr->next == NULL) {
            curr->next = strlist_new(news,NULL);
            return lst;
        }
        prev = curr;
        curr = curr->next;
    }
    prev->next = strlist_new(news,curr);
    return lst;
}

int my_strcmp(char *a, char *b)
{
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return a[i] - b[i];
        }
        i++;
    }
    return a[i] - b[i];
}

int strlist_len(strlist *lst) 
{
    int len = 0;
    while (lst != NULL) {
        len++;
        lst = lst->next;
    }
    return len;
}

strlist *reverse(strlist *lst)
{
    if (lst == NULL) {
        return NULL;
    }
    strlist *prev = NULL;
    while (lst != NULL) {
        strlist *temp = lst->next;
        lst->next = prev;
        prev = lst;
        lst = temp;
    }
    return prev;
}

strlist *sort(strlist *lst)
{
    // sort alphabetically
    // find min element, move to front, sort rest
    if (lst == NULL) {
        return NULL;
    }
    char *min = lst->word;
    strlist *curr = lst->next;
    while (curr != NULL) {
        if (strcmp(curr->word, min) < 0) {
            min = curr->word;
        }
        curr = curr->next;
    }
    // move min to front
    strlist *prev = NULL;
    curr = lst;
    while (curr != NULL) {
        if (strcmp(curr->word,min) == 0) {
            if (prev != NULL) {
                prev->next = curr->next;
                curr->next = lst;
            }
            curr->next = sort(curr->next);
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }
    fprintf(stderr,"Shouldn't reach here\n");
    exit(1);
}

htbl *htbl_new(unsigned long int(*hash)(char *), unsigned int size)
{
    strlist **table = (strlist **)malloc(sizeof(strlist *) * size);
    if (table == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        table[i] = NULL; //MUST INITIALIZE NULL POINTER
    }
    htbl *tbl = (htbl *)malloc(sizeof(htbl));
    if (tbl == NULL) {
        fprintf(stderr,"malloc failed\n"); 
        exit(1);
    }
    tbl->hash = hash;
    tbl->size = size;
    tbl->table = table;
    return tbl; //RETURN CORRECT THING 
}

void htbl_free(htbl *t)
{
    for (int i = 0; i < t->size;i++) {
        strlist_free(t->table[i]);
    }
    free(t->table);
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
    i = i % t->size;
    strlist *lst = strlist_new(s,NULL);
    if (t->table[i] == NULL) {
        t->table[i] = lst;
    } else {
        strlist *curr = t->table[i];
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = lst;
    }
}
void htbl_show(FILE *f, htbl *t, int show_string)
{
    if (t == NULL) {
        fprintf(stderr,"NULL table \n");
        exit(1);
    }
    for (int i = 0; i < t->size; i++) {
        fprintf(f, "%d: ", i);
        if (show_string) {
            strlist_show(t->table[i]);
            putc('\n',f);
        } else {
            fprintf(f, " (%d items)\n",strlist_len(t->table[i]));
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


double load_factor(struct hash_table *ht, int *min_bucket, int *max_bucket)
{
    *max_bucket = 0;
    int tot_elem = 0;   
    for (int i = 0; i < ht->size; i++) {
        int num = 0;
        strlist *curr = ht->table[i];
        while (curr != NULL) {
            curr = curr->next;
            num++;
        }
        tot_elem += num;
        if (i == 0 || num < *min_bucket) {
            // can also set min_bucket to max int, but this works too
            *min_bucket = num;
        }
        if (num > *max_bucket) {
            *max_bucket = num;
        }
    }
    return (double) tot_elem / ht->size;
    // MUST CAST AS DOUBLE OR JUST MAKE TOT-elem a double
    // do not forget idiot 
}

int **make_x(int size)
{
    int **x = (int **)malloc(sizeof(int *) * size);
    if (x == NULL) {
        fprintf(stderr,"malloc faiiled\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        x[i] = (int *)malloc(sizeof(int) * size);
        if (x[i] == NULL) {
            fprintf(stderr,"malloc failed\n");
            exit(1);
        }
        for (int j = 0; j < size; j++) {
            if (j == i || j == size - i - 1) {
                x[i][j] = 1;
            } else {
                x[i][j] = 0;
            }
        }
    }
    return x;
}

void show_md(int **x,int size)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ",x[i][j]);
        }
        printf("\n");
    }
}

queue *queue_new(inttree *first, queue *rest)
{
    queue *lst = (queue *)malloc(sizeof(queue));
    if (lst == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    lst->first = first;
    lst->rest = rest;
    return lst;
}

queue *enqueue(queue *lst, inttree *add)
{
    // better to have inttree type as parameter
    if (add == NULL) {
        return lst;
    }
    queue *q_add = queue_new(add,lst);
    return q_add;
}

queue *dequeue(queue *lst, queue **lst_ptr)
{
    if (lst == NULL) {
        fprintf(stderr, "can't dequeue from empty queue\n");
        exit(1);
    }
    if (lst->rest == NULL) {
        *lst_ptr = NULL; // lst is null
        return lst;
    }
    while (lst->rest->rest != NULL) {
        lst = lst->rest;
    }
    queue *end = lst->rest;
    lst->rest = NULL;
    return end;
}


void print_breadth_first(inttree *t)
{
    if (t == NULL) {
        return;
    }
    queue *q = queue_new(t, NULL);
    while (q != NULL) {
        queue *end = dequeue(q, &q);
        inttree *t = end->first;
        printf("%d \n", t->val);
        q = enqueue(q, t->left);
        //CAREFUL ON TYPE AGREEMENT 
        q = enqueue(q,t->right);
    }
}
