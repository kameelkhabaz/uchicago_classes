#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Kameel Khabaz
 * CMSC 15200
 * Homework 4
 * Feb 3 2022 */

struct fraction {
  int num;
  int denom;
};

typedef struct fraction frac;

int gcd(unsigned int a, unsigned int b)
{
    /* Compute greatest common divisor, I did this in hw 1 */

    if (a<b) {
        // if first number is smaller than second, switch the number
        return gcd(b,a);
    }
    if (b==0) {
        return a;
    } else {
        return gcd(b, a%b);
    }
}

void frac_reduce(frac *f)
{
    /* Reduce a fraction so that, for example, 4/8 becomes 1/2 */
    int greatest_common_divisor = gcd(abs(f->num),abs(f->denom));
    f->num = (f->num) / greatest_common_divisor;
    f->denom = (f->denom) / greatest_common_divisor;
}

void frac_free(frac *f)
{
    /* Free the fraction off of the heap */
    free(f);
}

void frac_show(frac *f)
{
    /* Print out the fraction  */
    printf("Fraction is %d/%d\n", f->num, f->denom);
} 

frac *frac_new(int num, int denom)
{
    /* Construct a fraction struct
     * standardize the fraction according to these rules:
     *     - reduce it, so, for example, 4/8 becomes 1/2
     *     - represent any 0/d as 0/1
     *     - represent any negative becomes -n/d
     * furthermore, raise an error is denom is 0
     */
        
    if (denom == 0) {
        // dividing by 0 
        fprintf(stderr,"Invalid fraction: denominator is 0\n");
        exit(1);
    } else if (num == 0) {
        // number is 0, represent as 0/1
        denom = 1;
    } else if (denom < 0) {
        // denom is either < 0 or both num and denom are < 0
        // same operation in either case
        denom *= -1;
        num *= -1;
    } 
    // Create fraction and check allocation
    frac *f = (frac *)malloc(sizeof(frac));
    if (f == NULL) {
        fprintf(stderr,"frac: allocation has failed.\n");
        exit(1);
    }
    
    f->num = num;
    f->denom = denom;

    // Reduce fraction 
    frac_reduce(f);

    return f;

}

frac *frac_add(frac *q, frac *r)
{
    /* add q and r, return new heap-allocated fraction */
    
    int new_num = q->num * r->denom + q->denom * r->num;
    int new_denom = q->denom * r->denom;

    return frac_new(new_num, new_denom);
}

frac *frac_mult(frac *q, frac *r)
{
    /* multiply q and r, return new heap-allocated fraction */

    int new_num = q->num * r->num;
    int new_denom = q->denom * r->denom;

    return frac_new(new_num, new_denom);
}

frac *frac_raise(frac *r, unsigned int power)
{
    /* raise r to power, return new heap-allocated fraction */
    // Everything to 0th power is 1
    if (power == 0) {
        return frac_new(1,1);
    }

    int new_num = pow(r->num,power);
    int new_denom = pow(r->denom,power);

    return frac_new(new_num, new_denom); 
}

int frac_eq(frac *q, frac *r)
{
    /* compare q and r for numerical equality */
    return (q->num == r->num) && (q->denom == r->denom);
}

char *frac_tos(frac *r)
{
    /* build a new heap-allocated string of the form "2/5" */

    char c[128] = {0};
    sprintf(c, "%d/%d", r->num, r->denom);
    return strdup(c);
}

int main()
{
    // Test frac_new
    int i = 4, j = 5;
    printf("Making and showing %d/%d as standardized form 4/5\n",i,j);
    frac *f = frac_new(i,j);
    frac_show(f);
    frac_free(f);

    i = 4, j = 8;
    printf("Making and showing %d/%d as standardized form 1/2\n",i,j);
    f = frac_new(i,j);
    frac_show(f);
    frac_free(f);

    i = -4, j = -8;
    printf("Making and showing %d/%d as standardized form 1/2\n",i,j);
    f = frac_new(i,j);
    frac_show(f);
    frac_free(f);


    i = -10234, j = -20468;
    printf("Making and showing %d/%d as standardized form 1/2\n",i,j);
    f = frac_new(i,j);
    frac_show(f);
    frac_free(f);

    i = -1, j = 3;
    printf("Making and showing %d/%d as standardized form -1/3\n",i,j);
    f = frac_new(i,j);
    frac_show(f);
    frac_free(f);

    i = 1, j = -3;
    printf("Making and showing %d/%d as standardized form -1/3\n",i,j);
    f = frac_new(i,j);
    frac_show(f);
    frac_free(f);

    i = 3, j = -2;
    printf("Making and showing %d/%d as standardized form -3/2\n",i,j);
    f = frac_new(i,j);
    frac_show(f);
    frac_free(f);

    i = -3*333*9, j = 2*333*9;
    printf("Making and showing %d/%d as standardized form -3/2\n",i,j);
    f = frac_new(i,j);
    frac_show(f);
    frac_free(f);

    i = 3*333*9, j = -2*333*9;
    printf("Making and showing %d/%d as standardized form -3/2\n",i,j);
    f = frac_new(i,j);
    frac_show(f);
    frac_free(f);

    i = -3*333*9, j = -2*333*9;
    printf("Making and showing %d/%d as standardized form 3/2\n",i,j);
    f = frac_new(i,j);
    frac_show(f);
    frac_free(f);

    i = -3*1571*9, j = -2*1571*9;
    printf("Making and showing %d/%d as standardized form 3/2\n",i,j);
    f = frac_new(i,j);
    frac_show(f);
    frac_free(f);

    i = 1571, j = 1571;
    printf("Making and showing %d/%d as standardized form 1/1\n",i,j);
    f = frac_new(i,j);
    frac_show(f);
    frac_free(f);

    i = -1522, j = -1572;
    printf("Making and showing %d/%d as standardized form 761/786\n",i,j);
    f = frac_new(i,j);
    frac_show(f);
    frac_free(f);

    // Test frac add 
    i = 1, j = 9;
    int l = 2, m = 9;
    f = frac_new(i,j);
    frac *g = frac_new(l, m);
    printf("Make fracs and doing %d/%d +  %d/%d, expecting 1/3\n",i,j,l,m);
    frac *h = frac_add(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);
    
    i = 1, j = 9, l = 8, m = 9;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d +  %d/%d, expecting 1/1\n",i,j,l,m);
    h = frac_add(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = 0, j = 9, l = 0, m = 4;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d +  %d/%d, expecting 0/1\n",i,j,l,m);
    h = frac_add(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = -1, j = 9, l = -8, m = 9;
    i = -1, j = 9, l = -8, m = 9;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d +  %d/%d, expecting -1/1\n",i,j,l,m);
    h = frac_add(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = 5, j = 9, l = -5, m = 9;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d +  %d/%d, expecting 0/1\n",i,j,l,m);
    h = frac_add(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = 5, j = 3, l = -3, m = 2;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d +  %d/%d, expecting 1/6\n",i,j,l,m);
    h = frac_add(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = -5, j = 3, l = 3, m = 2;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d +  %d/%d, expecting -1/6\n",i,j,l,m);
    h = frac_add(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = 5, j = -3, l = 3, m = 2;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d +  %d/%d, expecting -1/6\n",i,j,l,m);
    h = frac_add(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = 5, j = -3, l = 3, m = -2;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d +  %d/%d, expecting -19/6\n",i,j,l,m);
    h = frac_add(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = -5, j = -3, l = -3, m = -2;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d +  %d/%d, expecting 19/6\n",i,j,l,m);
    h = frac_add(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = 3, j = -3, l = 6, m = -2;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d +  %d/%d, expecting -4/1\n",i,j,l,m);
    h = frac_add(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);
    
    i = 4, j = -8, l = 9, m = -18;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d +  %d/%d, expecting -1/1\n",i,j,l,m);
    h = frac_add(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);
    
    // Test frac_mult
    i = 4, j = -8, l = 9, m = -18;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d * %d/%d, expecting 1/4\n",i,j,l,m);
    h = frac_mult(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = 3, j = -3, l = 6, m = -2;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d * %d/%d, expecting 3/1\n",i,j,l,m);
    h = frac_mult(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = 3, j = 3, l = 6, m = -2;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d * %d/%d, expecting -3/1\n",i,j,l,m);
    h = frac_mult(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = 0, j = 3, l = 6, m = -2;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d * %d/%d, expecting 0/1\n",i,j,l,m);
    h = frac_mult(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = 393, j = 2431, l = 913, m = 2485;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d * %d/%d, expecting 32619/549185\n",
            i,j,l,m);
    h = frac_mult(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = -393, j = -2431, l = -913, m = -2485;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d * %d/%d, expecting 32619/549185\n",
            i,j,l,m);
    h = frac_mult(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = 395, j = 2432, l = 912, m = 2485;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d * %d/%d, expecting 237/3976\n",
            i,j,l,m);
    h = frac_mult(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = -395, j = 2432, l = 912, m = -2485;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d * %d/%d, expecting 237/3976\n",
            i,j,l,m);
    h = frac_mult(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    i = 395, j = -2432, l = 912, m = 2485;
    f = frac_new(i,j);
    g = frac_new(l, m);
    printf("Make fracs and doing %d/%d * %d/%d, expecting -237/3976\n",
            i,j,l,m);
    h = frac_mult(f,g);
    frac_show(h);
    frac_free(f);
    frac_free(g);
    frac_free(h);

    // Test frac_raise 
    i = 3, j = 7;
    unsigned int power = 0;
    printf("Making and doing (%d/%d)^%d, expecting 1/1\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = 3, j = 7, power = 1;
    printf("Making and doing (%d/%d)^%d, expecting 3/7\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -3, j = 7, power = 1;
    printf("Making and doing (%d/%d)^%d, expecting -3/7\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = 0, j = 7, power = 0;
    printf("Making and doing (%d/%d)^%d, expecting 1/1\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -3, j = 7, power = 0;
    printf("Making and doing (%d/%d)^%d, expecting 1/1\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = 3, j = -7, power = 1;
    printf("Making and doing (%d/%d)^%d, expecting -3/7\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -3, j = -7, power = 1;
    printf("Making and doing (%d/%d)^%d, expecting 3/7\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = 3, j = 7, power = 2;
    printf("Making and doing (%d/%d)^%d, expecting 9/49\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -3, j = 7, power = 2;
    printf("Making and doing (%d/%d)^%d, expecting 9/49\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = 3, j = -7, power = 2;
    printf("Making and doing (%d/%d)^%d, expecting 9/49\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -3, j = -7, power = 2;
    printf("Making and doing (%d/%d)^%d, expecting 9/49\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = 3, j = 7, power = 3;
    printf("Making and doing (%d/%d)^%d, expecting 27/343\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -3, j = 7, power = 3;
    printf("Making and doing (%d/%d)^%d, expecting -27/343\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = 3, j = -7, power = 3;
    printf("Making and doing (%d/%d)^%d, expecting -27/343\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -3, j = -7, power = 3;
    printf("Making and doing (%d/%d)^%d, expecting 27/343\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -7, j = -2, power = 1;
    printf("Making and doing (%d/%d)^%d, expecting 7/2\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -7, j = 2, power = 1;
    printf("Making and doing (%d/%d)^%d, expecting -7/2\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -5, j = 2, power = 2;
    printf("Making and doing (%d/%d)^%d, expecting 25/4\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -5, j = 2, power = 3;
    printf("Making and doing (%d/%d)^%d, expecting -125/8\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = 5, j = -2, power = 3;
    printf("Making and doing (%d/%d)^%d, expecting -125/8\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -5, j = -2, power = 3;
    printf("Making and doing (%d/%d)^%d, expecting 125/8\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = 4, j = 16, power = 4;
    printf("Making and doing (%d/%d)^%d, expecting 1/256\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    i = -5, j = 2, power = 4;
    printf("Making and doing (%d/%d)^%d, expecting 625/16\n",i,j,power);
    f = frac_new(i,j);
    g = frac_raise(f, power);
    frac_show(g);
    frac_free(f);
    frac_free(g);

    // Test fract equality
    i = 1, j = 3, l = 1, m = 3;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 1) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = -1, j = 3, l = 1, m = -3;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 1) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = -1, j = -3, l = -1, m = -3;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 1) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = 2*222*22*3, j = -2*222*22*3, l = 1, m = -1;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 1) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = -2*222*22*3, j = -2*222*22*3, l = 1, m = -1;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 0) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = 2, j = 3, l = 1, m = 3;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 0) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = -1, j = 3, l = 1, m = 3;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 0) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = -1, j = -3, l = 1, m = -3;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 0) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = 2, j = 9, l = 1, m = 3;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 0) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = 1, j = 9, l = 1, m = 3;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 0) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = 1, j = 3, l = 3, m = 9;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 1) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);


    i = 0, j = -1, l = 0, m = 95;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 1) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = 4, j = 8, l = 1, m = 2;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 1) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = 4, j = 8, l = 16, m = 32;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 1) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    i = 4, j = 8, l = 16, m = 33;
    f = frac_new(i,j);
    g = frac_new(l,m);
    printf("Making fractions %d/%d and %d/%d and testing equality\n",i,j,l,m);
    printf("(expecting 0) got %d\n",frac_eq(f,g)); 
    frac_free(f);
    frac_free(g);

    // testing frac_tos
    i = 4, j = 8;
    f = frac_new(i,j);
    char *s = frac_tos(f);
    printf("Making %d/%d and printing to string: %s\n",i,j,s);
    free(s);
    frac_free(f);
        
    i = -4, j = 8;
    f = frac_new(i,j);
    s = frac_tos(f);
    printf("Making %d/%d and printing to string: %s\n",i,j,s);
    free(s);
    frac_free(f);

    i = 4, j = -8;
    f = frac_new(i,j);
    s = frac_tos(f);
    printf("Making %d/%d and printing to string: %s\n",i,j,s);
    free(s);
    frac_free(f);

    i = -4, j = -8;
    f = frac_new(i,j);
    s = frac_tos(f);
    printf("Making %d/%d and printing to string: %s\n",i,j,s);
    free(s);
    frac_free(f);

    i = -25, j = -25;
    f = frac_new(i,j);
    s = frac_tos(f);
    printf("Making %d/%d and printing to string: %s\n",i,j,s);
    free(s);
    frac_free(f);

    i = -25, j = -15;
    f = frac_new(i,j);
    s = frac_tos(f);
    printf("Making %d/%d and printing to string: %s\n",i,j,s);
    free(s);
    frac_free(f);

    i = -25, j = 5;
    f = frac_new(i,j);
    s = frac_tos(f);
    printf("Making %d/%d and printing to string: %s\n",i,j,s);
    free(s);
    frac_free(f);

    //Testing frac_new with d = 0
    i = 4, j = 0;
    printf("Making and showing %d/%d, expecting error\n",i,j);
    printf("NOTE: I commented out the command here so that "
           "the program does not exit with an error.\n");
    //f = frac_new(i,j);
    //frac_show(f);
    //frac_free(f);
    return 0;
}
