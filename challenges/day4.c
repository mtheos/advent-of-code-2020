//
// Created by Michael on 04/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "day4.h"

typedef struct passport {
    int byr;
    int iyr;
    int eyr;
    char hgt[7];  // Size is 1 more than needed to pick up size errors (i.e. 1933 vs 193)
    char hcl[9];  // ^
    char ecl[5];  // ^
    char pid[11]; // ^
//    char cid[]; // ignored
} Passport;

static void readInput(Passport **passports, int *passportCount) {
    FILE *input = fopen("../challenges/day4_passports.txt", "r");
    if (input == NULL) {
        perror("Could not open file");
        exit(1);
    }
    size_t bufSize = 0;
    char *buf = NULL;
    while (getline(&buf, &bufSize, input) != -1) {
        *passports = realloc(*passports, ++*passportCount * sizeof(Passport));
        Passport *p = &(*passports)[*passportCount - 1];
        memset(p, -1, sizeof(int) * 3);
        memset(p->hgt, 0, sizeof(Passport) - (sizeof(int) * 3));
        do {
            char *token = buf;
            while (1) {
                char *key = strtok(token, ":");
                token = NULL;
                char *value = strtok(NULL, " \n");
                if (value == NULL)
                    break;
                if (strcmp(key, "byr") == 0) {
                    p->byr = atoi(value);
                } else if (strcmp(key, "iyr") == 0) {
                    p->iyr = atoi(value);
                } else if (strcmp(key, "eyr") == 0) {
                    p->eyr = atoi(value);
                } else if (strcmp(key, "hgt") == 0) {
                    strncpy(p->hgt, value, sizeof(p->hgt) / sizeof(char));
                } else if (strcmp(key, "hcl") == 0) {
                    strncpy(p->hcl, value, sizeof(p->hcl) / sizeof(char));
                } else if (strcmp(key, "ecl") == 0) {
                    strncpy(p->ecl, value, sizeof(p->ecl) / sizeof(char));
                } else if (strcmp(key, "pid") == 0) {
                    strncpy(p->pid, value, sizeof(p->pid) / sizeof(char));
                } else if (strcmp(key, "cid") == 0) {
                    // ignored
                }
                else { abort(); }
            }
        } while (getline(&buf, &bufSize, input) != -1 && buf[0] != '\n');
    }
    fclose(input);
    free(buf);
}

static int quickValid(Passport *p) {
    if (p->byr == -1 || p->iyr == -1 || p->eyr == -1)
        return 0;
    if (p->hgt[0] == '\0' || p->hcl[0] == '\0' || p->ecl[0] == '\0' || p->pid[0] == '\0')
        return 0;
    return 1;
}

static int inCharSet(char *str, char *set) {
    char *ptr = str - 1;
    while (*++ptr != '\0')
        if (strchr(set, *ptr) == NULL)
            return 0;
    return 1;
}

static int slowValid(Passport *p) {
    if (p->byr < 1920 || p->byr > 2002)
        return 0;

    if (p->iyr < 2010 || p->iyr > 2020)
        return 0;

    if (p->eyr < 2020 || p->eyr > 2030)
        return 0;

    int x = atoi(p->hgt);
    if (p->hgt[strlen(p->hgt) - 2] == 'c' ? x < 150 || x > 193 : x < 59 || x > 76)
        return 0;

    if (p->hcl[0] != '#' || !inCharSet(p->hcl + 1, "0123456789abcdef"))
        return 0;

    if (strstr("amb blu brn gry grn hzl oth", p->ecl) == NULL)
        return 0;

    if (strlen(p->pid) != 9 || !inCharSet(p->pid, "0123456789"))
        return 0;
    return 1;
}

static int isValid(Passport *p, int thorough) {
    if (!quickValid(p))
        return 0;
    if (!thorough)
        return 1;
    return slowValid(p);
}

void passports1() {
    Passport *passports = NULL;
    int passportCount = 0;
    readInput(&passports, &passportCount);
    int valid = 0;
    for (int i = 0; i < passportCount; ++i) {
        Passport *p = &passports[i];
        if (isValid(p, 0))
            ++valid;
    }
    printf("Answer: %d\n", valid);
    free(passports);
}

void passports2() {
    Passport *passports = NULL;
    int passportCount = 0;
    readInput(&passports, &passportCount);
    int valid = 0;
    for (int i = 0; i < passportCount; ++i) {
        Passport *p = &passports[i];
        if (isValid(p, 1))
            ++valid;
    }
    printf("Answer: %d\n", valid);
    free(passports);
}
