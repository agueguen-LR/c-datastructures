/**
 * @file benchmark.c
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#include "benchmark.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void benchmark_delete(void* data) { return; }

int benchmark_compare(const void* a, const void* b) {
  uint32_t int_a = *(uint32_t*)a;
  uint32_t int_b = *(uint32_t*)b;
  if (int_a < int_b) return -1;
  if (int_a > int_b) return 1;
  return 0;
}

int benchmark(char* output_file_prefix, int number_of_nodes, int batch_size, void add(const void*),
              void remove(const void*), bool search(const void*), bool verify()) {
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

  uint32_t a = (rand() | 1) % BENCHMARK_MAX_NODES;  // 2 ** 20 MAX
  uint32_t b = rand() % BENCHMARK_MAX_NODES;
  uint32_t N = number_of_nodes;

  for (uint32_t x = 0; x < N; x += batch_size) {
    uint32_t batch_end = (x + batch_size < N) ? x + batch_size : N;
    printf("\rAdd and Search Progress: %f%%", ((double)(batch_end - 1) / (N - 1)) * 100);

    clock_t start_time = clock();
    for (uint32_t i = x; i < batch_end; i++) {
      const uint32_t val = (a * i + b) % BENCHMARK_MAX_NODES;
      add(&val);
    }
    double time_spent_add = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    assert(verify());
    fprintf(file_add, "%d,%f\n", batch_end, time_spent_add);

    start_time = clock();
    for (uint32_t i = x; i < batch_end; i++) {
      uint32_t val = (a * (rand() % (batch_end)) + b) % BENCHMARK_MAX_NODES;
      assert(search(&val));
    }
    double time_spent_search = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    fprintf(file_search, "%d,%f\n", batch_end, time_spent_search);
  }

  printf("\n");

  uint32_t p = rand() % N;
  for (uint32_t x = 0; x < N; x += batch_size) {
    uint32_t batch_end = (x + batch_size < N) ? x + batch_size : N;
    printf("\rRemove Progress: %f%%", ((double)(batch_end - 1) / (N - 1)) * 100);

    clock_t start_time = clock();
    for (uint32_t i = x; i < batch_end; i++) {
      uint32_t Xk = (i + p) % N;  // offset by random p to avoid removing in same order as added
      const uint32_t val = (a * Xk + b) % BENCHMARK_MAX_NODES;
      remove(&val);
    }
    double time_spent_remove = (double)(clock() - start_time) / CLOCKS_PER_SEC;
    assert(verify());
    fprintf(file_remove, "%d,%f\n", N - batch_end, time_spent_remove);
  };

  fclose(file_add);
  fclose(file_search);
  fclose(file_remove);
  return EXIT_SUCCESS;
}
