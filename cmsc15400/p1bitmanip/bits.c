/* 
 * CS:APP Data Lab 
 * 
 * Name: Kameel Khabaz, CNETID: kkhabaz
 * 
 * bits.c - Source file with your solutions to the project.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the project by
editing the collection of functions in this source file.

CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function.

/*
 * STEP 2: Modify the following functions according to the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the btest checker to verify that your solutions produce
 *      the correct answers.
 */


#endif

/************************************************ 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  /* If num is positive, use value and if negative, take the negative
   * and then add one to get the absolute value. Uses properties of two's
   * complement and arithmetic right shifting. */
  int is_signed = x >> 31; 
  // is_signed will be all 1s if signed, all 0s if unsigned
  int neg = ~x + 1;
  return (~is_signed & x) | (is_signed & neg);
}

/************************************************ 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  /* If both nums same sign, check if adding them switches sign, which 
   * indicates overflow. If opp sign, no overflow is possible. */
  int x_sign =  x >> 31; // 1 if x is negative, 0 otherwise
  int y_sign = y >> 31; // 1 if y is negative, 0 otherwise 
  int eq_sign = !(x_sign ^ y_sign); // 1 if signs are equal, 0 otherwise 
  int sum = x + y;
  int sum_sign = sum >> 31; // 1 if sum is negative, 0 otherwise
  int eq_sum_sign = !(x_sign ^ sum_sign); // 1 if signs are equal, 0 otherwise
  // addOK if nums are opposite signs or if the sum is the same sign
  return (!eq_sign) | (eq_sum_sign); 

}

/************************************************ 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
    /* Save the even-numbered bits. When using xor with all evens, it will 
     * return all 0s only when all even-numbered bits are 1 (when all evens
     * and even digits match). */ 
    int allEven = 0x55 + (0x55 << 8) + (0x55 << 16) + (0x55 << 24); 
    int x_evens = x & allEven; // even-numbered bits of x
    return !(x_evens ^ allEven);
}

/************************************************
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  /* If num negative, return 0. Else, check if x + INT_MAX has same sign as x. 
   * If so, then num is 0. Else, num is negative. Take advantage of twos 
   * complement. */ 
  int MAX = ~0 ^ (0x80 << 24); // get max (positive) int value
  int sign_x = x >> 31; // 1s if negative
  int sign_x_plus_max = (x + MAX) >> 31; // 1s if negative
  int same_sign = (sign_x ^ sign_x_plus_max) ^ 1; // 1 if same sign 

  // only time this returns 1 is when same sign is exactly 1 (only positive 
  // integer, neg int check separately)
  return (sign_x ^ 1) & (~sign_x & same_sign);
}

/************************************************
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  /* The approach is to count the number of ones in each two-bit piece, then
   * in each 4 bit piece, then 8 bit, then 16 bit, then 32 bit */
  // The constants are used to isolate the bits in pieces. c1 isolates 1 bit 
  // to calc 1s in a 2 bit piece. c2 isolates 2 bits to calc 1s in 4 bit piece.
  // and so on...
  int c1, c2, c4, c8, c32, r2, r4, r8, r16, r32;
  c1 = 0x55 + (0x55 << 8);
  c1 = c1 + (c1 << 16);
  c2 = 0x33 + (0x33 << 8) + (0x33 << 16) + (0x33 << 24);
  c4 = 0x0F + (0x0F << 8) + (0x0F << 16) + (0x0F << 24);
  c8 = 0xFF + (0xFF << 16);
  c32 = 0xFF + (0xFF << 8);
  r2 = (x & c1) + ((x >> 1) & c1);
  r4 = (r2 & c2) + ((r2 >> 2) & c2);
  r8 = (r4 & c4) + ((r4 >> 4) & c4);
  r16 = (r8 & c8) + ((r8 >> 8) & c8);
  r32 = (r16 & c32) + ((r16 >> 16) & c32);
  return r32;
}

/************************************************
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
  /* We know that NOT (A or B) is equal to (NOT A) & (NOT B) */
  return (~x) & (~y);
}

/************************************************
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
  /* Extract the stuff to be swapped, clear those bits, then perform the 
   * swap. */
  int bitsn = n << 3;
  int bitsm = m << 3;
  int temp1 = ((x >> bitsn) & 0xFF) << bitsm;
  int temp2 = ((x >> bitsm) & 0xFF) << bitsn;
  //printf("temp1: %.8X, temp2: %.8X \n",temp1,temp2);
  int x_clear1 = x & ~(0xFF << bitsn);
  //printf("x_clear1: %.8X\n",x_clear1);
  int x_clear2 = x_clear1 & ~(0xFF << bitsm);
  //printf("x_clear2: %.8X\n", x_clear2);
  return (x_clear2 | temp1) | temp2;
}

/************************************************ 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
   /* See if x is zero, then perform conditional */
   int eq_zero = !(x ^ 0); // is 1 if 0, 0 if nonzero
   eq_zero = ~eq_zero + 1; // all 1s if 0, all 0s if nonzero
   return ((~eq_zero) & y) | (eq_zero & z); 
}

/************************************************
 * ezThreeFourths - multiplies by 3/4 rounding toward 0,
 *   Should exactly duplicate effect of C expression (x*3/4),
 *   including overflow behavior.
 *   Examples: ezThreeFourths(11) = 8
 *             ezThreeFourths(-9) = -6
 *             ezThreeFourths(1073741824) = -268435456 (overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 */
int ezThreeFourths(int x) {
  /* Perform each operation, must check remainder to do the rounding correctly
   * for negative numbers */ 
  int times_2 = x << 1;
  int times_3 = times_2 + x;
  int div_4 = times_3 >> 2;
  // must check remainder. If there is remainder and num is negative, must add 
  // 1 to round to 0
  int remainder = !(!(times_3 & 3)); 
  // remainder: 0 if no remainder, 1 otherwise
  int times_3_sign = times_3 >> 31; // 1 if negative

  // applying the rule above, num_add = 1 if there is num to add, 0 if not
  int num_add = remainder & times_3_sign;

  return div_4 + num_add;
}

/************************************************ 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {        
    /* Check if x is less than the max (if positive) or has a magnitude less 
     * than the max + 1 (if negative) */ 
    int neg_one = ~0;
    int max = (1 << (n + neg_one)) + neg_one; 
    // max possible number with n bits 2s comp
    int sign = x >> 31; // 1s if neg
    // if positive, check max - x >= 0
    // if negative, check  max + x + 1 >= 0; 
    int neg_x = ~x + 1;
    // find right quantity to put in addition
    int quant_add = ((~sign) & neg_x) | (sign & (x + 1));
    int check = max + quant_add; 
    return !(check >> 31); 

}

/************************************************
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  /* Bit shift */ 
  int bitsn = n << 3; // multiply by 8                                              
  return (x >> bitsn) & 0xFF;    
}

/************************************************ 
 * greatestBitPos - return a mask that marks the position of the
 *               most significant 1 bit. If x == 0, return 0
 *   Example: greatestBitPos(96) = 0x40
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 70
 *   Rating: 4 
 */
int greatestBitPos(int x) {
/* Fill a mask of all 1s for all bits smaller than the sign bit, then add 1 
 * to it to isolate the largest bit. If x is neg, return the sign bit. If 
 * x is 0, return 0 */
    int x2 = (x >> 1);
    int x3 = ((x2 >> 2) | x2) | (x2 >> 1);
    int x4 = ((x3 >> 4) | x3) | (x3 >> 1);
    int x5 = ((x4 >> 8) | x4) | (x4 >> 1);
    int x6 = ((x5 >> 16) | x5) | (x5 >> 1);
    int sign_x = x >> 1; // 1s if negative 
    int neg_case = sign_x & (1 << 31);
    int pos_case = ~sign_x & (x6 + 1);
    return x & (neg_case | pos_case);
 }   

/************************************************
 * implication - return x -> y in propositional logic - 0 for false, 1
 * for true
 *   Example: implication(1,1) = 1
 *            implication(1,0) = 0
 *   Legal ops: ! ~ ^ |
 *   Max ops: 5
 *   Rating: 2
 */
int implication(int x, int y) {
  /* x -> y is FALSE only if x is 1 and y is 0 */
  return !x | (!(x ^ y));
}

/************************************************ 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /* Check if positive and if 0x39 - x >= 0 and x - 0x30 >= 0 */
  int pos = !(x >> 31); // 1 if x is pos
  int s1 = x + ~(0x30) + 1; // 0x39 - x
  int s2 = 0x39 + ~(x) + 1; // x - 0x30
  int sign1 = !(s1 >> 31); 
  int sign2 = !(s2 >> 31);
  return (pos & sign1) & sign2;
}

/************************************************ 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
  /* Check equality using XOR */
  return !(x ^ y);
}

/************************************************
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {          
    /* Divide the problem into the following logic:
     * If y is Tmin, return 0.
     * If x is Tmin, return 1;
     * If signs are different, then 
     *      if x is negative, return 1;
     *      if y is negative, return 0;
     * Calculate x - y to check if x - y < 0
     * If x - y < 0, return 1. Else, return 0. */ 

    // Check if any num is Tmin. Important because performing ops on Tmin
    // will result in overflow
    int Tmin = 1 << 31;
    int y_is_min = !(y ^ Tmin);
    int x_is_min = !(x ^ Tmin);

    // Check diff signs 
    int x_sign = x >> 31;
    int x_is_neg = !!x_sign;
    int y_sign = y >> 31;
    int same_sign = !(x_sign ^ y_sign);
    int diff_signs_cond =  ((!same_sign) & x_is_neg);

    // if same sign, return 1 only if x - y < 0
    int y_neg = (~y) + 1;
    int diff = x + y_neg;
    int diff_sign = diff >> 31; // return 1s if negative, 0 if nonneg
    int same_sign_cond = (same_sign & diff_sign);
    return (!y_is_min) & (x_is_min | diff_signs_cond | same_sign_cond);
}

/************************************************
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int isNonNegative(int x) {
  /* Check if non-negative */
  int sign = !(x >> 31); 
  return sign;
}

/************************************************
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
  /* x is a power of 2 if it has only 1 non-zero bit. A power of 2 minus 1
   * has all 1s with a 0 for the bit that is 1 in the power of 2. Anding both
   * will be all 0 only for power of 2. */ 
  int x_nonzero = !!x; // check x is nonzero
  int xm1 = x + (~0);
  int xsign = !(x >> 31); // return 1 if x is positive, 0 if negative
  int x_and_xm1 = xm1 & x;
  return (x_nonzero & xsign) & (!x_and_xm1); 
}

/************************************************
 * isTmin - returns 1 if x is the minimum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmin(int x) {
  /* if x - 1 = -1 (due to overflow) and x is not 0, then tmin  */
  int add_neg_one = x + (~0); // if Tmin, then this is max pos num
  int sum = add_neg_one + x; // if Tmin, will be -1;
  return (!!x) & !(sum ^ (~0));
}

/************************************************
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
  /* minus one in 2's complement has all 0s */
  return ~0;
}

/************************************************
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
    /* Save the bits that overflow, move them to right. Then rotate whole thing
     * and add the bits. Need special case when n = 32 (will need to add 1 if 
     * it is there) */ 
    int neg_one = ~0;
    int eq32 = !(n ^ 32);
    int larger2 = ((1 << (n + neg_one + 1)));
    // making a mask of all 1s for right n bits so can isolate that part
    int right_mask = larger2 + neg_one;
    int saved_bits = (x >> (33 + (~n))) & right_mask;
    int rotated = (x << n) + saved_bits;
    return (~eq32 & rotated) + (eq32 & 1);
}

/************************************************
 * satMul2 - multiplies by 2, saturating to Tmin or Tmax if overflow
 *   Examples: satMul2(0x30000000) = 0x60000000
 *             satMul2(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             satMul2(0x60000000) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int satMul2(int x) {
    /* Perform multiplication, check for overflow. If overflow, then return 
     * Tmin, but if x is also positive, then add -1 to Tmin to get Tmax in
     * 2's complement */ 
    int prod = x << 1;
    int sign_x = x >> 31;
    int sign_prod = prod >> 31;
    int same_sign = ~(sign_x ^ sign_prod);
    int valid_mult_result = (same_sign & prod);
    int Tmin = (1 << 31);
    // to get Tmax, add -1 to Tmin if not same sign (overflow) and if x is pos
    int should_sub1 = (~same_sign & ~sign_x);
    return (valid_mult_result | (~same_sign & Tmin)) + (should_sub1 & (~0));
}

