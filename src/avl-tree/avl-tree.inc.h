/**
 * @file avl-tree.inc.h
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.uninv-lr.fr>
 * @date 2025
 */

typedef struct _TreeNode* _Tree;

struct _TreeNode {
  _Tree parent;
  _Tree left;
  _Tree right;
  int balance;
  char data[1];
};

struct _AVLTree {
  _Tree* root;
  int data_size;
  int (*compare)(const void* a, const void* b);
  int (*delete_data)(void* data);
};
