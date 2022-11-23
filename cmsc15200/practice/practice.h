#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Enums
enum direction {CLOCKWISE, COUNTERCLOCKWISE};

struct rotate {
    enum direction direction;
    unsigned int degrees;
};

int same_direction(struct rotate rotations[], int num_rotations);

// Unions
struct cny {
    int jiao;
    int yuan;
};

struct gbp {
    int pence;
    int pounds;
};

struct usd {
    int cents;
    int dollars;
};

enum denomination {CNY, GBP, USD};

union money {
    struct cny cny;
    struct gbp gbp;
    struct usd usd;
};

struct wallet {
    enum denomination denomination;
    union money money;
};


void print_contents(struct wallet w);

struct wallet trade_in_coins(struct wallet w);

// Trees
typedef struct inttree inttree;

struct inttree {
    int val;
    inttree *left;
    inttree *right;
};

inttree *inttree_new(int val, inttree *left, inttree *right);
void inttree_free(inttree *t);
void inttree_show(inttree *t);

int preorder(inttree *t, int target, int *num_steps);
int inorder(inttree *t, int target, int *num_steps);
int postorder(inttree *t, int target, int *num_steps);

enum directions {LEFT, RIGHT};

int valid_path(inttree *t, enum directions directions[], int num_directions);
int valid_path_r(inttree *t, enum directions directions[], int num_directions);
// Linked lists
typedef struct strlist strlist;

struct strlist {
    char *word;
    strlist *next;
};

strlist *strlist_new(char *word, strlist *next);
void strlist_free(strlist *lst);
void strlist_show(strlist *lst);
int update(strlist *lst, char *old, char *news);
int update_r(strlist *lst, char *old, char *news);
strlist *insert(strlist *lst, char *news);
strlist *insert_r(strlist *lst, char *news);

int  my_strcmp(char *a, char *b);                                               
int strlist_len(strlist *lst);
// Hash tables
struct hash_table {
    unsigned int size;
    unsigned long int (*hash)(char *);
    strlist **table;
};
typedef struct hash_table htbl;

htbl *htbl_new(unsigned long int(*hash)(char *), unsigned int n_buckets);
void htbl_free(htbl *t);
unsigned long int hash(char *s);
void htbl_insert(htbl *t, char *s);
void htbl_show(FILE *f, htbl *t, int show_string);
unsigned long int hash(char *s);
double load_factor(struct hash_table *ht, int *min_bucket, int *max_bucket);

int **make_x(int size);
void show_md(int **x,int size);

typedef struct ilist queue;
struct ilist {
    inttree *first;
    struct ilist *rest;
};

queue *queue_new(inttree *first, queue *rest);

void print_breadth_first(inttree *t);
queue *dequeue(queue *lst, queue **lst_ptr);
queue *enqueue(queue *lst, inttree *add);
