/* 
 * Kameel Khabaz
 * Homework 3
 * January 27 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Define a type for a year/month/day date
typedef uint32_t pdate;

pdate new_date(unsigned int m, unsigned int d, unsigned int y)
{
    /* Create a new packed date from the given month, day, year
     * - for month, 1 means Jan, 2 means Feb, etc.
     * - GIGO applies
     *   format is YYMD
     */
    return (y << 16) + (m << 8) + d;
}

void unpack(pdate pd, unsigned int *m, unsigned int *d, unsigned int *y) 
{
    /* Write month, day, and year vals in pd into out parameters m, d, y
     */
    *m = (pd >> 8) & 255;
    *d = pd & 255;
    *y = pd >> 16;
}

void show_date(pdate pd)
{
    /* print the date in DD-MM-YYYY format (decimal numbers)
     * for example, for the date Feb 1, 2022, print "01-02-2022"
     */
    unsigned int m, d, y;
    unpack(pd,&m, &d, &y); 
    printf("%02d-%02d-%d\n", d, m, y);
}

int check_leap_year (unsigned int y)
{
    /* Check if leap year 
     * I wrote this for hw2 
     */
    return (y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0));
}


int get_days_in_month(int m, int y) 
{
    /* Get the days of the inputted month for the inputted year.
     * I wrote this for hw2  
     * */
    switch (m) {
        case 1: 
        case 3: 
        case 5:
        case 7:
        case 8:
        case 10:
        case 12: return 31;
        case 2: return check_leap_year(y) ? 29 : 28;
        case 4: 
        case 6:
        case 9:
        case 11: return 30;
    }
    fprintf(stderr,"bad month %d.\n",m);
    exit(1); 
}


int valid_pdate(pdate pd)
{
    /* check if the date is *valid* 
     * Date must be between Jan 1 1900 and Dec 31 2099 inclusive
     * and also have a valid month and valid day
     * (i.e., is not the 14th month or the 99th day)
     * note: GIGO does *not* apply to this function
     */
    // First check date range
    pdate min_date = new_date(1,1,1900);
    pdate max_date = new_date(12,31,2099);
    if (pd < min_date || pd > max_date) {
        return 0;
    }
    // Check month 
    unsigned int m, d, y;
    unpack(pd, &m, &d, &y); 
    if (m < 1 || m > 12) {
        return 0;
    }
    // Check day
    return d >= 1 && d <= get_days_in_month(m, y);
}

pdate yesterday(pdate pd)
{
    /* return the day before the argument */
    unsigned int m, d, y;                                                       
    unpack(pd, &m, &d, &y);
    if (d == 1 && m == 1) {
        // Go to previous year
        return new_date(12, 31, y - 1);
    }
    if (d == 1) {
        // Go to previous month
        return new_date(m - 1, get_days_in_month(m - 1, y), y);
    }
    // Go to previous day
    return pd - 1;
}

pdate tomorrow(pdate pd)
{
    /* return the day after the argument */
    unsigned int m, d, y; 
    unpack(pd, &m, &d, &y);
    unsigned int days_in_month = get_days_in_month(m, y);
    if (d == days_in_month && m == 12) {
        // Go to next year
        return new_date(1, 1, y + 1);
    } 
    if (d == days_in_month) {
        // Go to next month
        return new_date(m + 1, 1, y);
    }
    // Go to next day
    return pd + 1;
}

unsigned int get_days_between(pdate pd1, pdate pd2)
{
    /* Get #days between 2 dates. pd1 must be chronologically before pd2, or
     * they can be the same date, but pd2 canNOT be before pd1 in time. 
     */
    unsigned int db = 0;
    pdate date_comp = pd1;
    // Iterate from pd1 until you reach pd2
    while (date_comp != pd2) {
        date_comp = tomorrow(date_comp);
        db++;
    }
    return db;
}

pdate *date_span(pdate pd1, pdate pd2, unsigned int *length)
{
    /*  return the array that starts at pd1 and continues until pd2, inclusive
     * be sure to return a heap-allocated array of pdates
     * also, report the length of the array in the out-parameter length
     * if pd2 is chronologically before pd1, return NULL and set length to 0
     */
    // Check if pd2 is before pd1
    if (pd2 < pd1) {
        *length = 0;
        return NULL;
    }
    // Make array of correct length
    int days_between = get_days_between(pd1,pd2);
    *length = days_between + 1;
    pdate *dates = (pdate *) malloc((*length) * sizeof(pdate));
    if (dates == NULL) {
        fprintf(stderr,"malloc has failed\n");
        exit(1);
    }
    // Put dates in array
    dates[0] = pd1;
    for (int i = 1; i < *length; i++) {
        dates[i] = tomorrow(dates[i - 1]);
    } 
    return dates;
}

void find_min_max_dates(pdate *dates, unsigned int *min_date_idx, 
                        unsigned int *max_date_idx, unsigned int start_idx, 
                        unsigned int end_idx) 
{
    /* Find the chronologically earliest and latest date in the 
     * array. Start looking at index start_idx and stop at the end_idx,
     * inclusive (so you can look at a subarray).
     * Function modifies following out parameters:
     *     max_date_idx - index of dates array with most recent date.
     *     min_date_idx - index of dates array with oldest date.
     */ 
    unsigned int min_i = start_idx;
    unsigned int max_i = start_idx;
    for (int i = start_idx + 1; i <= end_idx; i++) {     
        if (dates[i] < dates[min_i]) {    
            min_i = i;
        } else if (dates[max_i] < dates[i]) {  
            max_i = i;
        }
    }
    *min_date_idx = min_i;
    *max_date_idx = max_i;
}

unsigned int date_distance(pdate *dates, unsigned int length)
{
    /* the given array is in no particular order 
     * and not necessarily consecutive
     * count the days between the earliest and the latest day in the array
     * for ex., if the array contains Jan 2 2022, Jan 1 2022, and Jan 3 2022,
     * the answer is 2 -- 2 days between Jan 1 2022 and Jan 3 2022
     * another example: 
     *     if the array contains Feb 15 2022, Feb 1 2020, Mar 1 2025, 
     *     Jan 11 2024, and Jan 22 2021, the answer is 1855
     */
    unsigned int min_date_idx, max_date_idx;    
    find_min_max_dates(dates, &min_date_idx, &max_date_idx, 0, length - 1);
    return get_days_between(dates[min_date_idx], dates[max_date_idx]);
}

void sort_asc(pdate *dates, unsigned int length)
{
    /* arrange the dates in order from earliest to latest (past-to-future) 
     * write an in-place sorting algorithm that does no allocation
     */
    unsigned int min_date_idx, max_date_idx;
    pdate temp;    
    for (int i = 0; i < length; i++) {    
        find_min_max_dates(dates, &min_date_idx, &max_date_idx, i, length - 1);
        temp = dates[min_date_idx];
        dates[min_date_idx] = dates[i]; 
        dates[i] = temp;
    }
}

void test_array_funcs(pdate *dates, unsigned int len, unsigned int exp_dist)
{
    /* Test min/max date, date distance, and sort_asc functions for array
     */ 
    unsigned int min_date_idx, max_date_idx;    
    printf("Printing dates\n");
    for (int i = 0; i < len; i++) {show_date(dates[i]);}  
    printf("Printing hex values\n");
    for (int i = 0; i < len; i++) {
        printf("%08X\n",dates[i]);
    }
    find_min_max_dates(dates, &min_date_idx, &max_date_idx, 0, len - 1);
    printf("Min elem at index %d: \n", min_date_idx);
    show_date(dates[min_date_idx]);
    printf("Max elem at index %d: \n", max_date_idx);
    show_date(dates[max_date_idx]);
    printf("Testing date distance of array ");
    printf("expect: %d, got: %d\n",exp_dist, date_distance(dates, len));
    printf("Sorting array and printing dates\n");
    sort_asc(dates, len);
    for (int i = 0; i < len; i++) {show_date(dates[i]);}  
    printf("Testing date distance of array ");
    printf("expect: %d, got: %d\n",exp_dist, date_distance(dates, len));
    printf("Printing hex values\n");
    for (int i = 0; i < len; i++) {
        printf("%08X\n",dates[i]);
    }
}

void test_date_span(pdate pd1, pdate pd2, unsigned int exp_dist)
{
    /* Test date_span */
    printf("Testing date span\n");
    unsigned int length;
    pdate *dates = date_span(pd1, pd2, &length);
    printf("Array is length %d.\n",length);
    test_array_funcs(dates, length, exp_dist);
    free(dates);
}
int main()
{
    printf("Dates are printed in DD-MM-YYYY format\n");
    //Test show date
    printf("Test show date 0X07E60201 expect Feb 1 2022.\n");
    show_date(0X07E60201);
    unsigned int m,d,y;
    unpack(0X07E60201,&m,&d,&y);
    printf("Test unpack of 0X07E60201 expecting Feb 1 2022:\n");
    printf("Printing as m/d/y: %d/%d/%d\n",m,d,y);
    unpack(0X08300A0F,&m,&d,&y);
    printf("Test unpack of 0X08300A0F expecting Oct 15 2096:\n");
    printf("Printing as m/d/y: %d/%d/%d\n",m,d,y);
    //Test new_date
    printf("Testing new_date(1,1,1900) and printing date\n");
    show_date(new_date(1,1,1900));
    printf("Testing new_date(2,1,2065) and printing date\n");
    show_date(new_date(2,1,2065));
    printf("Testing new_date(4,5,2009) and printing date\n");
    show_date(new_date(4,5,2009));
    printf("Testing new_date(12,31,2099) and printing date\n");
    show_date(new_date(12,31,2099));
    // Test valid_pdate
    printf("(expect 0) Checking validty of 0XFFFFFFFF: %d\n", 
            valid_pdate(0XFFFFFFFF));
    printf("(expect 0) Checking validty of 0X00000000: %d\n", 
            valid_pdate(0X00000000));
    printf("(expect 0) Checking validty of 0X07E60000: %d\n", 
            valid_pdate(0X07E60000));
    printf("(expect 0) Checking validty of 0X076C0000: %d\n", 
            valid_pdate(0X076C0000));
    printf("(expect 0) Checking validty of 0X079C0D01: %d\n", 
            valid_pdate(0X079C0D01));
    printf("(expect 1) Checking validty of 0X079C0C01: %d\n", 
            valid_pdate(0X079C0C01));
    printf("(expect 0) Checking validty of Dec 31 1899: %d\n", 
            valid_pdate(new_date(12,31,1899)));
    printf("(expect 1) Checking validty of Jan 1 1900: %d\n", 
            valid_pdate(new_date(1,1,1900)));
    printf("(expect 1) Checking validty of Jan 2 1900: %d\n", 
            valid_pdate(new_date(1,2,1900)));
    printf("(expect 1) Checking validty of Feb 1 1900: %d\n", 
            valid_pdate(new_date(2,1,1900)));
    printf("(expect 1) Checking validty of Jan 1 2099: %d\n",
            valid_pdate(new_date(1,1,2099))); 
    printf("(expect 1) Checking validty of Dec 30 2099: %d\n",
            valid_pdate(new_date(12,30,2099))); 
    printf("(expect 1) Checking validty of Dec 31 2099: %d\n",
            valid_pdate(new_date(12,31,2099))); 
    printf("(expect 0) Checking validty of Dec 31 2100: %d\n",
            valid_pdate(new_date(12,31,2100))); 
    printf("(expect 0) Checking validty of Jan 1 2100: %d\n",
            valid_pdate(new_date(1,1,2100))); 
    printf("(expect 0) Checking validty of Jan 2 2100: %d\n",
            valid_pdate(new_date(1,2,2100))); 
    printf("(expect 1) Checking validty of April 5 2009: %d\n",
            valid_pdate(new_date(4,5,2009))); 
    printf("(expect 0) Checking validty of Feb 30 2000: %d\n", 
            valid_pdate(new_date(2,30,2000)));
    printf("(expect 0) Checking validty of June 40 2000: %d\n", 
            valid_pdate(new_date(6,40,2000)));
    printf("(expect 0) Checking validty of 13/1/1999: %d\n",
            valid_pdate(new_date(13,1,1999))); 
    printf("(expect 0) Checking validty of Feb 29 1900: %d\n",
            valid_pdate(new_date(2,29,1900))); 
    printf("(expect 1) Checking validty of Feb 29 2000: %d\n", 
            valid_pdate(new_date(2,29,2000)));
    printf("(expect 0) Checking validty of Sep 0 2099: %d\n", 
            valid_pdate(new_date(9,0,2099)));
    printf("(expect 0) Checking validty of Jun 31 2099: %d\n", 
            valid_pdate(new_date(6,31,2099)));
    printf("(expect 0) Checking validty of Nov 31 2099: %d\n", 
            valid_pdate(new_date(11,31,2099)));
    printf("(expect 1) Checking validty of Oct 31 2099: %d\n", 
            valid_pdate(new_date(10,31,2099)));
    // Test yesterday
    printf("Returning day before Jan 2 1900: ");
    show_date(yesterday(new_date(1,2,1900)));
    printf("Returning day before Jan 1 1909: ");
    show_date(yesterday(new_date(1,1,1909)));
    printf("Returning day before Jan 1 2009: ");
    show_date(yesterday(new_date(1,1,2009)));
    printf("Returning day before March 1 1900: ");
    show_date(yesterday(new_date(3,1,1900)));
    printf("Returning day before March 1 1904: ");
    show_date(yesterday(new_date(3,1,1904)));
    printf("Returning day before March 1 1999: ");
    show_date(yesterday(new_date(3,1,1999)));
    printf("Returning day before March 1 2024: ");
    show_date(yesterday(new_date(3,1,2024)));
    printf("Returning day before June 1 2022: ");
    show_date(yesterday(new_date(6,1,2022)));
    printf("Returning day before July 1 2022: ");
    show_date(yesterday(new_date(7,1,2022)));
    printf("Returning day before August 10 2022: ");
    show_date(yesterday(new_date(8,10,2022)));
    // Test tomorrow
    printf("Returning day after Jan 1 1900: ");
    show_date(tomorrow(new_date(1,1,1900)));
    printf("Returning day after Jan 31 1900: ");
    show_date(tomorrow(new_date(1,31,1900)));
    printf("Returning day after Feb 28 1900: ");
    show_date(tomorrow(new_date(2,28,1900)));
    printf("Returning day after Feb 28 2096: ");
    show_date(tomorrow(new_date(2,28,2096)));
    printf("Returning day after Feb 29 2096: ");
    show_date(tomorrow(new_date(2,29,2096)));
    printf("Returning day after May 31 2000: ");
    show_date(tomorrow(new_date(5,31,2000)));
    printf("Returning day after Dec 31 2000: ");
    show_date(tomorrow(new_date(12,31,2000)));
    printf("Returning day after Dec 31 1923: ");
    show_date(tomorrow(new_date(12,31,1923)));
    // Test get # days in between
    printf("(expect 0) Get #days between Jan 1 2020 and Jan 1 2020: %d\n",
            get_days_between(new_date(1,1,2020),new_date(1,1,2020)));  
    printf("(expect 1) Get #days between Jan 1 2020 and Jan 2 2020: %d\n",
            get_days_between(new_date(1,1,2020),new_date(1,2,2020)));  
    printf("(expect 2) Get #days between Jan 1 1900 and Jan 3 1900: %d\n",
            get_days_between(new_date(1,1,1900),new_date(1,3,1900)));  
    printf("(expect 1855) Get #days between Feb 1 2020 and Mar 1 2025: %d\n",
            get_days_between(new_date(2,1,2020),new_date(3,1,2025)));  
    printf("(expect 73048) Get #days between Jan 1 1900 and Dec 31 2099: %d\n",
            get_days_between(new_date(1,1,1900),new_date(12,31,2099)));  
    // Test first date array
    printf("Testing date_span of Jan 1 2020 and Jan 1 2020:\n");
    test_date_span(new_date(1,1,2020),new_date(1,1,2020),0);
    // Test date span with good arrays
    printf("Testing date_span of Jan 1 2020 and Jan 2 2020:\n");
    test_date_span(new_date(1,1,2020),new_date(1,2,2020),1);
    printf("Testing date_span of Jan 1 2020 and Feb 2 2020:\n");
    test_date_span(new_date(1,1,2020),new_date(2,2,2021),398);
    printf("Testing date_span of Dec 31 2020 and Mar 2 2021:\n");
    test_date_span(new_date(12,31,2020),new_date(3,2,2021),61);
    printf("Testing date_span of Jan 1 1900 and Mar 1 1900:\n");
    test_date_span(new_date(1,1,1900),new_date(3,1,1900),59);
    printf("Testing date_span of Dec 31 2095 and Jul 1 2096:\n");
    test_date_span(new_date(12,31,2095),new_date(7,1,2096),183);
    // Test pd2 before pd1 date span
    unsigned int length;
    printf("Testing date_span of Jan 2 2020 and Jan 1 2020:\n");
    pdate *dates2 = date_span(new_date(1,2,2020),new_date(1,1,2020), &length);
    printf("Array is length %d.\n",length);
    if (dates2 == NULL) {
        printf("NULL was returned\n");
    } else {
        printf("Error: NULL was not returned\n");
    }
    free(dates2);
    printf("Testing date_span of May 2 2003 and Apr 2 2003:\n");
    pdate *dates = date_span(new_date(5,2,2003),new_date(4,2,2003), &length);
    printf("Array is length %d.\n",length);
    if (dates == NULL) {
        printf("NULL was returned\n");
    } else {
        printf("Error: NULL was not returned\n");
    }
    free(dates);
    printf("Testing date_span of Feb 1 2020 and Feb 1 2019:\n");
    pdate *dates1 = date_span(new_date(2,1,2020),new_date(2,1,2019), &length);
    printf("Array is length %d.\n",length);
    if (dates1 == NULL) {
        printf("NULL was returned\n");
    } else {
        printf("Error: NULL was not returned\n");
    }
    free(dates1);
    // Test 3rd date array
    printf("Creating new array {Jan 2 2022, Jan 1 2022, Jan 3 2022}:\n");
    pdate dates4[] = {new_date(1,2,2022), new_date(1,1,2022), 
                      new_date(1,3,2022)};
    test_array_funcs(dates4, 3, 2);
    // Test 4th date array
    printf("Test date distance for {Feb 15 2022, Feb 1 2020,"
           " Mar 1 2025, Jan 11 2024, and Jan 22 2021}:\n");
    pdate dates5[] = {new_date(2,15,2022),new_date(2,1,2020),
                      new_date(3,1,2025),new_date(1,11,2024),
                      new_date(1,22,2021)};
    test_array_funcs(dates5, 5, 1855);
    printf("Test date distance for {Dec 31 2099, Aug 30 2083, Jun 1 1900, "
           " Jul 6 2003, Jan 1 1900, Feb 2 1999, Feb 29 2064, "
           " Aug 29 2083}:\n");
    pdate dates6[] = {new_date(12,31,2099), new_date(8,30,2083),
                      new_date(6,1,1900),new_date(7,6,2003),
                      new_date(1,1,1900),new_date(2,2,1999), 
                      new_date(2,29,2064), new_date(8,29,2083)};
    test_array_funcs(dates6, 8, 73048);
    return 0;
}
