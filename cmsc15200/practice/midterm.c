#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **scatter(char *s)
{
    if (s == NULL) {
        fprintf(stderr,"null string\n");
        exit(1);
    }
    if (s[0] == '\0') {
        fprintf(stderr,"Empty string\n");
        exit(1);
    }
    int len = strlen(s);
    char **arr = (char **)malloc(sizeof(char *) * len);
    for (int i = 0; i < len; i++) {
        char sub[2] = {0};
        sub[0] = s[i];
        arr[i] = strdup(sub);
        // we can also do out[i] = (char *)malloc(sizeof(char) * 2);
        // and then check null
        // and then do out[i][0] = s[i] and out[i][1] = '\0'
    }
    return arr;
}

struct vec3{
    double x;
    double y;
    double z;
};

double dot(struct vec3 *v1, struct vec3 *v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

char first_non_repeated(char *str)
{
    if (str == NULL) {
        fprintf(stderr,"null string\n");
        exit(1);
    }
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    for (int i = 0; i < len; i++) {
        int unique = 1;
        for (int j = 0; j < len; j++) {
            if (str[j] == str[i] && i != j) { // MUST LOOP THROUGH ALL
                unique = 0;// MUST CHECK BOTH
                break; // MORE EFFICIENT
            }
        }
        if (unique) {
            return str[i];
        }
    }
    return '\0';
}

char first_repeated_2(char *str)
{
    // This returns first REPEATED CHARACTER
    // IF YOU WANT TO FIND NON-REPEATED CHARACTER WOULD NEED TO KEEP TRACK OF 
    // WHICH INDICES HAD WHICH LETTERS AS YOU MUST RETURN FIRST ELEMENT IN 
    // ARRAY TO BE UNIQUE NOT FIRST ALPHABETICALLY
    if (str == NULL) {
        fprintf(stderr,"null string\n");
        exit(1);
    }
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    char letters[26] = {0};
    for (int i = 0; i < len; i++) {
        int index = str[i] - 'a';
        if (letters[index] > 0) {
            return str[i];
        } else {
            letters[index]++;
        }
    }
    return '\0';
}

int main()
{
    char **a = scatter("ASADFDSF");
    printf("%s\n",a[0]);
    struct vec3 v1 = {2,3,4};
    struct vec3 v2 = {1000,100,10};
    printf("%f\n",dot(&v1,&v2));
    char *s = "dddef";
    printf("First unique in %s is %c\n",s,first_non_repeated(s));
    printf("First unique in %s (method 2) is %c\n",s,first_non_repeated_2(s));
    return 0;
}

