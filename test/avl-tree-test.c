#include "avl-tree.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void printInt(AVLNode node) { printf("%d:%d\n", *(int *)(avl_node_get_data(node)), avl_node_get_balance(node)); }

void printStr(AVLNode node) { printf("%s:%d\n", (char *)(avl_node_get_data(node)), avl_node_get_balance(node)); }

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

int cmpInt(const void *a, const void *b) {
  int int_a = *(int *)a;
  int int_b = *(int *)b;
  if (int_a < int_b) return -1;
  if (int_a > int_b) return 1;
  return 0;
}

int testVals[10] = {0, 2, 1, 8, 3, 7, 9, 4, 6, 5};

int main(void) {
  AVLTree tree = avl_new(&testVals[0], sizeof(int), cmpInt, NULL);
  assert(tree != NULL);
  assert(avl_get_root(tree) != NULL);
  assert(avl_node_get_height(avl_get_root(tree)) == 1);

  for (int i = 1; i < 10; i++) {
    assert(avl_add(tree, &testVals[i]));
    printTree(avl_get_root(tree), 0, 0, printInt);
    printf("\n------------------\n");
  }

  assert(avl_get_height(tree) <= 4);

  assert(*(int *)avl_find_data(tree, &testVals[5]) == 7);

  avl_delete(tree);

  return EXIT_SUCCESS;
}
