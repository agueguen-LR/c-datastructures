#include "avl-tree.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

int cmpInt(const void *a, const void *b) {
	int int_a = *(int *)a;
	int int_b = *(int *)b;
	if (int_a < int_b) return -1;
	if (int_a > int_b) return 1;
	return 0;
}

int testVals[10] = {5, 2, 8, 1, 3, 7, 9, 4, 6, 0};

int main(void) {

  AVLTree tree = tree_new(&testVals[0], sizeof(int), cmpInt, NULL);
	assert(tree != NULL);
	tree_delete(tree);

  return EXIT_SUCCESS;
}
