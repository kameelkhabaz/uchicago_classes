#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Kameel Khabaz
 * Lab 4
 * Feb 1 2022
 */

void replace_character(char *str, char replacee, char replacer) 
{
    /* takes a string str, a character replacee, and a character replacer
     * and replaces all instances of replacee with replacer in-place */
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == replacee) {
            str[i] = replacer;
        }
    }
}

char *remove_character(char *str, char c)
{
    /* takes a string str and a character c and returns a new string that
     * omits all instances of c in str
     */
    unsigned int cnt_c = 0;
    unsigned int len1 = 0; // initialize length of str

    // Calculate # times c is in str and str length in 1 loop
    while (str[len1] != '\0') {
        if (str[len1] == c) {
            cnt_c++;
        }
        len1++;
    }
    
    // Create new string without c 
    unsigned int len2 = len1 - cnt_c;
    char *str2 = (char *)malloc(sizeof(char) * (len2 + 1));
    if (str2 == NULL) {
        fprintf(stderr, "malloc has failed. \n");
        exit(1);
    }

    // Populate str2 with non-c characters of str
    int i1 = 0; // index in str 
    int i2 = 0; // index in str2
    while (i2 < len2) {
        if (str[i1] != c) {
            str2[i2] = str[i1];
            i2++;
        } 
        i1++;
    }
    str2[len2] = '\0';
    return str2;
}

int find_substring_location(char *str, char *sub)
{
    /* takes a string str and a string sub and returns the location of 
     * substring sub in str. It should return the index of the first 
     * character of sub in str and -1 if sub is not in str. If the 
     * substring sub occurs multiple times in str, return the location 
     * of the first instance
     */
    
    unsigned int sublen = strlen(sub);
    unsigned int slen = strlen(str);
    unsigned int stri; // index in main string when we search in substring

    for (int i = 0; i < slen; i++) {
        if (str[i] == sub[0]) {
            // First character of substring matches, check others
            int ismatch = 1;
            for (int j = 1; j < sublen; j++) {
                stri = i + j;
                if (stri >= slen || str[stri] != sub[j]) {
                    // Substring doesn't match
                    ismatch = 0; 
                    break;
                }
            }
            if (ismatch) {
                // Substring matches
                return i;
            }
        } 
    }
    return -1;
}

char *find_match(char *str, char *pattern)
{
    /* which takes a string str and a string pattern and returns 
     * a copy of the substring in str that matches pattern. pattern can
     * contain the wildcard character '?', which can match any character.
     * Other characters only match themselves. If pattern is not located
     * in str, return NULL.
     */

    unsigned int patlen = strlen(pattern);
    unsigned int slen = strlen(str);
    unsigned int stri; // index in main string when we search in pattern

    for (int i = 0; i < slen; i++) {
        if (pattern[0] == '?' || str[i] == pattern[0]) {
            // First character of pattern matches, check others
            int ismatch = 1;
            for (int j = 1; j < patlen; j++) {
                stri = i + j;
                if (stri >= slen || 
                    (str[stri] != pattern[j] && pattern[j] != '?')) {
                    // Substring doesn't match and no wildcard
                    ismatch = 0; 
                    break;
                }
            }
            if (ismatch) {
                // Substring matches or there is wildcard
                return strndup(str + i, patlen); 
            }
        } 
    }
    return NULL;
}

int main()
{
    char *s1 = strdup("Hello, world!");
    char rep = '!';
    char rep_with = '.';
    printf("Replacing %c for %c in %s:\n",rep,rep_with,s1);
    replace_character(s1, rep, rep_with);
    printf("%s\n", s1);

    rep = '.';
    rep_with = ','; 
    printf("Replacing %c for %c in %s:\n",rep,rep_with,s1);
    replace_character(s1, rep, rep_with);
    printf("%s\n", s1);
    
    char *ss = strdup("");
    printf("Replacing %c for %c in %s:\n",rep,rep_with,ss);
    replace_character(ss, rep, rep_with);
    printf("%s\n", ss);
    free(ss);

    char *ss1 = strdup("XXXxXXX");
    rep = 'X';
    rep_with = 'a';
    printf("Replacing %c for %c in %s:\n",rep,rep_with,ss1);
    replace_character(ss1, rep, rep_with);
    printf("%s\n", ss1);
    free(ss1);
    
    rep = 'd';
    rep_with = 'd';
    printf("Replacing %c for %c in %s:\n",rep,rep_with,s1);
    replace_character(s1, rep, rep_with);
    printf("%s\n", s1);
    
    rep = ',';
    rep_with = '&'; 
    printf("Replacing %c for %c in %s:\n",rep,rep_with,s1);
    replace_character(s1, rep, rep_with);
    printf("%s\n", s1);
    free(s1); 

    char *s2 = strdup("Hello, world!");
    rep = 'l';
    rep_with = 'x';
    printf("Replacing %c for %c in %s:\n",rep,rep_with,s2);
    replace_character(s2, rep, rep_with);
    printf("%s\n", s2); 

    rep = 'l';
    rep_with = 'I';
    printf("Replacing %c for %c in %s:\n",rep,rep_with,s2);
    replace_character(s2, rep, rep_with);
    printf("%s\n", s2); 

    rep = ' ';
    rep_with = '*';
    printf("Replacing %c for %c in %s:\n",rep,rep_with,s2);
    replace_character(s2, rep, rep_with);
    printf("%s\n", s2); 
    free(s2);

    char s3[] = "Hello world!";
    char rchar = 'l';
    printf("Removing %c from %s\n", rchar, s3);
    char *rep_str = remove_character(s3, rchar);
    printf("Got %s\n", rep_str);
    free(rep_str);
    
    rchar = 'K';
    printf("Removing %c from %s\n", rchar, s3);
    rep_str = remove_character(s3, rchar);
    printf("Got %s\n", rep_str);
    free(rep_str);

    rchar = 'H';
    printf("Removing %c from %s\n", rchar, s3);
    rep_str = remove_character(s3, rchar);
    printf("Got %s\n", rep_str);
    free(rep_str);

    rchar = '!';
    printf("Removing %c from %s\n", rchar, s3);
    rep_str = remove_character(s3, rchar);
    printf("Got %s\n", rep_str);
    free(rep_str);

    rchar = ' ';
    printf("Removing %c from %s\n", rchar, s3);
    rep_str = remove_character(s3, rchar);
    printf("Got %s\n", rep_str);
    free(rep_str);

    char s5[] = "OOOOOOOOOOOOOO";
    rchar = 'O';
    printf("Removing %c from %s\n", rchar, s5);
    rep_str = remove_character(s5, rchar);
    printf("Got %s\n", rep_str);
    free(rep_str);
    
    char s6[] = "";
    rchar = 'A';
    printf("Removing %c from %s\n", rchar, s6);
    rep_str = remove_character(s6, rchar);
    printf("Got %s\n", rep_str);
    free(rep_str);
    
    char sf[] = "Hello world!";
    char repf[] = "H";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf,repf);
    printf("Expect %d, got %d.\n", 0, find_substring_location(sf,repf));

    char sf2[] = "Hello, world!";
    char repf2[] = "orl";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf2,repf2);
    printf("Expect %d, got %d.\n", 8, find_substring_location(sf2,repf2));

    char sf3[] = "llo";
    char repf5[] = "lo";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf3,repf5);
    printf("Expect %d, got %d.\n", 1, find_substring_location(sf3,repf5));

    char sf4[] = "lllllllllll";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf4,repf5);
    printf("Expect %d, got %d.\n", -1, find_substring_location(sf4,repf5));

    char sf1[] = "";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf1,repf5);
    printf("Expect %d, got %d.\n", -1, find_substring_location(sf1,repf5));

    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf1,sf1);
    printf("Expect %d, got %d.\n", -1, find_substring_location(sf1,sf1));

    char sf5[] = "llllllo";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf5,repf5);
    printf("Expect %d, got %d.\n", 5, find_substring_location(sf5,repf5));

    char sf7[] = "lolololololololololololo";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf7,repf5);
    printf("Expect %d, got %d.\n", 0, find_substring_location(sf7,repf5));

    char sf6[] = "oll";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf6,repf5);
    printf("Expect %d, got %d.\n", -1, find_substring_location(sf6,repf5));

    char repf3[] = "l";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf,repf3);
    printf("Expect %d, got %d.\n", 2, find_substring_location(sf,repf3));

    char repf4[] = "Ha";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf2,repf4);
    printf("Expect %d, got %d.\n", -1, find_substring_location(sf2,repf4));

    char repf6[] = "world!";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf2,repf6);
    printf("Expect %d, got %d.\n", 7, find_substring_location(sf2,repf6));

    char repf7[] = "world! ";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf2,repf7);
    printf("Expect %d, got %d.\n", -1, find_substring_location(sf2,repf7));

    char repf8[] = "h";
    printf("Testing find_substring_location(\"%s\",\"%s\").\n",sf2,repf8);
    printf("Expect %d, got %d.\n", -1, find_substring_location(sf2,repf8));

    char pat[] = "llo";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat);
    char *match = find_match(sf, pat);
    printf("\"%s\".\n",match);
    free(match);

    char pat2[] = "r?d!";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat2);
    match = find_match(sf, pat2);
    printf("\"%s\".\n",match);
    free(match);

    char pat3[] = "?llo";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat3);
    match = find_match(sf, pat3);
    printf("\"%s\".\n",match);
    free(match);

    char pat4[] = "?llo??";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat4);
    match = find_match(sf, pat4);
    printf("\"%s\".\n",match);
    free(match);

    char pat5[] = "H?e";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat5);
    match = find_match(sf, pat5);
    printf("\"%s\".\n",match);
    free(match);

    char pat6[] = "?";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat6);
    match = find_match(sf, pat6);
    printf("\"%s\".\n",match);
    free(match);

    char pat7[] = "??";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat7);
    match = find_match(sf, pat7);
    printf("\"%s\".\n",match);
    free(match);

    char pat8[] = "????????????";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat8);
    match = find_match(sf, pat8);
    printf("\"%s\".\n",match);
    free(match);

    char pat9[] = "?????????????";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat9);
    match = find_match(sf, pat9);
    printf("\"%s\".\n",match);
    free(match);

    char pat10[] = "wo??d!";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat10);
    match = find_match(sf, pat10);
    printf("\"%s\".\n",match);
    free(match);

    char pat11[] = "world!!";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat11);
    match = find_match(sf, pat11);
    printf("\"%s\".\n",match);
    free(match);

    char pat12[] = "w";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat12);
    match = find_match(sf, pat12);
    printf("\"%s\".\n",match);
    free(match);

    char pat13[] = "worl";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat13);
    match = find_match(sf, pat13);
    printf("\"%s\".\n",match);
    free(match);

    char pat14[] = "l?";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat14);
    match = find_match(sf, pat14);
    printf("\"%s\".\n",match);
    free(match);

    char pat15[] = "o?";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat15);
    match = find_match(sf, pat15);
    printf("\"%s\".\n",match);
    free(match);

    char pat16[] = "?G";
    printf("Testing find_match(\"%s\", \"%s\"): ",sf,pat16);
    match = find_match(sf, pat16);
    printf("\"%s\".\n",match);
    free(match);

}
