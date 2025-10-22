/**
 * @file avl-tree.inc.h
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.uninv-lr.fr>
 * @date 2025
 */

#pragma once

#include <stddef.h>

typedef struct _TreeNode* AVLNode;

struct _TreeNode {
  AVLNode parent;
  AVLNode left;
  AVLNode right;
  int balance;
  char data[1];
};

struct _AVLTree {
  AVLNode root;
  size_t data_size;
  int (*compare)(const void* a, const void* b);
  void (*delete_data)(void* data);
};
