/**
 * @file avl-benchmark.c
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "avl-tree.h"
#include "benchmark.h"

AVLTree tree;

void avl_add_wrapper(const void* data) { avl_add(tree, data); }

bool avl_search_wrapper(const void* data) { return avl_find_data(tree, data) != NULL; }

void avl_remove_wrapper(const void* data) { avl_remove(tree, data); }

bool avl_verify_wrapper() { return avl_is_valid(tree); }

int main(int argc, char* argv[]) {
  if (argc != 4 || atoi(argv[1]) <= 0 || atoi(argv[1]) >= BENCHMARK_MAX_NODES || atoi(argv[2]) <= 0 ||
      atoi(argv[2]) > atoi(argv[1])) {
    fprintf(stderr, "Usage: %s <number_of_nodes> <batch_size> <output_file_prefix>\n", argv[0]);
    return EXIT_FAILURE;
  }

  tree = avl_new(BENCHMARK_DATA_SIZE, benchmark_compare, benchmark_delete);

  benchmark(argv[3], atoi(argv[1]), atoi(argv[2]), &avl_add_wrapper, &avl_remove_wrapper, &avl_search_wrapper,
            &avl_verify_wrapper);

  avl_delete(tree);
  return EXIT_SUCCESS;
}
