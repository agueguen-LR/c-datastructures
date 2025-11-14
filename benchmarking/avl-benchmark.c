/**
 * @file avl-benchmark.c
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "avl-tree.h"
#include "benchmark.h"

int cmpShort(const void *a, const void *b) {
  uint16_t int_a = *(uint16_t *)a;
  uint16_t int_b = *(uint16_t *)b;
  if (int_a < int_b) return -1;
  if (int_a > int_b) return 1;
  return 0;
}

AVLTree tree;

bool avl_add_wrapper(const void* data){
	return avl_add(tree, data);
}

bool avl_search_wrapper(const void* data){
	return avl_find_data(tree, data) != NULL;
}

bool avl_remove_wrapper(const void* data){
	return avl_remove(tree, data);
}

int main(int argc, char *argv[]) {
	if (argc != 3 || atoi(argv[1]) <= 0 || atoi(argv[1]) >= 65536){
		fprintf(stderr, "Usage: %s <number_of_nodes> <output_file_prefix>\n", argv[0]);
		return EXIT_FAILURE;
	}

	tree = avl_new(sizeof(uint16_t), cmpShort, NULL);

	benchmark(argv[2], atoi(argv[1]), &avl_add_wrapper, &avl_remove_wrapper, &avl_search_wrapper);

	avl_delete(tree);
	return EXIT_SUCCESS;
}
