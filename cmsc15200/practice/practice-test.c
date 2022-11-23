#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "practice.h"




int main()
{
    printf("Testing enums\n");
    
    struct rotate rotations[3];
    rotations[0].direction = CLOCKWISE;
    rotations[0].degrees = 60;
    rotations[1].direction = CLOCKWISE;
    rotations[1].degrees = 60;
    rotations[2].direction = CLOCKWISE;
    rotations[2].degrees = 240;

    struct rotate rotation2[3];
    rotation2[0].direction = COUNTERCLOCKWISE;
    rotation2[0].degrees = 60;
    rotation2[1].direction = COUNTERCLOCKWISE;
    rotation2[1].degrees = 60;
    rotation2[2].direction = COUNTERCLOCKWISE;
    rotation2[2].degrees = 240;

    struct rotate rotation3[2];
    rotation3[0].direction = COUNTERCLOCKWISE;
    rotation3[0].degrees = 90;
    rotation3[1].direction = CLOCKWISE;
    rotation3[1].degrees = 90;

    struct rotate rotation4[2];
    rotation4[0].direction = COUNTERCLOCKWISE;
    rotation4[0].degrees = 90;
    rotation4[1].direction = CLOCKWISE;
    rotation4[1].degrees = 89;

    printf("Rotation -60, -60, -240 is in same direction: expect 1 got %d\n",
            same_direction(rotations,3));

    printf("Rotation 60, 60, 240 is in same direction: expect 1 got %d\n",
            same_direction(rotation2,3));

    printf("Rotation 90, -90 is in same direction: expect 1 got %d\n",
            same_direction(rotation3,2));

    printf("Rotation 90, -89 is in same direction: expect 0 got %d\n",
            same_direction(rotation4,2));

    //  Unions 
    printf("Testing wallet\n");
    struct wallet *w = (struct wallet *)malloc(sizeof(struct wallet));
    if (w == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    printf("Making CNY wallet with 19 jiao and 1 yuan\n");
    w->denomination = CNY;
    w->money.cny.jiao = 19;
    w->money.cny.yuan = 1;
    print_contents(*w);
    printf("Trading in wallet got:\n");
    // MUST DEREFERENCE POINTER HERE, THE FUNCTION TAKES IN THE WHOLE STRUCT
    // AND MAKES COPY ON ITS STACK
    // Now I changed function to take in pointer - CAREFUL
    struct wallet w2 = trade_in_coins(w);
    print_contents(w2);
    printf("Making wallet with 250 pence and 5 GBP pounds\n");
    struct wallet w3;
    w3.denomination = GBP;
    w3.money.gbp.pence = 250;
    w3.money.gbp.pounds = 5;
    print_contents(w3);
    struct wallet w4 = trade_in_coins(&w3);
    print_contents(w4);
    
    printf("Making wallet with 100 cents USD\n");
    struct wallet w5;
    w5.denomination = USD;
    w5.money.usd.cents = 100;
    w5.money.usd.dollars = 0;
    print_contents(w5);
    w4 = trade_in_coins(&w5);
    print_contents(w4);

    // Tree
    inttree *t = inttree_new(5, inttree_new(3, inttree_new(2,NULL,NULL),
                                                inttree_new(4,NULL,NULL)),
                                 inttree_new(10, inttree_new(7,NULL,NULL),      
                                                 inttree_new(12,NULL,NULL)));
    inttree_show(t);
    printf("Printing tree breadth first\n");
    print_breadth_first(t);
    int n;
    printf("Testing preorder\n");
    printf("5 is in tree: %d (found in %d steps)\n",preorder(t,5,&n),n);
    printf("3 is in tree: %d (found in %d steps)\n",preorder(t,3,&n),n);
    printf("12 is in tree: %d (found in %d steps)\n",preorder(t,12,&n),n);
    printf("5 is in tree: %d (found in %d steps)\n",preorder(t,5,&n),n);
    printf("1 is in tree: %d (found in %d steps)\n",preorder(t,1,&n),n);
    printf("6 is in tree: %d (found in %d steps)\n",preorder(t,6,&n),n);
    printf("Testing preorder_a0\n");
    n = 0;
    printf("5 is in tree: %d (found in %d steps)\n",preorder_a0(t,5,&n),n);
    n = 0;
    printf("3 is in tree: %d (found in %d steps)\n",preorder_a0(t,3,&n),n);
    n = 0;
    printf("12 is in tree: %d (found in %d steps)\n",preorder_a0(t,12,&n),n);
    n = 0;
    printf("5 is in tree: %d (found in %d steps)\n",preorder_a0(t,5,&n),n);
    n = 0;
    printf("1 is in tree: %d (found in %d steps)\n",preorder_a0(t,1,&n),n);
    n = 0;
    printf("6 is in tree: %d (found in %d steps)\n",preorder_a0(t,6,&n),n);

    printf("Testing inorder\n");
    printf("5 is in tree: %d (found in %d steps)\n",inorder(t,5,&n),n);
    printf("3 is in tree: %d (found in %d steps)\n",inorder(t,3,&n),n);
    printf("12 is in tree: %d (found in %d steps)\n",inorder(t,12,&n),n);
    printf("5 is in tree: %d (found in %d steps)\n",inorder(t,5,&n),n);
    printf("1 is in tree: %d (found in %d steps)\n",inorder(t,1,&n),n);
    printf("6 is in tree: %d (found in %d steps)\n",inorder(t,6,&n),n);

    printf("Testing inorder_a0\n");
    n = 0;
    printf("5 is in tree: %d (found in %d steps)\n",inorder_a0(t,5,&n),n);
    n = 0;
    printf("3 is in tree: %d (found in %d steps)\n",inorder_a0(t,3,&n),n);
    n = 0;
    printf("12 is in tree: %d (found in %d steps)\n",inorder_a0(t,12,&n),n);
    n = 0;
    printf("5 is in tree: %d (found in %d steps)\n",inorder_a0(t,5,&n),n);
    n = 0;
    printf("1 is in tree: %d (found in %d steps)\n",inorder_a0(t,1,&n),n);
    n = 0;
    printf("6 is in tree: %d (found in %d steps)\n",inorder_a0(t,6,&n),n);

    printf("testing postorder\n");
    printf("5 is in tree: %d (found in %d steps)\n",postorder(t,5,&n),n);
    printf("3 is in tree: %d (found in %d steps)\n",postorder(t,3,&n),n);
    printf("7 is in tree: %d (found in %d steps)\n",postorder(t,7,&n),n);
    printf("2 is in tree: %d (found in %d steps)\n",postorder(t,2,&n),n);
    printf("0 is in tree: %d (found in %d steps)\n",postorder(t,0,&n),n);
    
    printf("testing postorder_a0\n");
    n = 0;
    printf("5 is in tree: %d (found in %d steps)\n",postorder_a0(t,5,&n),n);
    n = 0;
    printf("3 is in tree: %d (found in %d steps)\n",postorder_a0(t,3,&n),n);
    n = 0;
    printf("7 is in tree: %d (found in %d steps)\n",postorder_a0(t,7,&n),n);
    n = 0;
    printf("2 is in tree: %d (found in %d steps)\n",postorder_a0(t,2,&n),n);
    n = 0;
    printf("0 is in tree: %d (found in %d steps)\n",postorder_a0(t,0,&n),n);
    
    enum directions d1[] = {LEFT,RIGHT};
    printf("Path {L,R} valid: expect 1 got %d\n",valid_path(t,d1 ,2));
    enum directions d2[] = {LEFT};
    printf("Path {L} valid: expect 0 got %d\n",valid_path(t,d2,1));
    enum directions d3[] = {LEFT,RIGHT,RIGHT};
    printf("Path {L,R,R} valid: expect 0 got %d\n",
            valid_path_r(t,d3,3));
    enum directions d31[] = {RIGHT,LEFT};
    printf("Path {R,L} valid: expect 1 got %d\n",
            valid_path_r(t,d31,2));
    enum directions d4[] = {RIGHT,RIGHT,RIGHT};
    printf("Path {R,R,R} valid: expect 0 got %d\n",
            valid_path_r(t,d4,3));
    enum directions d5[] = {};
    printf("Path {} valid: expect 0 got %d\n",
            valid_path_r(t,d5,0));

    strlist *lst = strlist_new("b",strlist_new("d",strlist_new("e",
                                strlist_new("f",NULL))));
    strlist_show(lst);
    printf("Updating a for apple worked: %d\n",update(lst,"a","apple"));
    printf("Updating b for bapple worked: %d\n",update(lst,"b","bapple"));
    printf("Updating e for eagles worked: %d\n",update_r(lst,"e","eagles"));
    printf("Updating z for zebra worked: %d\n",update_r(lst,"z","zebra"));
    strlist_show(lst);

    printf("Inserting a and b and c and g and h\n");
    lst = insert(lst, "a");
    strlist_show(lst);
    lst = insert(lst, "g");
    strlist_show(lst);
    lst = insert(lst,"h");
    strlist_show(lst);
    lst = insert(lst,"c");
    strlist_show(lst);

    int pos;
    lst = insert_pos(lst,"i",&pos);
    printf("Inserted i in position %d\n",pos);
    strlist_show(lst);
    lst = insert_pos(lst,"b",&pos);
    printf("Inserted b in position %d\n",pos);
    strlist_show(lst);
    printf("Reversing list twice\n");
    lst = reverse(lst);
    strlist_show(lst);
    lst = reverse(lst);
    strlist_show(lst);
    printf("Sorting list\n");
    lst = sort(lst);
    strlist_show(lst);
    printf("Making new strlist b->c->d->e->h->a and sorting\n");
    strlist *lst2 = strlist_new("b",strlist_new("c",strlist_new("d",
                    strlist_new("e",strlist_new("h",strlist_new("a",NULL))))));
    strlist_show(lst2);
    lst2 = sort(lst2);
    strlist_show(lst2);

    
    char *a = "a";
    char *b = "b";    
    printf("My strcmp %s and %s: %d\n",a,b,my_strcmp(a,b));
    char *a2 = "aa";
    char *b2 = "aa";    
    printf("My strcmp %s and %s: %d\n",a2,b2,my_strcmp(a2,b2));
    char *a3 = "cc";
    printf("My strcmp %s and %s: %d\n",a3,b2,my_strcmp(a3,b2));

    // Hash tables
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
    htbl_show(stdout,tbl,1);

    int min_bucket;
    int max_bucket;
    printf("Load factor of table is %f\n",
            load_factor(tbl,&min_bucket,&max_bucket));
    printf("Min bucket is %d and max is %d\n",min_bucket,max_bucket);

    htbl_free(tbl);

    int **x = make_x(1);
    show_md(x,1);
    x = make_x(5);
    show_md(x,5);
    x = make_x(4);
    show_md(x,4);
    return 0;
    return 0;
}

