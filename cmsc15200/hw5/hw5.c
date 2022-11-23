#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* Kameel Khabaz
 * CMSC 15200
 * Homework 5
 * Feb 18 2022 */

/* === data definitions === */

typedef struct ilist_node ilist;

struct ilist_node {
  int first;
  ilist *rest;
};

// convention: NULL means empty list

typedef struct itree_node itree;

struct itree_node {
  int root;
  itree *lsub;
  itree *rsub;
};

// convention: NULL means empty tree

/* === operations (forward declarations) === */

// build a new ilist from int and pointer
ilist *ilist_new(int f, ilist *r);

// free the whole ilist, assuming everything is heap allocated 
// and safe to free (not shared)
void ilist_free(ilist *ns);

// use Haskell-style notation to write a list
// in the form "1:3:7:nil"
// - that is, put colons between numbers, and "nil" at the end
// - if newline is false, don't write a newline at end, otherwise do
// - note that the FILE* argument can be stdout or stderr
void ilist_write(FILE *f, ilist *ns, int newline);

// allocate a whole new list, with the same numbers
// in the same order (this is a "copy constructor")
ilist *ilist_dup(ilist *ns);

// if arr is NULL, return NULL, otherwise
// build a list with the same numbers in the
// same order as the given array
ilist *ilist_from_array(int *arr, unsigned int len);

// build a new tree from a root value and two subtrees
itree *itree_new(int root, itree *lsub, itree *rsub);

// free the whole itree, assuming everything is heap allocated 
// and safe to free (not shared)
void itree_free(itree *t);

// write a parenthesized tree of the form "(root [lsub] [rsub])"
// with a period "." for each empty tree
// - for example, the tree with root 1 and two empty subtrees
//   should be written "(1 . .)"
// - note that the FILE* argument can be stdout or stderr
void itree_write(FILE *f, itree *t);

// allocate a whole new tree, with the same numbers
// in the same positions (another "copy constructor")
itree *itree_dup(itree *t);

// compute the sum of all ints in the list
int ilist_sum(ilist *ns);

// compute the sum of all ints in the tree
int itree_sum(itree *t);

// compute the length of the list
unsigned int ilist_len(ilist *ns);

// count the number of nodes in the tree
unsigned int itree_size(itree *t);

// the height of the empty tree is 0;
// the height of a nonempty tree is the length
// of the longest path from root to leaf
unsigned int itree_height(itree *t); 

// build the list of all the ints in a tree,
// moving from left to right
ilist *inorder(itree *t);

// build the list of all in the ints at leaf position
// in the tree, moving from left to right
ilist *leaves(itree *t);

// this is like ilist_write above, but must produce
// a heap-allocated string and return it (no newline)
char *ilist_tos(ilist *ns);

// test if the two lists have the same ints in the same order
// (and are the same length)
int ilist_eq(ilist *ms, ilist *ns);

// look for an int in the list that passes the test;
// if found, return a pointer to its node, if not found,
// return NULL
ilist *ilist_find(int(*test)(int), ilist *ns); 

// count the number of ints in the list that pass the test
unsigned int ilist_count(int(*test)(int), ilist *ns);

// if n is 0, return NULL, otherwise
// build a list f(0), f(1), ... f(n-1)
// - for example, assuming a function "sqr",
//   ilist_build(4, sqr) --> 0:1:4:9:nil
ilist *ilist_build(unsigned int n, int(*f)(int));

/* === complete the operations specified above === */

// Helper function to join 2 lists by attaching right list to left list
// Returns pointer to joined list
ilist *ilist_join(ilist *left_ns, ilist *right_ns);

// Basic functions
int iseven(int a); 
int is_div_3(int a);
int sqr(int a);
int dbl(int a);

/* === BASIC FUNCTIONS === */
int is_even(int a)
{
    /* Return 1 if is even, otherwise 0 */
    return (a % 2 == 0) ? 1 : 0;
}

int is_div_3(int a)
{
    /* Return 1 if is div by 3, otherwise 0 */
    return (a % 3 == 0) ? 1 : 0;
}

int sqr(int a)
{   
    /* Return square of number */
    return a * a;
}

int dbl(int a)
{
    /* Return double of number */
    return a * 2;
}

/* === LIST FUNCITONS === */

ilist *ilist_new(int f, ilist *r)
{
    /* Build a new ilist from int and pointer */
    ilist *ns = (ilist *)malloc(sizeof(ilist));
    if (ns == NULL) {
        fprintf(stderr,"iliar_new: malloc allocation has failed\n.");
        exit(1);
    }
    ns->first = f;
    ns->rest = r;
    return ns;
}

void ilist_free(ilist *ns)
{
    /* Free whole ilist, assuming heap allocated and safe to free */
    // Base case: empty list
    if (ns == NULL) {
        return;
    }
    // Recursive case: nonempty list 
    ilist_free(ns->rest);
    free(ns);
}

void ilist_write(FILE *f, ilist *ns, int newline)
{   
    /* use Haskell-style notation to write a list in the form "1:3:7:nil"
     * if newline is false, don't write a newline at end, otherwise do
     * note that the FILE* argument can be stdout or stderr */
    if (ns == NULL) {
        // Base case: empty list 
        if (newline == 0)
            fprintf(f,"nil");
        else
            fprintf(f,"nil\n");
    } else {
        // Recursive case: non-empty list
        fprintf(f,"%d:",ns->first);
        ilist_write(f,ns->rest,newline);
    }
}

char *ilist_tos(ilist *ns)
{
    /* this is like ilist_write above, but must produce
     * a heap-allocated string and return it (no newline) */

    // Use iterative approach
    unsigned int len = ilist_len(ns);

    char *s = (char *)malloc(len * 64 + 4);
    // int won't be more than 64 digits long on any machine
    // add 3 for "nil" and 1 for '\0'
    // sizeof char is 1
    if (s == NULL) {
        fprintf(stderr,"ilist_tos: malloc allocation failed\n");
        exit(1);
    }

    char *curr = s; // pointer to current char in string
    
    while (ns != NULL) {
        int i = sprintf(curr,"%d:",ns->first);
        // Advance pointer until next empty position
        curr += i;
        ns = ns->rest;
    }
    sprintf(curr,"nil");
    curr += 3;
    *curr = '\0';
    
    // now copy string excluding extra space to deallocate extra memory
    char *cut_s = strdup(s);
    free(s);
    return cut_s;
}

ilist *ilist_dup(ilist *ns)
{
    /* allocate a whole new list, with the same numbers in the same order
     * this is a "copy constructor" */

    // Base case: empty list
    if (ns == NULL) {
        return NULL;
    }
    // Recursive case: non-empty list
    ilist *rest2 = ilist_dup(ns->rest);
    ilist *ns2 = ilist_new(ns->first, rest2);
    return ns2;
}

ilist *ilist_from_array(int *arr, unsigned int len)
{
    /* if arr is NULL, return NULL, otherwise build a list 
     * with the same numbers in the same order as the given array */
    // Checking NULL array
    if (arr == NULL) {
        return NULL;
    }
    // Create list from subarray starting at next index

    // Create pointer for next element of array 
    int *next_elem = (len > 1) ? arr + 1 : NULL;
    // Base case: if pointer is NULL, rest will be NULL
    // Recursive case: we get a list from the subarray
    ilist *rest = ilist_from_array(next_elem, len - 1);

    // Create list for current index of array    
    // and add list from the subarray (rest)
    ilist *ns = ilist_new(*arr, rest);
    return ns;
}

int ilist_sum(ilist *ns)
{
    /* compute sum of all ints in list */
    // Base case: empty list
    if (ns == NULL) {
        return 0;
    }
    // Recursive case: non-empty list
    return ns->first + ilist_sum(ns->rest);
}

unsigned int ilist_len(ilist *ns)
{
    /* compute length of the list */
    // Base case: empty list
    if (ns == NULL) {
        return 0;
    }
    // Recursive case: non-empty list
    return 1 + ilist_len(ns->rest);
}

ilist *ilist_join(ilist *left_ns, ilist *right_ns)
{
    /* helper functions to join two ilists, in which the right list is 
     * attached to the end of the left list. If left list is a node, return
     * right list. Returns pointer to joined list.*/
    if (left_ns == NULL) {
        return right_ns;
    }
    
    ilist *curr = left_ns; 
    // Iterate to end of left list  
    while (curr->rest != NULL) {
        curr = curr->rest;
    }
    // Set rest of last node of left list to pointer for right list
    curr->rest = right_ns;

    return left_ns;
}

int ilist_eq(ilist *ms, ilist *ns)
{
    /* test if the two lists have the same ints in the same order
     * (and are the same length) */
    // Base case: either or both of lists are null
    if (ms == NULL && ns == NULL) {
        return 1;
    } else if (ms == NULL || ns == NULL) {
        return 0;
    }
    // Recursive case: neither list is null
    // Check if current value is equal
    if (ms->first != ns->first) {
        return 0;
    }
    // Check rest of lists
    return ilist_eq(ms->rest, ns->rest);
}

ilist *ilist_find(int(*test)(int), ilist *ns)
{
    /* look for an int in the list that passes the test
     * if found, return a pointer to its node, if not found, return NULL */
    // Base case: empty list
    if (ns == NULL) {
        return NULL;
    }
    // Recursive case: non-empty list
    // If current value passes test, return 1. If not, check next int 
    return (*test)(ns->first) ? ns : ilist_find(test,ns->rest);
}
 
unsigned int ilist_count(int(*test)(int), ilist *ns)
{
    /* count the number of ints in the list that pass the test */

    // Iterate over list and add sum
    unsigned int sum = 0;
    while (ns != NULL) {
        if ((*test)(ns->first)) {
            sum++;
        }
        ns = ns->rest;
    }
    return sum;
}

ilist *ilist_build(unsigned int n, int(*f)(int))
{
    /* if n is 0, return NULL, otherwise build list f(0), f(1), ... f(n-1)
     * for example, assuming a function "sqr", 
     * ilist_build(4, sqr) --> 0:1:4:9:nil */

    if (n == 0) {
        return NULL;
    }

    int arr[n];
    for (int i = 0; i < n; i++) {
        arr[i] = (*f)(i);
    }

    return ilist_from_array(arr, n);
}

/* === TREE FUNCITONS === */

itree *itree_new(int root, itree *lsub, itree *rsub)
{
    /* Build a new tree from a root value and two subtrees */

    itree *t = (itree *)malloc(sizeof(itree));
    if (t == NULL) {
        fprintf(stderr,"itree_new: allocation malloc failed\n");
        exit(1);
    }
    t->root = root;
    t->lsub = lsub;
    t->rsub = rsub;
    return t;
}

void itree_free(itree *t)
{
    /* Free whole itree, assuming everything heap allocated 
     * and safe to free */
    // Base case: empty tree
    if (t == NULL) {
        return;
    }
    // Recursive case: nonempty tree
    itree_free(t->rsub);
    itree_free(t->lsub);
    free(t);
}

void itree_write(FILE *f, itree *t)
{
    /* write a parenthesized tree of the form "(root [lsub] [rsub])"
     * with a period "." for each empty tree
     * for example, the tree with root 1 and two empty subtrees
     * should be written "(1 . .)"
     * note that the FILE* argument can be stdout or stderr */
    if (t == NULL) {
        // Base case: empty tree
        fprintf(f,".");
    } else {
        // Recursive case: nonempty tree
        fprintf(f,"(%d ",t->root);
        itree_write(f, t->lsub);
        fprintf(f," ");
        itree_write(f, t->rsub);
        fprintf(f,")");
    }
}

itree *itree_dup(itree *t)
{
    /* allocate a whole new tree, with the same numbers
     * in the same positions (another "copy constructor") */
    
    // Base case: empty tree
    if (t == NULL) {
        return NULL;
    }
    
    // Recursive case: nonempty tree
    itree *lsub2 = itree_dup(t->lsub);
    itree *rsub2 = itree_dup(t->rsub);
    itree *t2 = itree_new(t->root, lsub2, rsub2);    
    return t2;
}

int itree_sum(itree *t)
{
    /* compute sum of all ints in tree */
    // Base case: empty tree
    if (t == NULL) {
        return 0;
    }
    // Recursive case: non-empty tree
    return t->root + itree_sum(t->lsub) + itree_sum(t->rsub);
}

unsigned int itree_size(itree *t)
{
    /* compute number of nodes in tree */
    // Base case: empty tree
    if (t == NULL) {
        return 0;
    }
    // Recursive case: non-empty tree
    return 1 + itree_size(t->lsub) + itree_size(t->rsub);
}

unsigned int itree_height(itree *t)
{
    /* the height of the empty tree is 0
     * the height of a nonempty tree is the length
     * of the longest path from root to leaf */
    // Base case: empty tree
    if (t == NULL) {
        return 0;
    }
    // Recursive case: non-empty tree
    unsigned int lsub_h = itree_height(t->lsub);
    unsigned int rsub_h = itree_height(t->rsub);
    return (rsub_h > lsub_h) ? rsub_h + 1 : lsub_h + 1;
}

/* === FUNCITONS WITH LISTS AND TREES === */

ilist *inorder(itree *t)
{
    /* build the list of all the ints in a tree, moving from left to right */
    // Base case: empty tree
    if (t == NULL) {
        return NULL;
    }
    // Recursive case: non-empty tree
    // Build lists for lsub and rsub, then join by attaching 
    // rsub to end of lsub
    ilist *lsubl = inorder(t->lsub);
    // Add root to beginning of right subtree
    ilist *rsubl = ilist_new(t->root, inorder(t->rsub));
    return ilist_join(lsubl, rsubl);
}

ilist *leaves(itree *t)
{
    /* build the list of all in the ints at leaf position
     * in the tree, moving from left to right */ 
    // Base case: empty tree
    if (t == NULL) {
        return NULL;
    }
    // Second base case: leaf node
    if (t->lsub == NULL && t->rsub == NULL) {
        return ilist_new(t->root, NULL);
    }
    // Recursive case: non-empty tree
    // Build lists for lsub and rsub, then join by attaching 
    // rsub to end of lsub
    ilist *lsubl = leaves(t->lsub);
    ilist *rsubl = leaves(t->rsub);
    return ilist_join(lsubl, rsubl);
}

/* === TESTING FUNCTIONS === */
void print_itree(FILE *f, itree *t) {
    fprintf(f,"Result: \"");
    itree_write(f,t);
    fprintf(f,"\"\n");
}

void print_ilist(FILE *f, ilist *ns,int nl) {
    fprintf(f,"Result: \"");
    ilist_write(f,ns,nl);
    fprintf(f,"\"\n");
}

int main()
{
    printf("--- Testing ilist functions ---\n");
    printf("(expecting nil) Printing NULL list with newline to stdout\n");
    print_ilist(stdout,NULL,1);
    ilist *ns1 = ilist_new(-4,NULL);
    printf("Printing -4:nil without newline to stdout\n");
    print_ilist(stdout,ns1,0);
    ilist *ns2 = ilist_new(0,NULL);
    printf("Printing 0:nil without newline to stdout\n");
    print_ilist(stdout,ns2,0);
    ilist_free(ns1);
    ilist_free(ns2);
    ilist *nums = ilist_new(1, ilist_new(3, ilist_new(5, NULL)));
    printf("Printing 1:3:5:nil without newline to stdout\n");
    print_ilist(stdout, nums, 0);
    printf("Printing 1:3:5:nil with newline to stdout\n");
    print_ilist(stdout, nums, 1);
    printf("Printing 1:3:5:nil with newline to stdout\n");
    print_ilist(stdout, nums, -1);
    printf("Printing 1:3:5:nil without newline to stderr\n");
    print_ilist(stderr, nums, 0);
    printf("Printing 1:3:5:nil with newline to stderr\n");
    print_ilist(stderr, nums, 1);
    printf("Now allocating copy of list, freeing original, and printng"
           " the copy with newline to stdout\n");
    ilist *numsc = ilist_dup(nums);
    ilist_free(nums);
    print_ilist(stdout, numsc, 1); 
    printf("(expect 9) Computing sum of list: %d\n", ilist_sum(numsc));
    printf("(expect 3) Computing len of list: %d\n",ilist_len(numsc));

    printf("Creating new ilist from array {3,4,5} and printing"
           " with newline to stdout\n");
    int arr[] = {3,4,5};
    ilist *nums2 = ilist_from_array(arr, 3);
    print_ilist(stdout,nums2, 1);
    printf("(expect 12) Computing sum of list: %d\n", ilist_sum(nums2));
    printf("(expect 3) Computing len of list: %d\n",ilist_len(nums2));
    printf("Now allocating copy of list, freeing original, and printng"
           " the copy with newline to stdout\n");
    ilist *numsc2 = ilist_dup(nums2);
    ilist_free(nums2);
    print_ilist(stdout, numsc2, 1); 

    printf("Creating new ilist from array {0} and printing"
           " with newline to stdout\n");
    int arr2[] = {0};
    ilist *nums3 = ilist_from_array(arr2, 1);
    print_ilist(stdout,nums3, 1);
    printf("(expect 0) Computing sum of list: %d\n", ilist_sum(nums3));
    printf("(expect 1) Computing len of list: %d\n",ilist_len(nums3));

    printf("Attaching prev 3 lists and printing with newline to stdout\n");
    printf("Expecting 1:3:5:3:4:5:0:nil\n");
    ilist_join(numsc2, nums3);
    ilist_join(numsc, numsc2);
    print_ilist(stdout,numsc,1);
    printf("Writing list to string and printing string\n");
    char *s = ilist_tos(numsc);
    printf("Result: %s\n",s);
    free(s);
    printf("(expect 21) Computing sum of list: %d\n", ilist_sum(numsc));
    printf("(expect 7) Computing len of list: %d\n",ilist_len(numsc));
    printf("(expect 2) Counting #even ints in list: %d\n",
            ilist_count(&is_even,numsc));
    printf("(expect 3) Counting #ints in list that are divisible by 3: %d\n",
            ilist_count(&is_div_3,numsc));
    printf("Finding even integer in list and printing list from there.\n");
    printf("Expecting 4:5:0:nil\n");
    ilist *even = ilist_find(&is_even, numsc);
    print_ilist(stdout,even,1);
    printf("Finding int in list that is div by 3 and printing.\n");
    printf("Expecting 3:5:3:4:5:0:nil\n");
    ilist *div3 = ilist_find(&is_div_3, numsc);
    print_ilist(stdout,div3,1);
   ilist_free(numsc); 

    printf("Making new list -1:-2:2:1:nil from array and printing\n");
    int arr3[] = {-1,-2,2,1};
    nums = ilist_from_array(arr3,4);
    print_ilist(stdout,nums,1);
    printf("Writing list to string and printing string\n");
    s = ilist_tos(nums);                  
    printf("Result: %s\n",s);
    free(s);
    printf("(expect 0) Computing sum of list: %d\n", ilist_sum(nums));        
    printf("(expect 4) Computing len of list: %d\n",ilist_len(nums));
    ilist_free(nums);

    printf("Making new list 5:7:17:31:nil from array and printing\n");
    int arr4[] = {5, 7, 17, 31};
    nums = ilist_from_array(arr4, 4);
    print_ilist(stdout, nums, 1);
    printf("(expect 60) Computing sum of list: %d\n", ilist_sum(nums));        
    printf("(expect 4) Computing len of list: %d\n",ilist_len(nums));
    printf("(expect 0) Counting #even ints in list: %d\n",
            ilist_count(&is_even,nums));
    printf("(expect 0) Counting #ints in list that are divisible by 3: %d\n",
            ilist_count(&is_div_3,nums));
    ilist_free(nums); 
    printf("Creating new ilist from array NULL and printing"
           " with newline to stdout\n");
    ilist *nums4 = ilist_from_array(NULL, 0);
    print_ilist(stdout,nums4, 1);
    printf("Writing list to string and printing string\n");      
    char *s2 = ilist_tos(nums4);
    printf("Result: %s\n",s2);
    free(s2);
    printf("(expect 0) Computing sum of list: %d\n", ilist_sum(nums4));
    printf("(expect 0) Computing len of list: %d\n",ilist_len(nums4));
    ilist_free(nums4);

    printf("Making new list 0:2147483647:-2147483647:nil from array"
           " and printing\n");
    int arr5[] = {0, 2147483647, -2147483647};
    nums = ilist_from_array(arr5,3);
    print_ilist(stdout,nums,0);
    printf("Writing list to string and printing\n");
    char *s3 = ilist_tos(nums);
    printf("Result: %s\n",s3);
    free(s3);
    printf("(expect 0) Computing sum of list: %d\n", ilist_sum(nums));
    printf("(expect 3) Computing length of list: %d\n",ilist_len(nums));
    ilist_free(nums);

    printf("Testing equality of lists {1} and {0}\n");
    ilist *n1 = ilist_new(1,NULL);
    ilist *m1 = ilist_new(0,NULL);
    printf("(expect %d) Result: %d\n",0,ilist_eq(n1,m1));

    printf("Testing equality of lists {1} and {1}\n");
    ilist_free(m1);
    m1 = ilist_new(1,NULL);
    printf("(expect %d) Result: %d\n",1,ilist_eq(n1,m1));

    printf("Testing equality of lists {1} and {1,2}\n");
    ilist_free(m1);
    m1 = ilist_new(1,ilist_new(2,NULL));
    printf("(expect %d) Result: %d\n",0,ilist_eq(n1,m1));
    
    printf("Testing equality of lists {1} and {-1,2}\n");
    ilist_free(m1);
    m1 = ilist_new(-1,ilist_new(2,NULL));
    printf("(expect %d) Result: %d\n",0,ilist_eq(n1,m1));
    
    printf("Testing equality of lists {3,4,5} and {3}\n");
    ilist_free(n1);
    n1 = ilist_new(3,ilist_new(4,ilist_new(5,NULL)));
    ilist_free(m1);
    m1 = ilist_new(3,NULL);
    printf("(expect %d) Result: %d\n",0,ilist_eq(n1,m1));
    
    printf("Testing equality of lists {3,4,5} and {3,4}\n");
    ilist_free(m1);
    m1 = ilist_new(3,ilist_new(4,NULL));
    printf("(expect %d) Result: %d\n",0,ilist_eq(n1,m1));
    
    printf("Testing equality of lists {3,4,5} and {3,4,5}\n");
    ilist_free(m1);
    m1 = ilist_new(3,ilist_new(4,ilist_new(5,NULL)));
    printf("(expect %d) Result: %d\n",1,ilist_eq(n1,m1));

    printf("Testing equality of lists {3,4,5} and {3,4,4}\n");
    ilist_free(m1);
    m1 = ilist_new(3,ilist_new(4,ilist_new(4,NULL)));
    printf("(expect %d) Result: %d\n",0,ilist_eq(n1,m1));
    
    printf("Testing equality of lists {3,4,5} and {3,4,5,6}\n");
    ilist_free(m1);
    m1 = ilist_new(3,ilist_new(4,ilist_new(4,ilist_new(6,NULL))));
    printf("(expect %d) Result: %d\n",0,ilist_eq(n1,m1));
   
    ilist_free(m1);
    ilist_free(n1);

    printf("Running ilist_build(0, &sqr) and printing list\n");
    ilist *sqrsn = ilist_build(0, &sqr);
    ilist_write(stdout,sqrsn,1);
    ilist_free(sqrsn);
    
    printf("Running ilist_build(1, &dbl) and printing list\n");
    ilist *dbls1 = ilist_build(1, &dbl);
    ilist_write(stdout, dbls1,1);
    printf("Writing to string\n");
    char *s1 = ilist_tos(dbls1);
    printf("Result: %s\n",s1);
    ilist_free(dbls1);

    printf("Running ilist_build(4, &sqr) and printing list\n");
    ilist *sqrs = ilist_build(4, &sqr);
    ilist_write(stdout,sqrs,1);
    printf("Writing to string\n");
    char *str2 = ilist_tos(sqrs);
    printf("Result: %s\n",str2);
    ilist_free(sqrs);
    
    printf("Running ilist_build(7, &dbl) and printing list\n");
    ilist *dbls = ilist_build(7, &dbl);
    ilist_write(stdout, dbls,1);
    printf("Writing to string\n");
    char *str3 = ilist_tos(dbls);
    printf("Result: %s\n",str3);
    ilist_free(dbls);
    free(str3); free(str2); free(s1);

    int sp = 100000001;
    printf("Testing long list from %d to %d\n",INT_MIN, INT_MIN + 100 * sp);
    int arr_l[101];
    for (int i = 0; i < 101; i++) {
        arr_l[i] = INT_MIN + i * sp;
    }
    ilist *longlst = ilist_from_array(arr_l, 101);
    char *llst = ilist_tos(longlst);
    printf("Result: %s\n",llst);    
    free(llst);
    ilist_free(longlst);

    printf("--- Testing itree functions ---\n");
    printf("(expect .) Printing empty tree to stdout.\n");
    print_itree(stdout,NULL);
    printf("(expect 0) computing sum of empty tree: %d\n",
            itree_sum(NULL));
    printf("(expect 0) computing #nodes in empty tree: %d\n",
            itree_size(NULL));
    printf("(expect 0) computing height of empty tree: %d\n",
            itree_height(NULL));
    printf("Building list of ints in tree in order and printing list, "
           "expecting nil\n");
    ilist *nstn = inorder(NULL);
    print_ilist(stdout,nstn,0);
    printf("Building list of ints at leaf positions in tree and printing "
           "list, expecting nil\n");
    ilist *ns_leafsn = leaves(NULL);
    print_ilist(stdout,ns_leafsn,0);
    ilist_free(nstn);
    ilist_free(ns_leafsn);

    itree *t = itree_new(1, NULL, NULL);
    printf("Printing tree (1 . .) to stdout and stderr\n");
    print_itree(stdout,t);
    print_itree(stderr,t);
    printf("(expect 1) Computing sum of tree: %d\n", itree_sum(t));
    printf("(expect 1) Counting #nodes in tree: %d\n",itree_size(t));
    printf("(expect 1) computing height of tree: %d\n",itree_height(t));
    printf("Building list of ints in tree in order and printing list, "
           "expecting 1:nil\n");
    ilist *nst1 = inorder(t);
    print_ilist(stdout,nst1,0);
    ilist_free(nst1);
    printf("Building list of ints at leaf positions in tree and printing "
           "list, expecting 1:nil\n");
    ilist *ns_leafs1 = leaves(t);
    print_ilist(stdout,ns_leafs1,0);
    ilist_free(ns_leafs1);

    printf("Allocating copy of tree, freeing original, and printing copy\n");
    itree *tc = itree_dup(t);
    itree_free(t);
    print_itree(stdout,tc);
    itree_free(tc);

    t = itree_new(5, itree_new(6,NULL,NULL), itree_new(7,NULL,NULL));
    printf("Printing tree (5 (6 . .) (7 . .)) to stdout and stderr\n");
    print_itree(stdout,t);
    print_itree(stderr,t);
    printf("(expect 18) Computing sum of tree: %d\n", itree_sum(t));
    printf("(expect 3) Counting #nodes in tree: %d\n",itree_size(t));
    printf("(expect 2) computing height of tree: %d\n",itree_height(t));
    printf("Allocating copy of tree, freeing original, and printing copy\n");
    itree *tc2 = itree_dup(t);
    itree_free(t);
    print_itree(stdout,tc2);
    itree_free(tc2);
    printf("Writing empty tree: ");
    print_itree(stdout,NULL);

    t = itree_new(5, itree_new(6,NULL,NULL), 
                  itree_new(3,NULL,itree_new(7,NULL,NULL)));
    printf("Printing tree (5 (6 . .) (3 . (7 . .))) to stdout and stderr\n");
    print_itree(stdout,t);
    print_itree(stderr,t);
    printf("(expect 21) Computing sum of tree: %d\n", itree_sum(t));
    printf("(expect 4) Counting #nodes in tree: %d\n",itree_size(t));
    printf("(expect 3) computing height of tree: %d\n",itree_height(t));
    printf("Allocating copy of tree, freeing original, and printing copy\n");
    itree *tc3 = itree_dup(t);
    itree_free(t);
    print_itree(stdout,tc3);
    printf("Building list of ints in tree in order and printing list, "
           "expecting 6:5:3:7:nil\n");
    ilist *nst = inorder(tc3);
    print_ilist(stdout,nst,0);
    printf("Building list of ints at leaf positions in tree and printing "
           "list, expecting 6:7:nil\n");
    ilist *ns_leafs = leaves(tc3);
    print_ilist(stdout,ns_leafs,0);
    itree_free(tc3);
    ilist_free(ns_leafs);
    ilist_free(nst);

    // Special indentation to see tree structure clearly
    t = itree_new(15, 
                itree_new(60, 
                    itree_new(0,NULL,NULL),
                    itree_new(17,NULL,NULL)),
                itree_new(-1,
                    itree_new(3,NULL,NULL),
                    itree_new(4,NULL,NULL)));
    printf("Printing tree (15 (60 (0 . .) (17 . .)) (-1 (3 . .) (4 . .)))"
           " to stdout\n");
    print_itree(stdout,t);
    printf("(expect 3) computing height of tree: %d\n",itree_height(t));
    printf("Building list of ints in tree in order and printing list, "
           "expecting 0:60:17:15:3:-1:4:nil\n");
    nst = inorder(t);
    print_ilist(stdout,nst,0);
    printf("Building list of ints at leaf positions in tree and printing "
           "list, expecitng 0:17:3:4:nil\n");
    ns_leafs = leaves(t);
    print_ilist(stdout,ns_leafs,0);
    itree_free(t);
    ilist_free(ns_leafs);
    ilist_free(nst);

    return 0;
}
