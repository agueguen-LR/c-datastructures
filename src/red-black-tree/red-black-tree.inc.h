/**
 * @file red-black-tree.inc.h
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct _TreeNode* RBNode;

struct _TreeNode {
  RBNode parent;
  RBNode left;
  RBNode right;
  bool isRed;
  char data[1];
};

struct _RBTree {
  RBNode root;
  size_t data_size;
  int (*compare)(const void* a, const void* b);
  void (*delete_data)(void* data);
};
