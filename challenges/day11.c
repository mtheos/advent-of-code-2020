//
// Created by Michael on 11/12/2020.
//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "day11.h"

typedef enum {
    FLOOR,
    EMPTY,
    SEATED
} SeatState;

typedef struct seat {
    SeatState newState;
    SeatState currState;
} Seat;

static int stateTransition_simplex(Seat **seatingRows, int y, int x, int rowCount, int seatsPerRow) {
    if (seatingRows[y][x].currState == FLOOR)
        return 0;
    int occupied = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        if (y + dy < 0 || y + dy >= rowCount)
            continue;
        for (int dx = -1; dx <= 1; ++dx) {
            if ((x + dx < 0 || x + dx >= seatsPerRow) || (dx == 0 && dy == 0))
                continue;
            if (seatingRows[y + dy][x + dx].currState == SEATED)
                ++occupied;
        }
    }
    if (occupied == 0 && seatingRows[y][x].currState == EMPTY)
        return 1;
    else if (occupied >= 4 && seatingRows[y][x].currState == SEATED)
        return -1;
    else
        return 0;
}

static int stateTransition_complex(Seat **seatingRows, int y, int x, int rowCount, int seatsPerRow) {
    if (seatingRows[y][x].currState == FLOOR)
        return 0;
    const int deltas[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    int occupied = 0;
    for (int i = 0; i < 8; ++i) {
        int dy = deltas[i][0], dx = deltas[i][1];
        while ((y + dy >= 0 && y + dy < rowCount) && (x + dx >= 0 && x + dx < seatsPerRow)) {
            if (dy != 0 || dx != 0)
                if (seatingRows[y + dy][x + dx].currState != FLOOR) {
                    if (seatingRows[y + dy][x + dx].currState == SEATED)
                        ++occupied;
                    break;
                }
            dx += deltas[i][1], dy += deltas[i][0];
        }
    }
    if (occupied == 0 && seatingRows[y][x].currState == EMPTY)
        return 1;
    else if (occupied >= 5 && seatingRows[y][x].currState == SEATED)
        return -1;
    else
        return 0;
}

static void updateState(Seat **seatingRows, int rowCount, int seatsPerRow) {
    for (int y = 0; y < rowCount; ++y)
        for (int x = 0; x < seatsPerRow; ++x)
            seatingRows[y][x].currState = seatingRows[y][x].newState;
}

static int isThisTheGameOfLife(Seat **seatingRows, int rowCount, int seatsPerRow, int (*transitionFunction)(Seat **, int, int, int, int)) {
    int occupied = 0, changed;
    do {
        changed = 0;
        for (int y = 0; y < rowCount; ++y)
            for (int x = 0; x < seatsPerRow; ++x)
                switch (transitionFunction(seatingRows, y, x, rowCount, seatsPerRow)) {
                    case 0:
                        break; // no change
                    case 1:
                        seatingRows[y][x].newState = SEATED;
                        changed = 1;
                        ++occupied;
                        break;
                    case -1:
                        seatingRows[y][x].newState = EMPTY;
                        changed = 1;
                        --occupied;
                        break;
                }
        updateState(seatingRows, rowCount, seatsPerRow);
    } while (changed);
    return occupied;
}

static void readInput(Seat ***seatingRows, int *rowCount, int *seatsPerRow) {
    FILE *input = fopen("../challenges/day11_seating.txt", "r");
    if (input == NULL) {
        perror("Could not open file");
        exit(1);
    }
    size_t bufSize = 0;
    char *buf = NULL;
    while (getline(&buf, &bufSize, input) != -1) {
        if (*seatsPerRow == 0)
            *seatsPerRow = (int)strlen(buf) - 1;
        *seatingRows = realloc(*seatingRows, ++*rowCount * sizeof(Seat *));
        (*seatingRows)[*rowCount - 1] = malloc(sizeof(Seat) * *seatsPerRow);
        bzero((*seatingRows)[*rowCount - 1], sizeof(Seat) * *seatsPerRow);
        for (int i = 0; i < *seatsPerRow; ++i)
            (*seatingRows)[*rowCount - 1][i].currState = buf[i] == 'L' ? EMPTY : FLOOR;
    }
    fclose(input);
    free(buf);
}

void seating1() {
    setbuf(stdout, NULL);
    Seat **seatingRows = NULL;
    int rowCount = 0, seatsPerRow = 0;
    readInput(&seatingRows, &rowCount, &seatsPerRow);
    printf("Answer: %d\n", isThisTheGameOfLife(seatingRows, rowCount, seatsPerRow, stateTransition_simplex));
    for (int i = 0; i < rowCount; ++i)
        free(seatingRows[i]);
    free(seatingRows);
}

void seating2() {
    setbuf(stdout, NULL);
    Seat **seatingRows = NULL;
    int rowCount = 0, seatsPerRow = 0;
    readInput(&seatingRows, &rowCount, &seatsPerRow);
    printf("Answer: %d\n", isThisTheGameOfLife(seatingRows, rowCount, seatsPerRow, stateTransition_complex));
    for (int i = 0; i < rowCount; ++i)
        free(seatingRows[i]);
    free(seatingRows);
}
