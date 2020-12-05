//
// Created by Michael on 02/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "day2.h"

typedef struct password {
    int min;
    int max;
    char ch;
    char password[31];
} Password;

static int validPassword_old(Password *p) {
    int count = 0;
    char *ch = p->password - 1;
    while (*++ch != '\0')
        if (*ch == p->ch)
            count++;
    return count >= p->min && count <= p->max;
}

static int validPassword_new(Password *p) {
    if (p->max > strlen(p->password))
        abort();
    return p->ch == p->password[p->min - 1] ^ p->ch == p->password[p->max - 1];
}

static void readInput(Password **passwords, int *passwordsCount) {
    FILE *input = fopen("../challenges/day2_passwords.txt", "r");
    if (input == NULL) {
        perror("Could not open file");
        exit(1);
    }
    size_t bufSize = 0;
    char *buf = NULL;
    while (getline(&buf, &bufSize, input) != -1) {
        *passwords = realloc(*passwords, ++*passwordsCount * sizeof(Password));
        Password *p = &(*passwords)[*passwordsCount - 1];
        sscanf(buf, "%d-%d %c: %30s", &p->min, &p->max, &p->ch, (char *)&p->password);
    }
    free(buf);
}

void passwords1() {
    Password *passwords = NULL;
    int passwordsCount = 0;
    readInput(&passwords, &passwordsCount);
    int valid = 0;
    for (int i = 0; i < passwordsCount; i++)
        if (validPassword_old(&passwords[i]))
            ++valid;
    free(passwords);
    printf("%d valid passwords\n", valid);
}

void passwords2() {
    Password *passwords = NULL;
    int passwordsCount = 0;
    readInput(&passwords, &passwordsCount);
    int valid = 0;
    for (int i = 0; i < passwordsCount; i++)
        if (validPassword_new(&passwords[i]))
            ++valid;
    free(passwords);
    printf("%d valid passwords\n", valid);
}
