#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "avl-tree.h"

int gcd(int a, int b) {
	while (b != 0) {
		int temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

int cmpShort(const void *a, const void *b) {
  uint16_t int_a = *(uint16_t *)a;
  uint16_t int_b = *(uint16_t *)b;
  if (int_a < int_b) return -1;
  if (int_a > int_b) return 1;
  return 0;
}

int main(int argc, char *argv[]) {
	if (argc != 3 || atoi(argv[1]) <= 0 || atoi(argv[1]) >= 65536){
		fprintf(stderr, "Usage: %s <number_of_nodes> <output_file_prefix>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char add_search_filename[256];
	snprintf(add_search_filename, sizeof(add_search_filename), "%s_add_search.csv", argv[2]);
	FILE* file = fopen(add_search_filename, "ax");
	if (!file) {
			fprintf(stderr, "Error opening file %s\nFile must not already exist\n", add_search_filename);
			return EXIT_FAILURE;
	}

	char remove_filename[256];
	snprintf(remove_filename, sizeof(remove_filename), "%s_remove.csv", argv[2]);
	FILE* file2 = fopen(remove_filename, "ax");
	if (!file2) {
			fprintf(stderr, "Error opening file %s\nFile must not already exist\n", remove_filename);
			return EXIT_FAILURE;
	}

	AVLTree tree = avl_new(sizeof(uint16_t), cmpShort, NULL);

	srand(time(NULL)); // flawfinder: ignore

	// https://en.wikipedia.org/wiki/Permuted_congruential_generator
	uint16_t a = (rand() | 1) % 65536;
	uint16_t b = rand() % 65536;
	uint16_t N = atoi(argv[1]);

	for (uint16_t x = 0; x < N; x++){
		printf("\rAdd and Search Progress: %f%%", ((float)x / (N-1)) * 100);
		uint16_t val = a*x + b;

		clock_t start_time = clock();
		assert(avl_add(tree, &val));
		double time_spent_add = (double)(clock() - start_time) / CLOCKS_PER_SEC;

		if (x != 0){
			val = (a*(rand() % x) + b); // random value among those already added
		}
		start_time = clock();
		assert(*(uint16_t*)avl_find_data(tree, &val) == val);
		double time_spent_search = (double)(clock() - start_time) / CLOCKS_PER_SEC;

		fprintf(file, "%d,%f,%f\n", x+1, time_spent_add, time_spent_search);
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
	if (i == MAX_TRIES){
		printf("Could not find coprime k for N=%d after %d tries, using k=1\n", N, MAX_TRIES);
		k = 1;
	};
	uint16_t p = rand() % N;

	for (uint16_t x = 0; x < N; x++){
		printf("\rRemove Progress: %f%%", ((float)x / (N-1)) * 100);
		// Here I use another permutation among values of x, this allows me to remove in a different order than insertion
		// I do still need to use a and b to only remove values that were actually added earlier
		uint16_t Xk = (k*x + p) % N;
		uint16_t val = a*Xk + b;

		clock_t start_time = clock();
		assert(avl_remove(tree, &val));
		double time_spent_remove = (double)(clock() - start_time) / CLOCKS_PER_SEC;

		fprintf(file2, "%d,%f\n", N-x, time_spent_remove);
	};

	fclose(file);
	fclose(file2);
	avl_delete(tree);
	return EXIT_SUCCESS;
}
