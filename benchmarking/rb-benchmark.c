/**
 * @file rb-benchmark.c
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "benchmark.h"
#include "red-black-tree.h"

RBTree tree;

void avl_add_wrapper(const void* data) { rb_add(tree, data); }

bool avl_search_wrapper(const void* data) { return rb_find_data(tree, data) != NULL; }

void avl_remove_wrapper(const void* data) { rb_remove(tree, data); }

bool avl_verify_wrapper() { return rb_is_valid(tree); }

int main(int argc, char* argv[]) {
  if (argc != 3 || atoi(argv[1]) <= 0 || atoi(argv[1]) >= 65536) {
    fprintf(stderr, "Usage: %s <number_of_nodes> <output_file_prefix>\n", argv[0]);
    return EXIT_FAILURE;
  }

  tree = rb_new(sizeof(uint16_t), benchmark_compare, benchmark_delete);

  benchmark(argv[2], atoi(argv[1]), &avl_add_wrapper, &avl_remove_wrapper, &avl_search_wrapper, &avl_verify_wrapper);

  rb_delete(tree);
  return EXIT_SUCCESS;
}
