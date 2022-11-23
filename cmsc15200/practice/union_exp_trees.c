#include <stdio.h>
#include <stdlib.h>

typedef struct tagged_exp exp;

enum exp_tag {CONSTANT, OPER};

// a constant is an unsigned int

struct oper_tree {
  char o; // '+', '*', '^'
  exp *lsub;
  exp *rsub;
};

union constant_or_oper {
  unsigned int constant;
  struct oper_tree oper;
};

struct tagged_exp {
  enum exp_tag tag;
  union constant_or_oper tree;
};

exp *exp_const_new(unsigned int k)
{
  exp *e = (exp *)malloc(sizeof(exp));
  if (e==NULL) {
    fprintf(stderr,"exp_const_new: malloc failed\n");
    exit(1);
  }  
  e->tag = CONSTANT;
  e->tree.constant = k;
  return e;
}

exp *exp_oper_new(char o, exp *lsub, exp *rsub)
{
  exp *e = (exp *)malloc(sizeof(exp));
  if (e==NULL) {
    fprintf(stderr,"exp_oper_new: malloc failed\n");
    exit(1);
  }
  e->tag = OPER;
  e->tree.oper.o = o;
  e->tree.oper.lsub = lsub;
  e->tree.oper.rsub = rsub;
  return e;
}

void exp_show(exp *e)
{
  switch (e->tag) {
    case CONSTANT:
      printf("%u",e->tree.constant);
      return; // print %u for unsigned int dont forget 
    case OPER:
        printf("(");
        exp_show(e->tree.oper.lsub);
        printf(" %c ", e->tree.oper.o);
        exp_show(e->tree.oper.rsub);
        printf(")");
        break;
    default:
        fprintf(stderr,"Invalid operation\n");
        exit(1);
  }
}
unsigned int mypow(unsigned int a, unsigned int b)
{   
    // raise a to power b
    unsigned int res = 1;
    for (int i = 0; i < b; i++) {
        res *= a;
    }
    return res;
}

unsigned int eval(exp *e)
{
    switch (e->tag) {
        case CONSTANT:
            return e->tree.constant;
        case OPER:
        {
            char op = e->tree.oper.o;
            unsigned int left = eval(e->tree.oper.lsub);
            unsigned int right = eval(e->tree.oper.rsub);
            switch (op) {
                case '+': return left + right;
                case '*': return left * right;
                case '^': return mypow(left,right);
                default: 
                    fprintf(stderr,"invalid operation %c\n",op);
                    exit(1);
            }
        }
        default: 
            fprintf(stderr,"invalid tag\n");
            exit(1);
    }
}

int main()
{
  exp *x = exp_oper_new('*', 
                exp_oper_new('+',exp_const_new(3), exp_const_new(5)), 
                exp_oper_new('^',exp_const_new(2), exp_const_new(1)));
  exp_show(x);
    printf("\nevaluation is %d\n",eval(x));
  return 0;
}
