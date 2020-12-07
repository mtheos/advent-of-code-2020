//
// Created by Michael on 07/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "day7.h"

#define MAX_BAG_NAME 30
#define MAX_BAGS_CONTAINED 5

typedef struct bag {
    char name[MAX_BAG_NAME];
    struct bag *bagsContained[MAX_BAGS_CONTAINED];
    int numOfBagsContained[MAX_BAGS_CONTAINED];
    int bagsContainedCount;
    int canContainNamedBag; // aka shiny gold
} Bag;

static char *strtok_str(char *str, const char *delimStr) {
    static char *nextStart;
    if (str)
        nextStart = str;
    char *ret = nextStart;
    char *x = strstr(nextStart, delimStr);
    if (x == NULL)
        return NULL;
    nextStart = x;
    *nextStart = '\0';
    nextStart += strlen(delimStr);
    return ret;
}

static void readInput(Bag **bags, int *bagsCount) {
    FILE *input = fopen("../challenges/day7_luggage.txt", "r");
    if (input == NULL) {
        perror("Could not open file");
        exit(1);
    }
    size_t bufSize = 0;
    char *buf = NULL;
    while (getline(&buf, &bufSize, input) != -1) {
        *bags = realloc(*bags, ++*bagsCount * sizeof(Bag));
        Bag *b = &(*bags)[*bagsCount - 1];
        bzero(b, sizeof(Bag));
        char *value = strtok_str(buf, " bags contain ");
        assert(strlen(value) < MAX_BAG_NAME);
        strncpy(b->name, value, MAX_BAG_NAME);
    }
    rewind(input);
    Bag *b = *bags - 1;
    while (getline(&buf, &bufSize, input) != -1) {
        ++b; // iterating in the same order as before
        strtok_str(buf, " bags contain "); // throwaway
        while (1) {
            char *containedBag = strtok_str(NULL, ", ");
            if (containedBag == NULL)
                containedBag = strtok_str(NULL, ".\n");
            if (containedBag == NULL)
                break; // finished
            int numContained = atoi(containedBag);
            containedBag += 2; // skip "%d "
            char *x = containedBag;
            while (*++x != '\0'); // Find and wipe out " bags" or " bag"
            while (*--x != ' ');
            *x = '\0';
            assert(b->bagsContainedCount < MAX_BAGS_CONTAINED);
            for (int i = 0; i < *bagsCount; ++i) {
                Bag *b2 = &(*bags)[i];
                if (strcmp(b2->name, containedBag) == 0) {
                    b->bagsContained[b->bagsContainedCount] = b2;
                    b->numOfBagsContained[b->bagsContainedCount++] = numContained;
                    break;
                }
            }
        }
    }
    fclose(input);
    free(buf);
}

int countBagsWithNamedBag(Bag *bags, int bagsCount, const char *name) {
    for (int i = 0; i < bagsCount; ++i)
        for (int j = 0; j < bags[i].bagsContainedCount; ++j)
            if (strcmp(bags[i].bagsContained[j]->name, name) == 0)
                bags[i].canContainNamedBag = 1;
    int changed;
    do {
        changed = 0;
        for (int i = 0; i < bagsCount; ++i)
            for (int j = 0; j < bags[i].bagsContainedCount; ++j)
                if (bags[i].bagsContained[j]->canContainNamedBag && bags[i].canContainNamedBag == 0)
                    changed = bags[i].canContainNamedBag = 1;
    } while (changed);
    int count = 0;
    for (int i = 0; i < bagsCount; ++i)
        if (bags[i].canContainNamedBag)
            ++count;
    return count;
}

int countBagsInBag(Bag *b) {
    int count = 0;
    for (int i = 0; i < b->bagsContainedCount; ++i) {
        Bag *b2 = b->bagsContained[i];
        int numB2 = b->numOfBagsContained[i];
        count += numB2 + numB2 * countBagsInBag(b2);
    }
    return count;
}

void luggage1() {
    Bag *bags = NULL;
    int bagsCount = 0;
    readInput(&bags, &bagsCount);
    printf("Answer: %d\n", countBagsWithNamedBag(bags, bagsCount, "shiny gold"));
    free(bags);
}

void luggage2() {
    Bag *bags = NULL;
    int bagsCount = 0;
    readInput(&bags, &bagsCount);
    Bag *b = bags;
    while (strcmp(b->name, "shiny gold") != 0) ++b;
    printf("Answer: %d\n", countBagsInBag(b));
    free(bags);
}
