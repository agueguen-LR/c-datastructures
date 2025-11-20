#include "red-black-tree.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void printShort(RBNode node) {
  if (rb_node_is_red(node)) {
    printf("\033[31m%d\033[0m\n", **(uint16_t**)(rb_node_get_data(node)));
  } else {
    printf("%d\n", **(uint16_t**)(rb_node_get_data(node)));
  }
}

void printTree(RBNode node, int current_depth, int LR, void printfunc(RBNode tree)) {
  if (current_depth > 10) {
    printf("Houston, we have a problem");
    return;
  }
  assert(LR == 0 || LR == -1 || LR == 1);
  if (node) {
    printTree(rb_node_get_left(node), current_depth + 1, 1, printfunc);
    for (int i = 0; i < current_depth; i++) {
      printf("  ");
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
    printTree(rb_node_get_right(node), current_depth + 1, -1, printfunc);
  }
}

int cmpShortPtr(const void* a, const void* b) {
  uint16_t int_a = **(uint16_t**)a;
  uint16_t int_b = **(uint16_t**)b;
  if (int_a < int_b) return -1;
  if (int_a > int_b) return 1;
  return 0;
}

int cmpShort(const void* a, const void* b) {
  uint16_t int_a = *(uint16_t*)a;
  uint16_t int_b = *(uint16_t*)b;
  if (int_a < int_b) return -1;
  if (int_a > int_b) return 1;
  return 0;
}

void freeShortPtr(void* data) { free(*(uint16_t**)data); }

uint16_t testVals[10] = {10, 85, 15, 70, 20, 60, 30, 50, 65, 80};

int main(void) {
  // Emulating a situation that would need a cleanup function, like freeShortPtr here.
  uint16_t* shortPtrs[10];
  for (int i = 0; i < 10; i++) {
    shortPtrs[i] = malloc(sizeof(uint16_t));
    *shortPtrs[i] = testVals[i];
  }

  RBTree tree = rb_new(sizeof(uint16_t*), cmpShortPtr, freeShortPtr);
  assert(tree != NULL);
  assert(rb_get_height(tree) == 0);

  for (int i = 0; i < 10; i++) {
    assert(rb_add(tree, &shortPtrs[i]));
    printTree(rb_get_root(tree), 0, 0, printShort);
    printf("\n------------------\n");
  }

  assert(rb_add(tree, &shortPtrs[0]) == false);  // adding duplicate

  assert(rb_get_size(tree) == 10);

  assert(rb_get_height(tree) <= 4);

  assert(**(uint16_t**)rb_find_data(tree, &shortPtrs[5]) == 60);

  // for (int i = 0; i < 10; i++) {
  //   assert(rb_remove(tree, &shortPtrs[i]));
  //   printTree(rb_get_root(tree), 0, 0, printShort);
  //   printf("\n------------------\n");
  // }
  rb_delete(tree);

  // RBTree tree2 = rb_new(sizeof(uint16_t), cmpShort, NULL);
  // assert(rb_add(tree2, &testVals[0]));
  // assert(rb_add(tree2, &testVals[1]));
  // assert(rb_remove(tree2, &testVals[0]));
  // assert(rb_add(tree2, &testVals[2]));

  // rb_delete(tree2);

  return EXIT_SUCCESS;
}
