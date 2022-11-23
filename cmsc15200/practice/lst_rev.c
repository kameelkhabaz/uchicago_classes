#include <stdio.h>
#include <stdlib.h>


struct ilist {
    int first;
    struct ilist *rest;
};

typedef struct ilist ilist;

ilist *ilist_new(int first, ilist *rest)
{
    ilist *lst = (ilist *)malloc(sizeof(ilist));
    if (lst == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    lst->first = first;
    lst->rest = rest;
    return lst;
}

void ilist_show(ilist *lst)
{
    while (lst != NULL) {
        printf("%d: ",lst->first);
        lst = lst->rest;
    }
    
    printf("nil\n");
}

ilist *reverse(ilist *lst)
{
    if (lst == NULL || lst->rest == NULL) {
        return lst;
    }

    ilist *rest = lst;
    ilist *rev = lst->rest;
    lst->rest = NULL; // DONT FORGET
    
    while (rev != NULL) {
        ilist *temp = rev;
        rev = temp->rest;
        temp->rest = rest;
        rest = temp;
    }
    return rest;
}

ilist *reverse_rec(ilist *lst)
{
    if (lst == NULL || lst->rest == NULL) {
        // Need BOTH conditions
        return lst;
    }
    
    ilist *rev = reverse_rec(lst->rest);
    ilist *curr = rev;
    while (curr->rest != NULL) {
        curr = curr->rest;
    }
    curr->rest = lst;
    lst->rest = NULL; // MUST REMEMBER
    return rev;
}


ilist *XOR_sorted(ilist *lst1, ilist *lst2)
{
    // make new list of elems in 1 of 2 lists
    // both lists must be sorted
    if (lst1 == NULL) {
        return lst2;
    } else if (lst2 == NULL) {
        return lst1;
    }
    
    ilist *XOR_lst = NULL;
    ilist *curr = NULL;
    
    while (lst1 != NULL || lst2 != NULL) {
        int val;
        if (lst1 == NULL || lst2->first < lst1->first) {
            val = lst2->first;
            lst2 = lst2->rest;
        } else if (lst2 == NULL || lst1->first < lst2->first) {
            val = lst1->first;
            lst1 = lst1->rest;
        } else if (lst1->first == lst2->first) {
            lst1 = lst1->rest;
            lst2 = lst2->rest;
            continue;
        }
        
        if (XOR_lst == NULL) {
            XOR_lst = ilist_new(val, NULL);
            curr = XOR_lst;
        } else {
            curr->rest = ilist_new(val, NULL);
            curr = curr->rest;
        }
    }
    return XOR_lst;
}

ilist *intersect(ilist *lst1, ilist *lst2)
{
    if (lst1 == NULL) {
        return lst2;
    } else if (lst2 == NULL) {
        return lst1;
    }
    
    ilist *intersect = NULL;
    ilist *curr = NULL;
    while (lst1 != NULL && lst2 != NULL) {
        if (lst1->first == lst2->first) {
            if (intersect == NULL) {
                intersect = ilist_new(lst1->first, NULL);
                curr = intersect;
            } else {
                curr->rest = ilist_new(lst1->first, NULL);
                curr = curr->rest;
            }
            lst1 = lst1->rest;
            lst2 = lst2->rest;
        } else if (lst1->first < lst2->first) {
            lst1 = lst1->rest; //just advance 1 ptr
       } else {
            lst2 = lst2->rest;
        }
    }
    return intersect;
}

    
    
int main()
{
    ilist *lst = ilist_new(1, ilist_new(2, ilist_new(3, ilist_new(4, 
                 ilist_new(5,NULL)))));
    ilist_show(lst);
    lst = reverse(lst);
    ilist_show(lst);
    lst = reverse_rec(lst);
    printf("lst1\n");
    ilist_show(lst);
    ilist *lst2 = ilist_new(0, ilist_new(1, ilist_new(2, ilist_new(6, 
                            ilist_new(7,NULL)))));
    printf("lst2\n");
    ilist_show(lst2);
    printf("Doing XOR of lst1 and lst2\n");
    ilist *XOR_lst = XOR_sorted(lst,lst2);
    ilist_show(XOR_lst);
    printf("Doing AND of lst1 and lst2\n");
    ilist *AND_lst = intersect(lst,lst2);
    ilist_show(AND_lst);
    return 0;
}
