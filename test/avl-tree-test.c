#include "avl-tree.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void printShort(AVLNode node) {
	printf("%d:%d\n", **(uint16_t **)(avl_node_get_data(node)), avl_node_get_balance(node)); 
}

void printTree(AVLNode node, int current_depth, int LR, void printfunc(AVLNode tree)) {
  if (current_depth > 10) {
    printf("Houston, we have a problem");
    return;
  }
  assert(LR == 0 || LR == -1 || LR == 1);
  if (node) {
    printTree(avl_node_get_left(node), current_depth + 1, 1, printfunc);
    for (int i = 0; i < current_depth; i++) {
      printf("    ");
    }
    switch (LR) {
      case -1:
        printf("\\");
        break;
      case 1:
        printf("/");
      default:
        break;
    }
    printfunc(node);
    printTree(avl_node_get_right(node), current_depth + 1, -1, printfunc);
  }
}

int cmpShortPtr(const void *a, const void *b) {
  uint16_t int_a = **(uint16_t **)a;
  uint16_t int_b = **(uint16_t **)b;
  if (int_a < int_b) return -1;
  if (int_a > int_b) return 1;
  return 0;
}

int cmpShort(const void *a, const void *b) {
  uint16_t int_a = *(uint16_t *)a;
  uint16_t int_b = *(uint16_t *)b;
  if (int_a < int_b) return -1;
  if (int_a > int_b) return 1;
  return 0;
}

void freeShortPtr(void* data){
	free(*(uint16_t**)data);
}

uint16_t testVals[10] = {0, 2, 1, 8, 3, 7, 9, 4, 6, 5};

int main(void) {
	// Emulating a situation that would need a cleanup function, like freeShortPtr here.
	uint16_t* shortPtrs[10];
	for (int i = 0; i < 10; i++) {
		shortPtrs[i] = malloc(sizeof(uint16_t));
		*shortPtrs[i] = testVals[i];
	}

  AVLTree tree = avl_new(sizeof(uint16_t*), cmpShortPtr, freeShortPtr);
  assert(tree != NULL);
  assert(avl_get_height(tree) == 0);

  for (int i = 0; i < 10; i++) {
    assert(avl_add(tree, &shortPtrs[i]));
    printTree(avl_get_root(tree), 0, 0, printShort);
    printf("\n------------------\n");
  }

  assert(avl_get_height(tree) <= 4);

  assert(**(uint16_t **)avl_find_data(tree, &shortPtrs[5]) == 7);

  for (int i = 0; i < 10; i++) {
    assert(avl_remove(tree, &shortPtrs[i]));
    printTree(avl_get_root(tree), 0, 0, printShort);
    printf("\n------------------\n");
  }
  avl_delete(tree);

	AVLTree tree2 = avl_new(sizeof(uint16_t), cmpShort, NULL);
  assert(avl_add(tree2, &testVals[0]));
  assert(avl_add(tree2, &testVals[1]));
  assert(avl_remove(tree2, &testVals[0]));
  assert(avl_add(tree2, &testVals[2]));

  avl_delete(tree2);

  return EXIT_SUCCESS;
}
