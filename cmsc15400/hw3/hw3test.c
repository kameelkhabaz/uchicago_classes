#include <stdio.h>

    long loop(long x, long n) {
        long result = 0;
        long mask;
        for (mask = 3; mask != 0; mask = mask << n) {
            result = result | (mask & x);
        }
        return result; 
    }


long funcQ(long x, long y) { 
    long result = 3 * y;
    result = result + x; 
    return result;
}

long funcP(long r, long s, long t) {
    if (s <= 0) {
        return funcQ(s, r) + t;
    } else {
        return funcQ(t, s) + r;
    }
}

int main() {
    printf("loop(%d,%d) = %d\n",1,2,loop(1,2));    
    printf("loop(%d,%d) = %d\n",3,2,loop(3,2));    
    printf("loop(%d,%d) = %d\n",7,2,loop(7,2));    
    printf("loop(%d,%d) = %d\n",7,4,loop(7,4));    
    printf("loop(%d,%d) = %d\n",5,2,loop(5,2));    
    printf("funcQ(%d,%d) = %d\n",1,1,funcQ(1,1));
    printf("funcQ(%d,%d) = %d\n",2,1,funcQ(2,1));
    printf("funcQ(%d,%d) = %d\n",1,2,funcQ(1,2));
    printf("funcP(%d,%d,%d) = %d\n",1,1,1,funcP(1,1,1));
    printf("funcP(%d,%d,%d) = %d\n",1,-1,1,funcP(1,-1,1));
    printf("funcP(%d,%d,%d) = %d\n",2,1,1,funcP(2,1,1));
    printf("funcP(%d,%d,%d) = %d\n",2,-1,1,funcP(2,-1,1));
    return 0;
}
