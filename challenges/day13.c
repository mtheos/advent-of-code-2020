//
// Created by Michael on 13/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "day13.h"

static void readInput(int *earliestTime, int **busses, int *bussesCount) {
    FILE *input = fopen("../challenges/day13_shuttle_bus.txt", "r");
    if (input == NULL) {
        perror("Could not open file");
        exit(1);
    }
    size_t bufSize = 0;
    char *buf = NULL;
    getline(&buf, &bufSize, input);
    *earliestTime = atoi(buf);
    // only 2  lines of input
    getline(&buf, &bufSize, input);
    char *token = buf;
    while (1) {
        char *value = strtok(token, ",");
        token = NULL;
        if (value == NULL)
            break;
        *busses = realloc(*busses, ++*bussesCount * sizeof(int));
        int *p = &(*busses)[*bussesCount - 1];
        if (value[0] == 'x')
            *p = -1;
        else
            *p = atoi(value);
    }
    fclose(input);
    free(buf);
}

static int nextBus(int earliestTime, const int *busses, int bussesCount) {
    int earliestBus, earliestBusArrival = INT_MAX;
    for (int i = 0; i < bussesCount; ++i) {
        if (busses[i] == -1)
            continue;
        int nextArrival = busses[i] - earliestTime % busses[i];
        if (nextArrival < earliestBusArrival) {
            earliestBusArrival = nextArrival;
            earliestBus = busses[i];
        }
    }
    return earliestBus * earliestBusArrival;
}

// Chinese remainder theorem and/or extended Euclidean algorithm... not sure if they're the same
static ulong goldStar(const int *busses, int bussesCount) {
    ulong result = 0, lcm = 1;
    for (int i = 0; i < bussesCount; ++i) {
        int busId = busses[i];
        if (busId == -1)
            continue;
        while ((result + i) % busId != 0)
            result += lcm;
        lcm *= busId;
    }
    return result;
}

void shuttleBus1() {
    setbuf(stdout, NULL);
    int earliestTime;
    int *busses = NULL;
    int busesCount = 0;
    readInput(&earliestTime, &busses, &busesCount);
    printf("Answer: %d\n", nextBus(earliestTime, busses, busesCount));
    free(busses);
}

void shuttleBus2() {
    setbuf(stdout, NULL);
    int earliestTime;
    int *busses = NULL;
    int busesCount = 0;
    readInput(&earliestTime, &busses, &busesCount);
    printf("Answer: %ld\n", goldStar(busses, busesCount));
    free(busses);
}
