#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "avl-tree.h"

int cmpShort(const void *a, const void *b) {
  uint16_t int_a = *(uint16_t *)a;
  uint16_t int_b = *(uint16_t *)b;
  if (int_a < int_b) return -1;
  if (int_a > int_b) return 1;
  return 0;
}

int main(int argc, char *argv[]) {
	if (argc != 3 || atoi(argv[1]) <= 0){
		fprintf(stderr, "Usage: %s <number_of_nodes> <output_file>\n", argv[0]);
		return EXIT_FAILURE;
	}
	FILE* add_file = fopen(argv[2], "ax");
	if (!add_file){
		fprintf(stderr, "Error opening file %s\nFile must not already exist\n", argv[2]);
		return EXIT_FAILURE;
	}

	AVLTree tree = avl_new(sizeof(uint16_t), cmpShort, NULL);

	srand(time(NULL)); // flawfinder: ignore

	// https://en.wikipedia.org/wiki/Permuted_congruential_generator
	uint16_t a = (rand() | 1) % 65536;
	uint16_t b = rand() % 65536;

	for (uint16_t x = 0; x <= atoi(argv[1]); x++){
		printf("\rProgress: %f%%", ((float)x / atoi(argv[1])) * 100);
		uint16_t val = (a*x + b) & 0xFFFF;

		clock_t start_time = clock();
		avl_add(tree, &val);
		double time_spent_add = (double)(clock() - start_time) / CLOCKS_PER_SEC;

		start_time = clock();
		avl_find_data(tree, &val);
		double time_spent_search = (double)(clock() - start_time) / CLOCKS_PER_SEC;

		fprintf(add_file, "%d,%f,%f\n", x, time_spent_add, time_spent_search);
	}

	fclose(add_file);
	avl_delete(tree);
	return EXIT_SUCCESS;
}
