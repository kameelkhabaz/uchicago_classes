/*
 * Kameel Khabaz
 * Homework 1
 */

#include <stdio.h>
#include <math.h>

// Problem 1
unsigned int gcd(unsigned int a, unsigned int b)
{
    /* Compute greatest common divisor */
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

// Problem 2
double area_pipe(double inner_radius, double length, double thickness)
{
    /* Compute surface area of the pipe */
    double inner_area = 2 * M_PI * inner_radius * length;
    double outer_area = 2 * M_PI * (inner_radius + thickness) * length;
    double base_area = M_PI * pow(inner_radius + thickness, 2) - 
                       M_PI * pow(inner_radius, 2);
    return inner_area + outer_area + 2 * base_area;
}

// Problem 3
int check_leap_year (unsigned int y)
{
    /* Check if leap year */
    return (y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0));
}

int dow(unsigned int m, unsigned int d, unsigned int y)
{
    unsigned int j;
    switch (m) {
        case 1: 
            if (check_leap_year(y)) {
                j = 0;
            } else {
                j = 1;
            }
            break;
        case 2:
            if (check_leap_year(y)) {
                j = 3;
            } else {
                j = 4;
            }
            break;
        case 3:
            j = 4;
            break;
        case 4: 
            j = 0;
            break;
        case 5:
            j = 2;
            break;
        case 6:
            j = 5;
            break;
        case 7:
            j = 0;
            break;
        case 8:
            j = 3;
            break;
        case 9:
            j = 6;
            break;
        case 10:
            j = 1;
            break;
        case 11:
            j = 4;
            break;
        case 12:
            j = 6;
            break;
    }
    unsigned int n = (y - 1900) + j + d + y/4;
    return n % 7;
}

// Problem 4
unsigned int digit_sum(unsigned int n)
{
    /* Compute the sum of the digits in a number. */
    if (n==0) {
        return 0;
    } else {
        return n % 10 + digit_sum(n / 10);
    }
}

unsigned long int comp_luhn_sum(unsigned long int n, int s)
{
    /* Compute luhn sum for a number, given a starting subscript */
    // base case
    if (n == 0) {
        return 0;
    }
    // recursive case
    unsigned int next_digit = n % 10;
    unsigned int value_add;
    if (s % 2 == 0) {
        value_add = next_digit;
    } else {
        value_add = digit_sum(2 * next_digit);
    }
    return value_add + comp_luhn_sum(n / 10, s + 1);
}

int luhn(unsigned long int n)
{
    /* Check if the number is a luhn number */
    unsigned long int luhn_sum = comp_luhn_sum(n / 10, 1);   
    unsigned int cand_check_digit = 10 - (luhn_sum % 10);
    unsigned int check_digit = n % 10;
    return check_digit == cand_check_digit;
}

int main()
{
    // test problem 1
    printf("(expecting 7) gcd(7,77) is %d.\n", gcd(7,77)); 
    printf("(expecting 7) gcd(77,7) is %d.\n", gcd(77,7));
    printf("(expecting 10) gcd(0,10) is %d.\n", gcd(0,10));
    printf("(expecting 10) gcd(10,0) is %d.\n", gcd(10,0));
    printf("(expecting 1) gcd(3,5) is %d.\n", gcd(3,5));
    printf("(expecting 1) gcd(5,3) is %d.\n", gcd(5,3));
    printf("(expecting 97) gcd(1843,2231) is %d.\n", gcd(1843,2231));
    printf("(expecting 97) gcd(2231,1843) is %d.\n", gcd(2231,1843));
    // test problem 2
    printf("(expecting 207.3) area_pipe(1,10,1) is %f.\n", 
            area_pipe(1.0,10.0,1.0));
    printf("(expecting 2638.9) area_pipe(1,1,19) is %f.\n", 
            area_pipe(1.0,1.0,19.0));
    printf("(expecting 100.5) area_pipe(1.5,3,1) is %f.\n", 
            area_pipe(1.5,3.0,1.0));
    printf("(expecting 65.97) area_pipe(0.5,0.5,2.5) is %f.\n", 
            area_pipe(0.5,0.5,2.5));
    // test problem 3
    printf("(expecting 4) dow(1,14,2021) is %d.\n", dow(1,14,2021));
    printf("(expecting 0) dow(5,2,1999) is %d.\n", dow(5,2,1999));
    printf("(expecting 6) dow(12,22,1900) is %d.\n", dow(12,22,1900));
    printf("(expecting 6) dow(1,8,2000) is %d.\n", dow(1,8,2000));
    printf("(expecting 4) dow(1,14,1999) is %d.\n", dow(1,14,1999));
    printf("(expecting 1) dow(2,12,1996) is %d.\n", dow(2,12,1996));
    printf("(expecting 0) dow(2,4,1900) is %d.\n", dow(2,4,1900));
    printf("(expecting 3) dow(3,3,2021) is %d.\n", dow(3,3,2021));
    printf("(expecting 6) dow(4,5,2053) is %d.\n", dow(4,5,2053));
    printf("(expecting 2) dow(5,1,2001) is %d.\n", dow(5,1,2001));
    printf("(expecting 5) dow(6,15,2001) is %d.\n", dow(6,15,2001));
    printf("(expecting 0) dow(7,11,2004) is %d.\n", dow(7,11,2004));
    printf("(expecting 4) dow(8,12,1976) is %d.\n", dow(8,12,1976));
    printf("(expecting 5) dow(9,10,1943) is %d.\n", dow(9,10,1943));
    printf("(expecting 6) dow(10,23,2010) is %d.\n", dow(10,23,2010));
    printf("(expecting 2) dow(11,10,2015) is %d.\n", dow(11,10,2015));
    printf("(expecting 3) dow(12,4,1963) is %d.\n", dow(12,4,1963));
    // test problem 4
    printf("(expecting nonzero) luhn(18929) is %d.\n", luhn(18929));
    printf("(expecting 0) luhn(18928) is %d.\n", luhn(18928));
    printf("(expecting 0) luhn(283848384) is %d.\n", luhn(283848384));
    printf("(expecting 0) luhn(19294) is %d.\n", luhn(19294));
    printf("(expecting 0) luhn(2320) is %d.\n", luhn(2320));
    printf("(expecting nonzero) luhn(520954421) is %d.\n", luhn(520954421));
    printf("(expecting nonzero) luhn(34) is %d.\n", luhn(34));
    return 0;
}
