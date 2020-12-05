//
// Created by Michael on 05/12/2020.
//

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "day5.h"

static int binarySearch(const char *boardingPass, int start, int end, int iter, char low, char high) {
    for (int i = 0; i < iter; i++)
        if (boardingPass[i] == low)
            end = start + (end - start) / 2;
        else if (boardingPass[i] == high)
            start = start + (end - start) / 2 + 1;
        else abort();
    assert(start == end);
    return start;
}

static int getRow(const char *boardingPass) {
    return binarySearch(boardingPass, 0, 127, 7, 'F', 'B');
}

static int getSeat(const char *boardingPass) {
    return binarySearch(boardingPass, 0, 7, 3, 'L', 'R');
}

static int getMaxSeatId(char **boardingPasses, int boardingPassesCount) {
    int max = -1;
    for (int i = 0; i < boardingPassesCount; i++) {
        int id = getRow(boardingPasses[i]) * 8 + getSeat(boardingPasses[i] + 7);
        max = id > max ? id : max;
    }
    return max;
}

static int getMissingSeat(char **boardingPasses, int boardingPassesCount) {
    int ids[871] = { 0 }; // from max seat id
    for (int i = 0; i < boardingPassesCount; i++) {
        int id = getRow(boardingPasses[i]) * 8 + getSeat(boardingPasses[i] + 7);
        ids[id] = 1;
    }
    int i = 0;
    while (++i < 870)
        if (ids[i] == 0 && ids[i - 1] == 1 && ids[i + 1] == 1)
            return i;
    abort();
}

static void readInput(char ***trail, int *trailLength) {
    FILE *input = fopen("../challenges/day5_boarding.txt", "r");
    if (input == NULL) {
        perror("Could not open file");
        exit(1);
    }
    size_t bufSize = 0;
    char *buf = NULL;
    while (getline(&buf, &bufSize, input) != -1) {
        *trail = realloc(*trail, ++*trailLength * sizeof(char *));
        char *line = malloc(sizeof(char) * strlen(buf));
        (*trail)[*trailLength - 1] = line;
        sscanf(buf, "%s", line);
    }
    free(buf);
}

void boarding1() {
    char **boardingPasses = NULL;
    int boardingPassesCount = 0;
    readInput(&boardingPasses, &boardingPassesCount);
    printf("Answer: %d\n", getMaxSeatId(boardingPasses, boardingPassesCount));
    for (int i = 0; i < boardingPassesCount; i++)
        free(boardingPasses[i]);
    free(boardingPasses);
}

void boarding2() {
    char **boardingPasses = NULL;
    int boardingPassesCount = 0;
    readInput(&boardingPasses, &boardingPassesCount);
    printf("Answer: %d\n", getMissingSeat(boardingPasses, boardingPassesCount));
    for (int i = 0; i < boardingPassesCount; i++)
        free(boardingPasses[i]);
    free(boardingPasses);
}
