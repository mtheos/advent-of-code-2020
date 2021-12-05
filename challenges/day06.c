//
// Created by Michael on 06/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "day06.h"

typedef struct customsForm {
  int people;
  char questions[26];
} CustomsForm;

static void readInput(CustomsForm **customsForms, int *customsFormsCount) {
  FILE *input = fopen("../challenges/day06_customs.txt", "r");
  if (input == NULL) {
    perror("Could not open file");
    exit(1);
  }
  size_t bufSize = 0;
  char *buf = NULL;
  while (getline(&buf, &bufSize, input) != -1) {
    *customsForms = realloc(*customsForms, ++*customsFormsCount * sizeof(CustomsForm));
    CustomsForm *cf = &(*customsForms)[*customsFormsCount - 1];
    bzero(cf, sizeof(CustomsForm));
    do {
      char *line = buf - 1;
      ++cf->people;
      while (*++line != '\n' || *++line != '\0')
        cf->questions[*line - 'a'] += 1;
    } while (getline(&buf, &bufSize, input) != -1 && buf[0] != '\n');
  }
  fclose(input);
  free(buf);
}

void customs1() {
  CustomsForm *customsForms = NULL;
  int customsFormsCount = 0;
  readInput(&customsForms, &customsFormsCount);
  int sum = 0;
  for (int i = 0; i < customsFormsCount; ++i)
    for (int j = 0; j < 26; ++j)
      if (customsForms[i].questions[j] != 0)
        ++sum;
  printf("Answer: %d\n", sum);
  free(customsForms);
}

void customs2() {
  CustomsForm *customsForms = NULL;
  int customsFormsCount = 0;
  readInput(&customsForms, &customsFormsCount);
  int sum = 0;
  for (int i = 0; i < customsFormsCount; ++i)
    for (int j = 0; j < 26; ++j)
      if (customsForms[i].questions[j] == customsForms[i].people)
        ++sum;
  printf("Answer: %d\n", sum);
  free(customsForms);
}
