//
// Created by Michael on 14/12/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "day14.h"

typedef struct dockingData {
  union data {
    ulong value;
    char mask[37];
  } Data;
  ulong address;
} DockingData;

typedef struct memory {
  ulong value;
  ulong address;
} Memory;

static void readInput(DockingData **dockingData, int *dataLength) {
  FILE *input = fopen("../challenges/day14_docking.txt", "r");
  if (input == NULL) {
    perror("Could not open file");
    exit(1);
  }
  size_t bufSize = 0;
  char *buf = NULL;
  while (getline(&buf, &bufSize, input) != -1) {
    *dockingData = realloc(*dockingData, ++*dataLength * sizeof(DockingData));
    DockingData *dd = &(*dockingData)[*dataLength - 1];
    bzero(dd, sizeof(DockingData));
    char *x = strstr(buf, "mem");
    if (x == NULL) { // mask
      sscanf(buf, "mask = %s", dd->Data.mask);
    } else { // mem
      sscanf(buf, "mem[%ld] = %ld", &dd->address, &dd->Data.value);
    }
  }
  fclose(input);
  free(buf);
}

static Memory *findAddress(Memory *m, int memoryLength, ulong address) {
  for (int i = 0; i < memoryLength; ++i)
    if (m[i].address == address)
      return &m[i];
  return NULL;
}

static void maskValue(ulong *value, const char *mask, char zeroChar, char oneChar) {
  int shift = 36;
  --mask;
  while (*++mask != '\0') {
    --shift;
    if (*mask == zeroChar)
      *value &= ~(1UL << shift);
    else if (*mask == oneChar)
      *value |= 1UL << shift;
  }
}

static void updateMemoryV1(Memory *m, ulong value, const char *mask) {
  maskValue(&value, mask, '0', '1');
  m->value = value;
}

static Memory *initialiseProgramV1(const DockingData *dockingData, int dataLength, int *memoryLength) {
  Memory *memory = NULL;
  char const *curMask = NULL;
  for (int i = 0; i < dataLength; ++i) {
    const DockingData *dd = &dockingData[i];
    if (dd->address == 0) { // mask instruction
      curMask = dd->Data.mask;
    } else {
      Memory *m = findAddress(memory, *memoryLength, dd->address);
      if (m == NULL) {
        memory = realloc(memory, ++*memoryLength * sizeof(Memory));
        m = &memory[*memoryLength - 1];
        bzero(m, sizeof(Memory));
        m->address = dd->address;
      }
      updateMemoryV1(m, dd->Data.value, curMask);
    }
  }
  return memory;
}

static Memory *writeMemoryV2(Memory *memory, int *memoryLength, const ulong address, const ulong value) {
  Memory *m = findAddress(memory, *memoryLength, address);
  if (m == NULL) {
    memory = realloc(memory, ++*memoryLength * sizeof(Memory));
    m = &memory[*memoryLength - 1];
    bzero(m, sizeof(Memory));
    m->address = address;
  }
  m->value = value;
  return memory;
}

static Memory *updateMemoryV2(Memory *memory, int *memoryLength, ulong address, const ulong value, const char *mask) {
  maskValue(&address, mask, 'X', '1');
  memory = writeMemoryV2(memory, memoryLength, address, value);
  const char *end = mask + 36; // addresses and masks are 36 long (this takes us to the null byte)
  while (1) {
    const char *maskPtr = end;
    int shift = -1;
    while (--maskPtr != mask - 1) {
      ++shift;
      if (*maskPtr != 'X')
        continue;
      if ((address & 1UL << shift) == 0) {
        address |= 1UL << shift;
        break;
      }
      address &= ~(1UL << shift);
    }
    if (maskPtr == mask - 1)
      break;
    memory = writeMemoryV2(memory, memoryLength, address, value);
  }
  return memory;
}

static Memory *initialiseProgramV2(const DockingData *dockingData, int dataLength, int *memoryLength) {
  Memory *memory = NULL;
  char const *curMask = NULL;
  for (int i = 0; i < dataLength; ++i) {
    const DockingData *dd = &dockingData[i];
    if (dd->address == 0) // mask instruction
      curMask = dd->Data.mask;
    else
      memory = updateMemoryV2(memory, memoryLength, dd->address, dd->Data.value, curMask);
  }
  return memory;
}

void docking1() {
  setbuf(stdout, NULL);
  DockingData *dockingData = NULL;
  int dataLength = 0, memoryLength = 0;
  readInput(&dockingData, &dataLength);
  Memory *m = initialiseProgramV1(dockingData, dataLength, &memoryLength);
  ulong sum = 0;
  for (int i = 0; i < memoryLength; ++i)
    sum += m[i].value;
  printf("Answer: %ld\n", sum);
  free(dockingData);
  free(m);
}

void docking2() {
  setbuf(stdout, NULL);
  DockingData *dockingData = NULL;
  int dataLength = 0, memoryLength = 0;
  readInput(&dockingData, &dataLength);
  Memory *m = initialiseProgramV2(dockingData, dataLength, &memoryLength);
  ulong sum = 0;
  for (int i = 0; i < memoryLength; ++i)
    sum += m[i].value;
  printf("Answer: %ld\n", sum);
  free(dockingData);
  free(m);
}
