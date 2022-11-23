/* === Data definitions === */

typedef struct strlist strlist;

struct strlist {
    char *word;
    strlist *next;
};

/* === Operations === */

// Create a new heap-allocated strlist element:
//  - Each element stores a heap-allocated copy of w
//  - And a pointer to the next strlist n
strlist *strlist_new(char *w, strlist *n);

// Free the entire strlist and each word in the list
void strlist_free(strlist *lst);

// Join 2 lists
strlist *strlist_join(strlist *l1, strlist *l2);

// Write a list to a file (any readable format is acceptable)
void strlist_write(FILE *f, strlist *lst);

// Read words from a file into a strlist
// - Assume that each line of the file is a separate word
// - Do not store the newline character (Hint: Use fscanf)
strlist *strlist_from_file(FILE *f);

// Compute the length of a strlist
unsigned int strlist_len(strlist *lst);

// Count the number of words that pass the test
unsigned int strlist_count(int(*test)(char*), strlist *lst);
