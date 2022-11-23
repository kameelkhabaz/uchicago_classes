#include <stdio.h>
#include <limits.h>

/* Kameel Khabaz
 * Lab 3
 * Jan 25 2022
 */

int num_digits(int num) 
{
    /* Return the number of digits in the number. */
    unsigned int ndigits = 1;
    num /= 10;
    while (num) {
        ndigits++;
        num /= 10;
    }
    return ndigits;
}

int minimum(int nums[], int len) 
{
    /* Return the minimum value in the array. */
    int min_val = INT_MAX;
    for (int i = 0; i < len; i++) {
        if (nums[i] < min_val) {
            min_val = nums[i];
        }
    }
    return min_val;
}


void swap(int *a, int *b) 
{
    /* Swap the values of the two integer pointers */
    int c = *b;
    *b = *a;
    *a = c;
}

void count_digits(int digits[], int num)
{
    /* Count the digits of the number and add count to digits array.*/
    int digit;
    int num_left = num;
    for (int i = 0; i < num_digits(num); i++) {
        digit = num_left % 10;
        num_left /= 10;
        digits[digit] += 1;
    }
}

int most_frequent_digit(int nums[], int len, int *freq) 
{
    /* Return the most frequent digit and set the output parameter 
     * to the frequency. */
    int digits[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < len; i++) {
        count_digits(digits,nums[i]);
    }
    int max_freq = 0;
    int most_freq_digit;
    for (int i = 0; i < 10; i++) {
        if (digits[i] > max_freq) {
            max_freq = digits[i];
            most_freq_digit = i;
        }
    }
    *freq = max_freq;
    return most_freq_digit;
}    
    
int main() 
{
    // Testing while loop
    printf("(expecting 1) num_digits(%d) = %d\n",1,num_digits(1));   
    printf("(expecting 2) num_digits(%d) = %d\n",10,num_digits(10));   
    printf("(expecting 1) num_digits(%d) = %d\n",0,num_digits(0));   
    printf("(expecting 4) num_digits(%d) = %d\n",9394,num_digits(9394));   
    printf("(expecting 2) num_digits(%d) = %d\n",50,num_digits(50));   
    printf("(expecting 3) num_digits(%d) = %d\n",434,num_digits(434));   
    printf("(expecting 7) num_digits(%d) = %d\n",1000000,num_digits(1000000));   
    // Testing for loop
    int arr[] = {10, 20, 30};
    printf("(expecting 10) minimum({10, 20, 30} , 3) = %d\n",minimum(arr, 3));
    int arr2[] = {-1,-2,-3};
    printf("(expecting -3) minimum({-1, -2, -3} , 3) = %d\n",minimum(arr2, 3));
    int arr3[] = {-1334};
    printf("(expecting -1334) minimum({-1334} , 1) = %d\n",minimum(arr3, 1));
    int arr4[] = {1001};
    printf("(expecting 1001) minimum({1001} , 1) = %d\n",minimum(arr4, 1));
    int arr5[] = {101,99,3};
    printf("(expecting 3) minimum({101,99,3} , 3) = %d\n",minimum(arr5, 3));
    int arr6[] = {-100,-4,4,3};
    printf("(expecting -100) minimum({-100,-4,4,3} , 4) = %d\n",
            minimum(arr6, 4));
    // Testing swap
    int x = 7;
    int y = 5;
    swap(&x, &y);
    printf("Swapping 7 and 5 and printing: %d, %d\n", x, y); 
    int x2 = 0;
    int y2 = 0;
    swap(&x2, &y2);
    printf("Swapping 0 and 0 and printing: %d, %d\n", x2, y2); 
    int x3 = -9;
    int y3 = -11;
    swap(&x3, &y3);
    printf("Swapping -9 and -11 and printing: %d, %d\n", x3, y3); 
    int z = -100;
    int a = 232;
    swap(&z, &a);
    printf("Swapping -100 and 232 and printing: %d, %d\n", z, a); 
    // Testing most frequent digit
    int nums[] = {14, 32, 44, 45, 211, 42};
    int f;
    int d = most_frequent_digit(nums, 6, &f);
    printf("Testing most_frequent_digit of {14, 32, 44, 45, 211, 42}\n");
    printf("(expect %d, %d) most frequent: %d, frequency: %d\n", 4, 5, d, f);
    int nums2[] = {0,10,230,30003,2002,5005};
    int f2;
    int d2 = most_frequent_digit(nums2, 6, &f2);
    printf("Testing most_frequent_digit of {0,10,230,30003,2002,5005}\n");
    printf("(expect %d, %d) most frequent: %d, frequency: %d\n", 
            0, 10, d2, f2); 
    int nums3[] = {99330777};
    int f3;
    int d3 = most_frequent_digit(nums3, 1, &f3);
    printf("Testing most_frequent_digit of {99330777}\n");
    printf("(expect %d, %d) most frequent: %d, frequency: %d\n", 7, 3, d3, f3); 
    int nums4[] = {97,99,309,929};
    int f4;
    int d4 = most_frequent_digit(nums4, 4, &f4);
    printf("Testing most_frequent_digit of {97,99,309,929}\n");
    printf("(expect %d, %d) most frequent: %d, frequency: %d\n", 9, 6, d4, f4); 
    int nums5[] = {121,212,999};
    int f5;
    int d5 = most_frequent_digit(nums5, 3, &f5);
    printf("Testing most_frequent_digit of {121,212,999}\n");
    printf("(expect %d, %d) most frequent: %d, frequency: %d\n", 1, 3, d5, f5); 
    int nums6[] = {1000,1101,2323,2332};
    int f6;
    int d6 = most_frequent_digit(nums6, 4, &f6);
    printf("Testing most_frequent_digit of {1000,1101,2323,2332}\n");
    printf("(expect %d, %d) most frequent: %d, frequency: %d\n", 0, 4, d6, f6); 
    return 0;
}

