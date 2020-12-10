//
// Created by Michael on 10/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "day10.h"

static void readInput(int **stream, int *streamLength) {
    FILE *input = fopen("../challenges/day10_jolting.txt", "r");
    if (input == NULL) {
        perror("Could not open file");
        exit(1);
    }
    size_t bufSize = 0;
    char *buf = NULL;
    *streamLength = 2; // outlet + computer
    *stream = realloc(*stream, *streamLength * sizeof(int));
    *stream[0] = 0;
    int max = -1;
    while (getline(&buf, &bufSize, input) != -1) {
        *stream = realloc(*stream, ++*streamLength * sizeof(int));
        int *datum = &(*stream)[*streamLength - 1];
        sscanf(buf, "%d", datum);
        if (*datum > max) max = *datum;
    }
    (*stream)[1] = max + 3; // hole left from before
    fclose(input);
    free(buf);
}

static int cmp(const int *x, const int *y) {
    if (*x > *y)
        return 1;
    if (*x < *y)
        return -1;
    return 0;
}

static int countDiff(const int *stream, const int streamLength, const int diff) {
    int count = 0;
    for (int i = 0; i < streamLength - 1; ++i)
        if (stream[i + 1] - stream[i] == diff)
            count += 1;
    return count;
}

static int verify(const int *stream, const int streamLength, const int limit) {
    for (int i = 0; i < streamLength - 1; ++i)
        if (stream[i + 1] - stream[i] > limit)
            return 0;
    return 1;
}

static long godIHateDynamicProgramming(const int *stream, const int streamLength) {
    long dp[streamLength];
    bzero(dp, streamLength * sizeof(long));
    dp[0] = 1;
    for (int i = 0; i < streamLength; ++i)
        for (int d = 1; d <= 3; ++d)
            if (i + d < streamLength)
                if (stream[i + d] <= stream[i] + 3)
                    dp[i + d] += dp[i];
    return dp[streamLength - 1];
}

void jolting1() {
    int *stream = NULL;
    int streamLength = 0;
    readInput(&stream, &streamLength);
    qsort(stream, streamLength, sizeof(int), (__compar_fn_t) cmp);
    assert(verify(stream, streamLength, 3));
    int by1 = countDiff(stream, streamLength, 1);
    int by3 = countDiff(stream, streamLength, 3);
    printf("Answer (%d, %d): %d\n", by1, by3, by1 * by3);
    free(stream);
}

void jolting2() {
    int *stream = NULL;
    int streamLength = 0;
    readInput(&stream, &streamLength);
    qsort(stream, streamLength, sizeof(int), (__compar_fn_t) cmp);
    assert(verify(stream, streamLength, 3));
    long doublePenetrogramming = godIHateDynamicProgramming(stream, streamLength);
    printf("Answer: %ld\n", doublePenetrogramming);
    free(stream);
}
