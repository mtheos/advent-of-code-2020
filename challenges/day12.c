//
// Created by Michael on 12/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "day12.h"

typedef enum {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3, // selected so heading // 90 = N/E/S/W
    FORWARD,
    LEFT,
    RIGHT
} Movement;

typedef struct n {
    Movement action;
    int amount;
} NavPoint;

typedef struct waypoint {
    int nsPos; // +ve = north
    int ewPos; // +ve = east
} Waypoint;

typedef struct ship {
    int heading;
    int nsPos; // +ve = north
    int ewPos; // +ve = east
    Waypoint wp;
} Ship;

static void readInput(NavPoint **instructions, int *instructionCount) {
    FILE *input = fopen("../challenges/day12_evasion.txt", "r");
    if (input == NULL) {
        perror("Could not open file");
        exit(1);
    }
    size_t bufSize = 0;
    char *buf = NULL;
    while (getline(&buf, &bufSize, input) != -1) {
        *instructions = realloc(*instructions, ++*instructionCount * sizeof(NavPoint));
        NavPoint *point = &(*instructions)[*instructionCount - 1];
        point->amount = atoi(buf + 1);
        switch (buf[0]) {
            case 'N':
                point->action = NORTH;
                break;
            case 'S':
                point->action = SOUTH;
                break;
            case 'E':
                point->action = EAST;
                break;
            case 'W':
                point->action = WEST;
                break;
            case 'F':
                point->action = FORWARD;
                break;
            case 'L':
                point->action = LEFT;
                break;
            case 'R':
                point->action = RIGHT;
                break;
        }
    }
    fclose(input);
    free(buf);
}

static void theEasyWay(Ship *s, NavPoint n) {
    switch (n.action) {
        case NORTH:
            s->nsPos += n.amount;
            break;
        case SOUTH:
            s->nsPos -= n.amount;
            break;
        case EAST:
            s->ewPos += n.amount;
            break;
        case WEST:
            s->ewPos -= n.amount;
            break;
        case FORWARD: ; // empty statement
            Movement m = s->heading / 90;
            assert(m >= 0 && m <= 3);
            n.action = m;
            theEasyWay(s, n);
            break;
        case LEFT:
            s->heading = (s->heading - n.amount) % 360;
            break;
        case RIGHT:
            s->heading = (s->heading + n.amount) % 360;
            break;
    }
    if (s->heading < 0)
        s->heading += 360;
}

static void theHardWay(Ship *s, NavPoint n) {
    int temp, times;
    switch (n.action) {
        case NORTH:
            s->wp.nsPos += n.amount;
            break;
        case SOUTH:
            s->wp.nsPos -= n.amount;
            break;
        case EAST:
            s->wp.ewPos += n.amount;
            break;
        case WEST:
            s->wp.ewPos -= n.amount;
            break;
        case FORWARD:
            s->nsPos += s->wp.nsPos * n.amount;
            s->ewPos += s->wp.ewPos * n.amount;
            break;
        case LEFT:  // Every 90deg moves a point N -> W -> S -> E -> N (i.e. 4N.10E => 4W.10N), take care to flip magnitude when going N->W and S->E (i.e. moving ns to ew)
            times = n.amount / 90;
            while (--times >= 0) {
                temp = s->wp.ewPos;
                s->wp.ewPos = -s->wp.nsPos;
                s->wp.nsPos = temp;
            }
            break;
        case RIGHT: // Every 90deg moves a point N -> E -> S -> W -> N (i.e. 4N.10E => 4E.10S), take care to flip magnitude when going E->S and W->N (i.e. moving ew to ns)
            times = n.amount / 90;
            while (--times >= 0) {
                temp = s->wp.nsPos;
                s->wp.nsPos = -s->wp.ewPos;
                s->wp.ewPos = temp;
            }
            break;
    }
}

static void navigate(Ship *s, NavPoint *instructions, int instructionsCount, void (*moveFunction)(Ship *, NavPoint)) {
    for (int i = 0; i < instructionsCount; ++i)
        moveFunction(s, instructions[i]);
}

void evasion1() {
    setbuf(stdout, NULL);
    NavPoint *instructions = NULL;
    int instructionCount = 0;
    readInput(&instructions, &instructionCount);
    Ship s;
    bzero(&s, sizeof(Ship));
    s.heading = 90;
    navigate(&s, instructions, instructionCount, theEasyWay);
    int manhattan = abs(s.nsPos) + abs(s.ewPos);
    printf("Answer: %d\n", manhattan);
    free(instructions);
}

void evasion2() {
    setbuf(stdout, NULL);
    NavPoint *instructions = NULL;
    int instructionCount = 0;
    readInput(&instructions, &instructionCount);
    Ship s;
    bzero(&s, sizeof(Ship));
    s.wp.ewPos = 10;
    s.wp.nsPos = 1;
    navigate(&s, instructions, instructionCount, theHardWay);
    int manhattan = abs(s.nsPos) + abs(s.ewPos);
    printf("Answer: %d\n", manhattan);
    free(instructions);
}
