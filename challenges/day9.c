//
// Created by Michael on 08/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "day9.h"

static void readInput(int **stream, int *streamLength) {
    FILE *input = fopen("../challenges/day9_encoding.txt", "r");
    if (input == NULL) {
        perror("Could not open file");
        exit(1);
    }
    size_t bufSize = 0;
    char *buf = NULL;
    while (getline(&buf, &bufSize, input) != -1) {
        *stream = realloc(*stream, ++*streamLength * sizeof(int));
        int *datum = &(*stream)[*streamLength - 1];
        sscanf(buf, "%d", datum);
    }
    fclose(input);
    free(buf);
}

static int xmas(const int *window, const int target) {
    for (int i = 0; i < 25; ++i) {
        for (int j = i; j < 25; ++j) {
            int x = window[i];
            int y = window[j];
            if (x + y == target)
                return 1;
        }
    }
    return 0;
}

static int validateStream(int *stream, int streamLength, int window) {
    int i = window - 1;
    while (++i < streamLength)
        if (!xmas(&stream[i - 25], stream[i]))
            return i;
    return -1;
}

static int min(int *start, const int *end) {
    int min = INT_MAX;
    start = start - 1;
    while (++start < end)
        if (*start < min)
            min = *start;
    return min;
}

static int max(int *start, const int *end) {
    int max = INT_MIN;
    start = start - 1;
    while (++start < end)
        if (*start > max)
            max = *start;
    return max;
}

static int check(int *start, const int *end) {
    int sum = 0;
    start = start - 1;
    while (++start < end)
            sum += *start;
    return sum;
}

static int breakXmas(int *stream, int streamLength, int target) {
    long sum = 0;
    int first = -1, last = -1;
    while (last < streamLength) {
        if (sum < target)
            sum += stream[++last];
        else if (sum > target)
            sum -= stream[++first];
        else
            return min(&stream[first - 1], &stream[last]) + max(&stream[first - 1], &stream[last]);
    }
    return -1;
}

void encoding1() {
    int *stream = NULL;
    int streamLength = 0;
    readInput(&stream, &streamLength);
    printf("Answer: %d\n", stream[validateStream(stream, streamLength, 25)]);
    free(stream);
}

void encoding2() {
    int *stream = NULL;
    int streamLength = 0;
    readInput(&stream, &streamLength);
    printf("Answer: %d\n", breakXmas(stream, streamLength, stream[validateStream(stream, streamLength, 25)]));
    free(stream);
}
