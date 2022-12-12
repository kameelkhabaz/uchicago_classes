#include <stdio.h>

int q2D(int x) {
    return !!(x & (0xF << ((sizeof(int) - 1) * 8 + 4)));
}
short q2Ds(short x) {
    return !!(x & (0xF << ((sizeof(short) - 1) * 8 + 4)));
}

int main() {
    int foo;
    char endian;
    foo = 1;
    endian = (char *) &foo == 0 ? 1 : 0;
    printf("Hello world: %s\n", (char *) &foo);
    printf("hello world, from a %s-endian machine\n",
    endian ? "big" : "little"); 


    // Q2 testing 
    int x1 = 0xFFFFFFFF;
    int x2 = 0x00000000;
    int x3 = 0x00001000;
    int x4 = 0xFFFFEFFF;
    int x5 = 0x80000000;
    printf("testing at least one bit of %x is 1: %d\n",x1,!!x1);
    printf("testing at least one bit of %x is 1: %d\n",x2,!!x2);
    printf("testing at least one bit of %x is 1: %d\n",x3,!!x3);
    printf("testing at least one bit of %x is 1: %d\n",x4,!!x4);
    printf("testing at least one bit of %x is 1: %d\n",x5,!!x5);

    printf("testing at least one bit of %x is 0: %d\n",x1,!!(~x1));
    printf("testing at least one bit of %x is 0: %d\n",x2,!!(~x2));
    printf("testing at least one bit of %x is 0: %d\n",x3,!!(~x3));
    printf("testing at least one bit of %x is 0: %d\n",x4,!!(~x4));
    printf("testing at least one bit of %x is 0: %d\n",x5,!!(~x5));
    
    int a1 = 0xFF0000;
    int a2 = 0x000100;
    int a5 = 0x008000;
    int a6 = 0x010000;
    int a3 = 0x00FFFF;
    int a4 = 0x00FFEF;  
    int a7 = 0x00EFFF;  
    int a8 = 0x00FFFF;  
    int a9 = 0x00FFFE;  
    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 1: %d\n",a1, !!(a1 & 0xFFFF));
    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 1: %d\n",a2, !!(a2 & 0xFFFF));
    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 1: %d\n",a3, !!(a3 & 0xFFFF));
    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 1: %d\n",a5, !!(a5 & 0xFFFF));
    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 1: %d\n",a6, !!(a6 & 0xFFFF));

    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 0: %d\n",a1, !!((~a1) & 0xFFFF) );
    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 0: %d\n",a2, !!((~a2) & 0xFFFF));
    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 0: %d\n",a3, !!((~a3) & 0xFFFF));
    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 0: %d\n",a5, !!((~a5) & 0xFFFF));
    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 0: %d\n",a6, !!((~a6) & 0xFFFF));
    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 0: %d\n",a7, !!((~a7) & 0xFFFF));
    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 0: %d\n",a9, !!((~a9) & 0xFFFF));
    printf("testing at least 1 bit in least sig 2 bytes of %.8x is 0: %d\n",a8, !!((~a8) & 0xFFFF));

    int o1 = 0xFFFFFFFF;
    int o2 = 0x77777777;
    int o4 = 0x00000080;
    int o3 = 0x55555555;
    int o5 = 0xAAAAAAAA;
    printf("testing any odd bit of %.8x is 1: %d\n",o1, !!(o1 & 0xAAAAAAAA));
    printf("testing any odd bit of %.8x is 1: %d\n",o2, !!(o2 & 0xAAAAAAAA));
    printf("testing any odd bit of %.8x is 1: %d\n",o3, !!(o3 & 0xAAAAAAAA));
    printf("testing any odd bit of %.8x is 1: %d\n",o4, !!(o4 & 0xAAAAAAAA));
    printf("testing any odd bit of %.8x is 1: %d\n",o5, !!(o5 & 0xAAAAAAAA));

    int n1 = 0xFFFFFFFF;
    int n2 = 0xF0000000;
    int n3 = 0x7FFFFFFF;
    int n4 = 0x10000000;
    int n5 = 0x0FFFFFFF;
    short n6 = 0x1000;
    short n7 = 0x0800;
    printf("testing any bits in most sig %.8x is 1: %d\n",n1,q2D(n1));
    printf("testing any bits in most sig %.8x is 1: %d\n",n2,q2D(n2));
    printf("testing any bits in most sig %.8x is 1: %d\n",n3,q2D(n3));
    printf("testing any bits in most sig %.8x is 1: %d\n",n4,q2D(n4));
    printf("testing any bits in most sig %.8x is 1: %d\n",n5,q2D(n5));
    printf("testing any bits in most sig %.4x is 1: %d\n",n6,q2Ds(n6));
    printf("testing any bits in most sig %.4x is 1: %d\n",n7,q2Ds(n7));
    return 0;

}

