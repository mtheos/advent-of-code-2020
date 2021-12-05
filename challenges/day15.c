//
// Created by Michael on 15/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "day15.h"

static void readInput(int **stream, int *streamLength) {
  FILE *input = fopen("../challenges/day15_recitation.txt", "r");
  if (input == NULL) {
    perror("Could not open file");
    exit(1);
  }
  size_t bufSize = 0;
  char *buf = NULL;
  getline(&buf, &bufSize, input); // 1 line of volume
  char *token = buf;
  while (1) {
    char *value = strtok(token, ",");
    token = NULL;
    if (value == NULL)
      break;
    *stream = realloc(*stream, ++*streamLength * sizeof(int));
    (*stream)[*streamLength - 1] = atoi(value);
  }
  fclose(input);
  free(buf);
}

static int max(int *start, const int *end) {
  int max = 0;
  start = start - 1;
  while (++start < end)
    if (*start > max)
      max = *start;
  return max;
}

static int getNthValue(int *stream, int streamLength, int n) {
  // One day memory will be so plentiful all hashmaps will be done as arrays
  // And code will write itself while we ride off into the sunset on machine learning unicorns (yay...)
  int arrayMapSize = 1 + max(&stream[0], &stream[streamLength - 1]); // last number isn't used here
  int *arrayMap = malloc(arrayMapSize * sizeof(int));
  bzero(arrayMap, arrayMapSize * sizeof(int));
  for (int i = 0; i < streamLength - 1; ++i)
    arrayMap[stream[i]] = i + 1; // fill everything but the last number with it's sequence number

  int last = *(stream + streamLength - 1);
  n -= streamLength - 1;
  while (--n) {
    if (last >= arrayMapSize) {
      arrayMap = realloc(arrayMap, (last + 1) * sizeof(int));
      bzero(arrayMap + arrayMapSize, (last + 1 - arrayMapSize) * sizeof(int));
      arrayMapSize = 1 + last;
    }
    int next = arrayMap[last];
    arrayMap[last] = streamLength;
    last = next == 0 ? 0 : streamLength - next;
    ++streamLength;
  }
  free(arrayMap);
  return last;
}

void recitation1() {
  int *stream = NULL;
  int streamLength = 0;
  readInput(&stream, &streamLength);
  int nthValue = getNthValue(stream, streamLength, 2020);
  printf("Answer: %d\n", nthValue);
  free(stream);
}

void recitation2() {
  int *stream = NULL;
  int streamLength = 0;
  readInput(&stream, &streamLength);
  int nthValue = getNthValue(stream, streamLength, 30000000);
  printf("Answer: %d\n", nthValue);
  free(stream);
}
