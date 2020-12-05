//
// Created by Michael on 01/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "day1.h"

void expenses1() {
    FILE *input = fopen("../challenges/day1_expenses.txt", "r");
    if (input == NULL) {
        perror("Could not open file");
        exit(1);
    }
    size_t bufSize = 0;
    char *buf = NULL;
    int *expenses = NULL;
    int expenseCount = 0;
    while (getline(&buf, &bufSize, input) != -1) {
        expenses = realloc(expenses, ++expenseCount * sizeof(int));
        expenses[expenseCount - 1] = atoi(buf);
    }
    free(buf);
    int seen[2020];
    bzero(seen, sizeof(seen));
    for (int i = 0; i < expenseCount; i++) {
        int x = expenses[i];
        int y = 2020 - x;
        if (seen[y] == 1) {
            printf("Answer: %d, %d => %d\n", x, y, x * y);
            break;
        }
        seen[x] = 1;
    }
    free(expenses);
}

void expenses2() {
    FILE *input = fopen("../challenges/day1_expenses.txt", "r");
    if (input == NULL) {
        perror("Could not open file");
        exit(1);
    }
    size_t bufSize = 0;
    char *buf = NULL;
    int *expenses = NULL;
    int expenseCount = 0;
    while (getline(&buf, &bufSize, input) != -1) {
        expenses = realloc(expenses, ++expenseCount * sizeof(int));
        expenses[expenseCount - 1] = atoi(buf);
    }
    free(buf);
    int seen[2020];
    int found = 0;
    bzero(seen, sizeof(seen));
    for (int i = 0; i < expenseCount - 1; i++) {
        int x = expenses[i];
        for (int j = i + 1; j < expenseCount; j++) {
            int y = expenses[j];
            int z = 2020 - x - y;
            if (z < 0) continue;
            if (seen[z] == 1) {
                printf("Answer: %d, %d, %d => %d\n", x, y, z, x * y * z);
                found = 1;
            }
            if (found) break;
            seen[x] = 1;
            seen[y] = 1;
        }
        if (found) break;
    }
    free(expenses);
}
