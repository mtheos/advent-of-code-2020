//
// Created by Michael on 03/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "day3.h"

static int traverse(char **trail, int trailLength, int moveX, int moveY) {
    int hit = 0, xPos = 0, yPos = 0, width = (int)strlen(trail[0]);
    while (yPos < trailLength - moveY) {
        xPos += moveX;
        yPos += moveY;
        if (xPos >= width)
            xPos -= width;
        if (trail[yPos][xPos] == '#')
            ++hit;
    }
    return hit;
}

static void readInput(char ***trail, int *trailLength) {
    FILE *input = fopen("../challenges/trees_day3.txt", "r");
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

void trees1() {
    char **trail = NULL;
    int trailLength = 0;
    readInput(&trail, &trailLength);
    int ex = 3, oh = 1;
    printf("Answer (%d, %d): %d\n", ex, oh, traverse(trail, trailLength, ex, oh));
    for (int i = 0; i < trailLength; i++)
        free(trail[i]);
    free(trail);
}

void trees2() {
    char **trail = NULL;
    int trailLength = 0;
    readInput(&trail, &trailLength);
    int exs[] = { 1, 3, 5, 7, 1 };
    int ohs[] = { 1, 1, 1, 1, 2 };
    long ans = 1;
    for (int i = 0; i < sizeof(exs) / sizeof(exs[0]); i++) {
        int result = traverse(trail, trailLength, exs[i], ohs[i]);
        ans *= result;
        printf("(%d, %d): %d\n", exs[i], ohs[i], result);
    }
    printf("Answer: %ld\n", ans);
    for (int i = 0; i < trailLength; i++)
        free(trail[i]);
    free(trail);
}
