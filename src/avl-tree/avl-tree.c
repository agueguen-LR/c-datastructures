/**
 * @file avl-tree.c
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#include "avl-tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../min-max.h"
#include "avl-tree.inc.h"

// --- Constructor and Destructor ---

static AVLNode __avl_node_new(const void* data, size_t size, AVLNode parent) {
  AVLNode node = malloc(3 * sizeof(AVLNode) + sizeof(int) + sizeof(size_t) + size);
  if (!node) {
    return NULL;
  }
  node->parent = parent;
  node->left = NULL;
  node->right = NULL;
  node->balance = 0;
  memcpy(node->data, data, size);
  return node;
}

AVLTree avl_new(size_t size, int (*cmp)(const void*, const void*), void (*del)(void*)) {
  AVLTree tree =
      malloc(sizeof(void*) + sizeof(size_t) + sizeof(int (*)(const void*, const void*)) + sizeof(void (*)(void*)));
  if (!tree) {
    return NULL;
  }

  tree->data_size = size;
  tree->compare = cmp;
  tree->delete_data = del;
  tree->root = NULL;

  return tree;
}

static void __delete_node(AVLNode node, void del(void*)) {
  if (!node) {
    return;
  }

  if (del) {
    del(node->data);
  }
  free(node);
}

static void __delete_all_nodes(AVLNode node, void del(void*)) {
  if (node == NULL) {
    return;
  }

  __delete_all_nodes(node->left, del);
  __delete_all_nodes(node->right, del);
  __delete_node(node, del);
}

void avl_delete(AVLTree tree) {
  if (!tree) {
    return;
  }

  __delete_all_nodes(tree->root, tree->delete_data);
  free(tree);
}

// --- Getters ---

AVLNode avl_get_root(AVLTree tree) {
  if (tree == NULL) {
    return NULL;
  }
  return tree->root;
}

int avl_get_height(AVLTree tree) {
  if (tree == NULL) {
    return 0;
  }
  return avl_node_get_height(tree->root);
}

int avl_node_get_height(AVLNode node) {
  if (node == NULL) {
    return 0;
  } else {
    return 1 + MAX(avl_node_get_height(node->left), avl_node_get_height(node->right));
  }
}

int __avl_node_get_size(AVLNode node) {
  if (node == NULL) {
    return 0;
  } else {
    return 1 + __avl_node_get_size(node->left) + __avl_node_get_size(node->right);
  }
}

int avl_get_size(AVLTree tree) { return __avl_node_get_size(tree->root); }

AVLNode avl_node_get_left(AVLNode node) {
  if (node == NULL) {
    return NULL;
  }
  return node->left;
}

AVLNode avl_node_get_right(AVLNode node) {
  if (node == NULL) {
    return NULL;
  }
  return node->right;
}

void* avl_node_get_data(AVLNode node) {
  if (node == NULL) {
    return NULL;
  }
  return node->data;
}

int avl_node_get_balance(AVLNode node) {
  if (node == NULL) {
    return 0;
  }
  return node->balance;
}

// --- Rotations and Rebalancing ---

static void __rotate_right(AVLNode node);

static void __rotate_left(AVLNode node) {
  if (node == NULL || node->right == NULL) {
    return;
  }

  if (node->right->balance > 0) {
    __rotate_right(node->right);  // double rotation scenario
  }

  AVLNode r_node = node->right;
  node->right = r_node->left;

  if (r_node->left) {
    r_node->left->parent = node;
  }

  r_node->parent = node->parent;

  if (node->parent) {
    if (node->parent->left == node) {
      node->parent->left = r_node;
    } else {
      node->parent->right = r_node;
    }
  }

  r_node->left = node;
  node->parent = r_node;

  node->balance = node->balance + 1 - MIN(0, r_node->balance);
  r_node->balance = r_node->balance + 1 + MAX(0, node->balance);
}

static void __rotate_right(AVLNode node) {
  if (node == NULL || node->left == NULL) {
    return;
  }

  if (node->left->balance < 0) {
    __rotate_left(node->left);  // double rotation scenario
  }

  AVLNode l_node = node->left;
  node->left = l_node->right;

  if (l_node->right) {
    l_node->right->parent = node;
  }

  l_node->parent = node->parent;

  if (node->parent) {
    if (node->parent->left == node) {
      node->parent->left = l_node;
    } else {
      node->parent->right = l_node;
    }
  }

  l_node->right = node;
  node->parent = l_node;

  node->balance = node->balance - 1 - MAX(0, l_node->balance);
  l_node->balance = l_node->balance - 1 + MIN(0, node->balance);
}

static void __rebalance(AVLNode node) {
  node->balance = avl_node_get_height(node->left) - avl_node_get_height(node->right);

  if (node->balance < -1) {
    __rotate_left(node);
  } else if (node->balance > 1) {
    __rotate_right(node);
  }
}

// --- Insertion ---

static bool __avl_node_add(AVLNode node, const void* data, size_t size, int (*compare)(const void*, const void*)) {
  if (node == NULL) {
    return false;
  }
  AVLNode* next_node;

  int cmp = compare(data, node->data);
  int norm = (cmp > 0) ? 1 : (cmp < 0) ? -1 : 0;
  switch (norm) {
    case -1:
      next_node = &(node->left);
      break;
    case 1:
      next_node = &(node->right);
      break;
    case 0:
      perror("Adding duplicate data is not allowed in __avl_node_add");
      return false;
    default:
      perror("Unexpected comparison result in __avl_node_add");
      return false;
  }

  if (*next_node == NULL) {
    AVLNode new_node = __avl_node_new(data, size, node);
    if (!new_node) {
      return false;
    }
    *next_node = new_node;
    node->balance = avl_node_get_height(node->left) - avl_node_get_height(node->right);
    return true;
  }

  if (!__avl_node_add(*next_node, data, size, compare)) {  // recursion
    return false;
  }

  __rebalance(node);
  return true;
}

bool avl_add(AVLTree tree, const void* data) {
  if (tree->root == NULL) {
    AVLNode new_node = __avl_node_new(data, tree->data_size, NULL);
    if (!new_node) {
      return false;
    }
    tree->root = new_node;
    return true;
  }

  if (avl_find_node(tree, data) != NULL) {
    // Data already exists in the tree
    printf("Data already exists in the tree, ignoring.\n");
    return false;
  }

  if (!__avl_node_add(tree->root, data, tree->data_size, tree->compare)) {
    return false;
  }
  // update root node after potential rotations
  while (tree->root->parent != NULL) {
    tree->root = tree->root->parent;
  }
  return true;
}

// --- Search ---

AVLNode avl_find_node(AVLTree tree, const void* data) {
  AVLNode current = tree->root;

  while (current != NULL) {
    int cmp = tree->compare(data, current->data);
    if (cmp == 0) {
      return current;
    } else if (cmp < 0) {
      current = current->left;
    } else {
      current = current->right;
    }
  }

  return NULL;
}

void* avl_find_data(AVLTree tree, const void* data) {
  AVLNode node = avl_find_node(tree, data);
  return node ? node->data : NULL;
}

static AVLNode __tree_get_min_node(AVLNode node) {
  if (node->left == NULL) return node;
  return __tree_get_min_node(node->left);  // ! Not necessarily a leaf, can have a right branch !
}

static AVLNode __tree_get_max_node(AVLNode node) {
  if (node->right == NULL) return node;
  return __tree_get_max_node(node->right);  // ! Not necessarily a leaf, can have a left branch !
}

// --- Deletion ---

bool avl_remove(AVLTree tree, const void* data) {
  if (tree->root == NULL) return false;

  AVLNode node = avl_find_node(tree, data);
  if (node == NULL) {
    return false;
  }
  AVLNode parent = node->parent;

  int child_count = (node->left != NULL) + (node->right != NULL);
  switch (child_count) {
    case 2:
      AVLNode substitute = __tree_get_min_node(node->right);  // Get in-order successor

      // store node data temporarily, substitute will need to have it when delete_data is called in __delete_node
      void* temp = malloc(tree->data_size);
      if (!temp) return false;
      memcpy(temp, node->data, tree->data_size);

      memcpy(node->data, substitute->data, tree->data_size);

      if (substitute->right != NULL) __rotate_left(substitute);  // logically, min now has to be a leaf
      parent = substitute->parent;                               // Rebalancing will start from substitute's parent

      if (parent->right == substitute) {
        parent->right = NULL;  // Special case where in-order successor is direct child
      } else
        parent->left = NULL;

      memcpy(substitute->data, temp, tree->data_size);  // put back original data to be deleted for delete_data call
      free(temp);
      __delete_node(substitute, tree->delete_data);
      break;

    case 1:
      AVLNode child = (node->left != NULL) ? node->left : node->right;
      if (parent != NULL) {
        if (parent->left == node) {
          parent->left = child;
        } else {
          parent->right = child;
        }
        child->parent = parent;
      } else {
        child->parent = NULL;
        tree->root = child;
      }
      __delete_node(node, tree->delete_data);
      break;

    case 0:
      if (parent != NULL) {
        if (parent->left == node) {
          parent->left = NULL;
        } else {
          parent->right = NULL;
        }
      } else {
        // Removing the last node in the tree
        tree->root = NULL;
        __delete_node(node, tree->delete_data);
        return true;
      }
      __delete_node(node, tree->delete_data);
      break;

    default:
      perror("Unexpected child count in avl_remove");
  }

  while (parent != NULL) {
    __rebalance(parent);
    parent = parent->parent;
  }
  // update root node after potential rotations
  while (tree->root->parent != NULL) {
    tree->root = tree->root->parent;
  }

  return true;
}
