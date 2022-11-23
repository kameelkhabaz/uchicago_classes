#include <stdio.h>
#include <stdlib.h>

typedef struct bst_tree bst;

struct bst_tree {
    int root;
    bst *lsub;
    bst *rsub;
};

bst *bst_new(int a, bst *l, bst *r)
{
    bst *t = (bst *)malloc(sizeof(bst));
    if (t == NULL) {
        fprintf(stderr,"malloc fail\n");
        exit(1);    
    }
    t->root = a;
    t->lsub = l;
    t->rsub = r;
    return t;
}

void bst_free(bst *t)
{
    if (t == NULL)
        return;
    bst_free(t->lsub);
    bst_free(t->rsub);
    free(t);
}

int find(bst *t, int a) 
{
    if (t == NULL) {
        return 0;
    } else if (t->root == a) {
        return 1;
    } else if (a < t->root) {
        return find(t->lsub, a);
    } else {
        return find(t->rsub,a);
    }
}

int find_iter(bst *t, int a)
{
    while (t != NULL) {
        if (a == t->root) {
            return 1;
        } else if(a < t->root) {
            t = t->lsub;
        } else {
            t = t->rsub;
        }
    }
    return 0;
}

int sum(bst *t)
{
    if (t == NULL) {
        return 0;
    }
    
    int s = 0;
    s += t->root;
    s += sum(t->lsub);
    s += sum(t->rsub);
    return s;
}

int count_between(bst *t, int low, int high)
{
    // inclusive
    if (t == NULL) {
        return 0;
    }
    if (t->root < low) {
        return count_between(t->rsub,low,high);
    } else if (t->root > high) {
        return count_between(t->lsub, low, high);
    } else {
        int count = 1; // root
        count += count_between(t->lsub, low, high);
        count += count_between(t->rsub, low, high);
        return count;
    }
}

int max(bst *t) 
{
    if (t == NULL) {
        return 0;
    }
    while (t->rsub != NULL) {
        t = t->rsub;
    }
    return t->root;
}

bst *successor(bst *t)
{
    if (t == NULL) {
        return NULL;
    } else if (t->rsub == NULL) {
        return t;
    }
    t = t->rsub;
    while (t->lsub != NULL) {
        t = t->lsub;
    }
    return t;
}

bst *add(bst *t, bst *nn)
{
    if (t == NULL) {
        return nn;
    }
    if (t->root == nn->root) {
        fprintf(stderr,"Already in tree, can't add %d\n",nn->root);
        exit(1);
    }
    if (nn->root < t->root) {
        t->lsub = add(t->lsub, nn);
    } else {
        t->rsub = add(t->rsub, nn);
    }
    return t;
}

bst *bst_add(bst *t, int a) 
{
    bst *nn = bst_new(a,NULL,NULL);
    return add(t, nn);
}

bst *add_iter(bst *t, bst *nn)
{
    if (t == NULL) {
        return nn;
    }
    bst *curr = t;
    bst *par = NULL; //parent of currr
    while (curr != NULL) {
        par = curr;
        if (nn->root < t->root) {
            curr = curr->lsub;
        } else if (nn->root > t->root) {
            curr = curr->rsub;
        } else {
            fprintf(stderr,"Already in tree, can't add %d\n",nn->root);
            exit(1);
        }
    }
    if (nn->root < par->root) {
        par->lsub = nn;
    } else {
        par->rsub = nn;
    }
    return t;
}

bst *bst_add_iter(bst *t, int a)
{
    bst *nn = bst_new(a, NULL,NULL);
    return add_iter(t, nn);
}

bst *bst_remove(bst *t, int a)
{
    if (t == NULL) {
        return NULL;
    }
    
    if (a < t->root) {
        t->lsub = bst_remove(t->lsub, a);
        return t;
    } else if (a > t->root) {
        t->rsub = bst_remove(t->rsub, a);
        return t;
    } else {
        if (t->lsub == NULL && t->rsub == NULL) {
            bst_free(t);
            return NULL;
        } else if (t->rsub == NULL) {
            bst *left = t->lsub;
            free(t); // free ONLY that node
            return left;
        } else if (t->lsub == NULL) {
            bst *right = t->rsub;
            free(t);
            return right;
        } else {
            bst *succ = successor(t);
            bst *nsucc = bst_new(succ->root, t->lsub, NULL);
            nsucc->rsub = bst_remove(t->rsub, succ->root);
            return nsucc;
        }
    }
}


bst *bst_dup(bst *orig)
{
    if (orig == NULL) {
        return NULL;
    }
    bst *t = (bst *)malloc(sizeof(bst));
    if (t == NULL) {
        fprintf(stderr,"malloc fail\n");
        exit(1);    
    }
    t->root = orig->root;
    t->lsub = bst_dup(orig->lsub);
    t->rsub = bst_dup(orig->rsub);
    return t;
}

bst *right_rotate(bst *t) 
{
    if (t == NULL) {
        return NULL;
    }
    if (t->lsub == NULL) {
        return t;
    }
    bst *nt = t->lsub;
    t->lsub = nt->rsub;
    nt->rsub = t; // DONT FORGET
    return nt;
} 

void bst_show(bst *t)
{
    if (t == NULL) {
        printf("(nil) ");
        return;
    }
    bst_show(t->lsub);
    printf("| %d | ",t->root);
    bst_show(t->rsub);
}
void bst_depth_first_show(bst *t, int depth)
{
    if (t == NULL) {
        return;
    }
    printf("depth %d %d\n",depth,t->root);
    bst_depth_first_show(t->lsub,depth+1);
    bst_depth_first_show(t->rsub,depth+1);  
    return;
}

int main()
{
    bst *t = bst_new(4, bst_new(1,NULL,NULL), bst_new(8,NULL,NULL));
    bst_show(t);
    printf("\n");
    bst_add(t, 2);
    bst_add(t,3);
    bst_add(t,5);
    bst_add(t,6);
    bst_add(t,7);
    bst_add_iter(t,10);
    bst *t2 = bst_dup(t);
    bst_show(t);
    printf("\n");
    printf("find 2: %d\n",find(t,2));
    printf("find 0: %d\n",find(t,0));
    printf("find_iter 8: %d\n",find_iter(t,8));
    printf("find_iter 9: %d\n",find_iter(t,9));
    printf("find_iter 8: %d\n",find_iter(t,8));
    printf("Sum is %d\n",sum(t));
    printf("%d nodes between %d and %d,inclusive\n",
            count_between(t,1,10),1,10);
    printf("%d nodes between %d and %d,inclusive\n",
            count_between(t,1,3),1,3);
    printf("%d nodes between %d and %d,inclusive\n",
            count_between(t,11,12),11,12);
    printf("%d is max of tree\n",max(t));
    printf("%d is value of successor of tree\n",successor(t)->root);
    printf("Removing 10\n");
    t = bst_remove(t,10);
    bst_show(t);
    printf("Removing 8\n");
    t = bst_remove(t,8);
    bst_show(t);
    printf("Removing 4\n");
    t = bst_remove(t,4);
    bst_show(t);
    printf("\n Showing copy tree \n");
    bst_show(t2);
    printf("Rotating right the right subtree\n");
    t2->rsub = right_rotate(t2->rsub);
    bst_show(t2); 
    printf("\n");
    bst_depth_first_show(t2,0);
    return 0;
}

