#include "benchmark.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Get greatest common divisor of a and b
static int gcd(int a, int b) {
  while (b != 0) {
    int temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}

int benchmark(char* output_file_prefix, int number_of_nodes, bool add(const void*), bool remove(const void*),
              bool search(const void*)) {
  char add_filename[256];
  snprintf(add_filename, sizeof(add_filename), "%s_add.csv", output_file_prefix);
  FILE* file_add = fopen(add_filename, "ax");
  if (!file_add) {
    fprintf(stderr, "Error opening file %s\nFile must not already exist\n", add_filename);
    return EXIT_FAILURE;
  }

  char search_filename[256];
  snprintf(search_filename, sizeof(search_filename), "%s_search.csv", output_file_prefix);
  FILE* file_search = fopen(search_filename, "ax");
  if (!file_search) {
    fprintf(stderr, "Error opening file %s\nFile must not already exist\n", search_filename);
    return EXIT_FAILURE;
  }

  char remove_filename[256];
  snprintf(remove_filename, sizeof(remove_filename), "%s_remove.csv", output_file_prefix);
  FILE* file_remove = fopen(remove_filename, "ax");
  if (!file_remove) {
    fprintf(stderr, "Error opening file %s\nFile must not already exist\n", remove_filename);
    return EXIT_FAILURE;
  }

  srand(time(NULL));  // flawfinder: ignore

  // https://en.wikipedia.org/wiki/Permuted_congruential_generator
  uint16_t a = (rand() | 1) % 65536;
  uint16_t b = rand() % 65536;
  uint16_t N = number_of_nodes;

  for (uint16_t x = 0; x < N; x++) {
    printf("\rAdd and Search Progress: %f%%", ((float)x / (N - 1)) * 100);
    uint16_t val = a * x + b;

    clock_t start_time = clock();
    assert(add(&val));
    double time_spent_add = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    fprintf(file_add, "%d,%f\n", x + 1, time_spent_add);

    if (x != 0) {
      val = (a * (rand() % x) + b);  // random value among those already added
    }
    start_time = clock();
    assert(search(&val));
    double time_spent_search = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    fprintf(file_search, "%d,%f\n", x + 1, time_spent_search);
  }

  printf("\n");

  // This attempts to find a k coprime with N for the removal permutation
  // If it fails after MAX_TRIES, it just uses k = 1 (no permutation) but still has the offset from p
  uint16_t k;
  int i = 0;
  int MAX_TRIES = 10;
  do {
    k = (rand() | 1) % N;
    i++;
  } while (gcd(k, N) != 1 && i < MAX_TRIES);
  if (i == MAX_TRIES) {
    printf("Could not find coprime k for N=%d after %d tries, using k=1\n", N, MAX_TRIES);
    k = 1;
  };
  uint16_t p = rand() % N;

  for (uint16_t x = 0; x < N; x++) {
    printf("\rRemove Progress: %f%%", ((float)x / (N - 1)) * 100);
    // Here I use another permutation among values of x, this allows me to remove in a different order than insertion
    // I do still need to use a and b to only remove values that were actually added earlier
    uint16_t Xk = (k * x + p) % N;
    uint16_t val = a * Xk + b;

    clock_t start_time = clock();
    assert(remove(&val));
    double time_spent_remove = (double)(clock() - start_time) / CLOCKS_PER_SEC;

    fprintf(file_remove, "%d,%f\n", N - x, time_spent_remove);
  };

  fclose(file_add);
  fclose(file_search);
  fclose(file_remove);
  return EXIT_SUCCESS;
}
