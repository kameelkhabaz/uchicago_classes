#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum department {ARTH, BIOS, CHEM, CMSC, GNSE, SOCI};

struct course {
   enum department dept;
   unsigned int num;
   char *title;
   char *descrip;
   // imagine more fields here as you see fit...
};
 struct bst_node {
   char *key; // a string like "CMSC15200"
   struct course *c;
   struct bst_node *lsub;
   struct bst_node *rsub;
 };
typedef struct bst_node bst;

char *dept_string(enum department d)
 {
   switch (d) {
     case ARTH: return "ARTH";
     case BIOS: return "BIOS";
     case CHEM: return "CHEM";
     case CMSC: return "CMSC";
     case GNSE: return "GNSE";
    
      case SOCI: return "SOCI";
  }
  fprintf(stderr,"dept_string: unrecognized department code %d\n",d);
exit(1); }
char *build_key(struct course *c)
{
  // build a string like "CMSC15200"
  char buf[10] = {0}; // we already know this is a 9-char string
  sprintf(buf,"%s%d",dept_string(c->dept),c->num);
  return strdup(buf);
}

bst *bst_singleton(struct course *c)
{
    // build a tree with one node in it
    // see how this is a DEEP copy constructor for course
    bst *t = (bst *)malloc(sizeof(bst));
    if (!t) {
      fprintf(stderr,"bst_singleton: allocation failed\n");
      exit(1);
    }
    char *k = build_key(c);
    t->key = k;
    struct course *c_hp = (struct course *)malloc(sizeof(struct course));
    if (c_hp == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    c_hp->dept = c->dept;
    c_hp->num = c->num;
    c_hp->title = strdup(c->title);
    c_hp->descrip = strdup(c->descrip);
    t->c = c_hp;
    t->lsub = NULL;
    t->rsub = NULL;
    return t; 
}

struct course *bst_find(bst *t, char *k)
{
    // we are looking in search tree t for string k (like "CMSC15200")
    // return a pointer to the course struct, or NULL if not found
    bst *curr = t;
    while (curr!=NULL) {
        int cmp = strcmp(k,curr->key);
    if (cmp==0)
        return curr->c;
    else if (cmp<0)
        curr = curr->lsub;
    else //cmp>0
        curr = curr->rsub;
    }
    return NULL;
}

bst *bst_insert(bst *t, struct course *c)
{
    // insert course in tree, return pointer to root of tree
    if (t==NULL)
        return bst_singleton(c);
    char *k = build_key(c);
    bst *curr = t;
    free(k);
    while (curr!=NULL) {
        int cmp = strcmp(k, curr->key);
    if (cmp==0) {
        fprintf(stderr,"WARNING: tree already contains %s\n",k);
        free(k);
        return t; }
    else if (cmp<0) {
        if (curr->lsub == NULL) {
            curr->lsub = bst_singleton(c);
            free(k);
            return t; }
        curr = curr->lsub;
    } else {
        if (curr->rsub == NULL) {
            curr->rsub = bst_singleton(c);
            free(k);
            return t;
        }
        curr = curr->rsub;
    }
    }
  // this should be unreachable
  fprintf(stderr,"ERROR: bug in bst_insert\n");
  exit(1);
}

bst *bst_recur_insert(bst *t, struct course *c)
{
    if (t == NULL) {
        return bst_singleton(c);
    }
    char *k = build_key(c);
    int cmp = strcmp(k ,t->key);
    if (cmp > 0) {
        t->rsub = bst_recur_insert(t->rsub, c);
    } else if (cmp < 0) {
        t->lsub = bst_recur_insert(t->lsub, c);
    } else {
        fprintf(stderr,"Warning, course already in tree\n");
    }
    free(k);
    return t;
}

void bst_write(FILE *f, bst *t, unsigned int depth)
{
  if (t==NULL)
    return;
  if (depth>0) {
    for (int i=0; i<depth; i++)
      putc(' ',f);
    fprintf(f,"- ");
  }
  fprintf(f,"%s (%s)\n",t->key,t->c->title);
  bst_write(f,t->lsub,depth+1);
  bst_write(f,t->rsub,depth+1);
}

void free_course(struct course *c)
{
    free(c->title);
    free(c->descrip);
    free(c);
} 

void bst_free(bst *t)
{
    if (t == NULL) {
        return;
    }
    bst_free(t->lsub);
    bst_free(t->rsub);
    free_course(t->c);
    free(t->key);
    free(t);
}

bst *find_max(bst *t)
{
    // BST IS SORTED SORTED SORTED
    if (t == NULL) {
        return NULL;
    }
    while (t->rsub != NULL) 
        t = t->rsub;
    return t;
}

bst *delete(bst *t, char *krem)
{
    if (t == NULL) {
        return NULL;
    }
    // NO == FOR STRING EQUALITY IN C
    int cmp = strcmp(krem,t->key);
    if (cmp == 0) {
        if (t->lsub == NULL && t->rsub == NULL) {
            bst_free(t);
            return NULL;
        } else if (t->lsub == NULL) {
            bst *temp = t->rsub;
            t->rsub = NULL;
            bst_free(t);
            return temp;
        } else if (t->rsub == NULL) {
            bst *temp = t;
            t = t->rsub;
            temp->rsub = NULL;
            free(temp);
        } else {
            bst *max = find_max(t->lsub);
            // need to copy max to remove pointers to it
            bst *mcopy = bst_singleton(max->c);
            mcopy->lsub = delete(t->lsub,max->key);
            mcopy->rsub = t->rsub;
            t->lsub = NULL;
            t->rsub = NULL;
            bst_free(t);
            return mcopy;
        }
    } else if (cmp < 0) {
        t->lsub = delete(t->lsub,krem);
    } else {
        t->rsub = delete(t->rsub,krem);
    }
    return t;
}            
// enum department {ARTH, BIOS, CHEM, CMSC, GNSE, SOSC};
int main()
{
  // here is some made-up course data
  struct course arth =
    {ARTH, 10100, "Introduction to Art History", "all about art"};
  struct course bios =
    {BIOS, 10100, "Introduction to Biology", "all about biology"};
  struct course chem =
    {CHEM, 10100, "Introduction to Chemistry", "all about chemistry"};
  struct course cs01 =
    {CMSC, 10100, "Introduction to Computer Science", "all about CS"};
  struct course cs02 =
    {CMSC, 10200, "More Computer Science", "even more about CS"};
  struct course cs03 =
    {CMSC, 10300, "More Computer Science 2", "even more about CS"};
  struct course gnse =
    {GNSE, 10100, "Introduction to Gender Studies", "all about gender"};
  struct course gnse2 =
    {GNSE, 10102, "Introduction to Gender Studies 2", "all about gender"};

  struct course sosc =
    {SOCI, 10100, "Introduction to Sociology", "sociology!"};

  // build tree in scrambled order
  struct course courses[] = {cs02, bios, cs01, arth, chem, gnse, cs03, gnse2, sosc};
  bst *t = NULL;
  for (int i=0; i<9; i++)
        t = bst_recur_insert(t, &courses[i]);
  printf(">>> here's the whole tree...\n");
  bst_write(stdout,t,0);
  printf("\n");
  char *k = build_key(&sosc);
  printf("Removing %s\n",k);
  t = delete(t, k);
  free(k);
  bst_write(stdout,t,0);
    printf("\n");
  printf(">>> searching the tree for courses...\n");
  char *keys[] = {"CMSC10100", "CMSC10200", "GNSE10100", "PHYS11100", "CMSC22300"};
  
    for (int i=0; i<5; i++) {
        struct course *c = bst_find(t, keys[i]);
        printf("%s: %s\n", keys[i], c!=NULL ? c->title : "(course not found)");
    }
  // the tree is not freed in this demo code
  // you must be careful freeing this tree since it points to un-malloc'd data
  // there are other memory leaks in this code too...
  // fixing them left as an exercise to the interested student
  bst_free(t);
}


