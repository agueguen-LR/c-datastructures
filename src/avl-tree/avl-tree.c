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

static AVLNode avl_node_new(const void* data, size_t size) {
  AVLNode node = malloc(3 * sizeof(AVLNode) + sizeof(int) + size);
  if (!node) {
    perror("Out of memory");
    exit(EXIT_FAILURE);
  }
  node->left = NULL;
  node->right = NULL;
  node->height = 1;
  memcpy(node->data, data, size);
  return node;
}

AVLTree avl_new(size_t size, int (*cmp)(const void*, const void*), void (*del)(void*)) {
  AVLTree tree =
      malloc(sizeof(void*) + sizeof(size_t) + sizeof(int (*)(const void*, const void*)) + sizeof(void (*)(void*)));
  if (!tree) {
    perror("Out of memory");
    exit(EXIT_FAILURE);
  }

  tree->data_size = size;
  tree->compare = cmp;
  tree->delete_data = del;
  tree->root = NULL;

  return tree;
}

static void delete_node(AVLNode node, void del(void*), bool del_data) {
  if (!node) {
    return;
  }

  if (del && del_data) {
    del(node->data);
  }
  free(node);
}

static void delete_all_nodes(AVLNode node, void del(void*)) {
  if (node == NULL) {
    return;
  }

  delete_all_nodes(node->left, del);
  delete_all_nodes(node->right, del);
  delete_node(node, del, true);
}

void avl_delete(AVLTree tree) {
  if (!tree) {
    return;
  }

  delete_all_nodes(tree->root, tree->delete_data);
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
    return node->height;
  }
}

static int avl_node_get_size(AVLNode node) {
  if (node == NULL) {
    return 0;
  } else {
    return 1 + avl_node_get_size(node->left) + avl_node_get_size(node->right);
  }
}

int avl_get_size(AVLTree tree) { return avl_node_get_size(tree->root); }

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

static bool avl_node_is_valid(AVLNode node) {
  if (node == NULL) return true;
  int bal = avl_node_get_height(node->left) - avl_node_get_height(node->right);
  return bal < 2 && bal > -2;
}

static bool avl_subtree_is_valid(AVLNode node) {
  if (node == NULL) return true;
  return avl_node_is_valid(node) && avl_subtree_is_valid(node->left) && avl_subtree_is_valid(node->right);
}

bool avl_is_valid(AVLTree tree) {
  if (tree->root == NULL) return true;
  return avl_subtree_is_valid(tree->root);
}

// --- Rotations and Rebalancing ---

static AVLNode rotate_right(AVLNode node);

static AVLNode rotate_left(AVLNode node) {
  if (node == NULL || node->right == NULL) {
    return node;
  }

  AVLNode r_node = node->right;
  node->right = r_node->left;

  r_node->left = node;

  node->height = 1 + MAX(avl_node_get_height(node->left), avl_node_get_height(node->right));
  r_node->height = 1 + MAX(avl_node_get_height(r_node->left), avl_node_get_height(r_node->right));

  return r_node;
}

static AVLNode rotate_right(AVLNode node) {
  if (node == NULL || node->left == NULL) {
    return node;
  }

  AVLNode l_node = node->left;
  node->left = l_node->right;

  l_node->right = node;

  node->height = 1 + MAX(avl_node_get_height(node->left), avl_node_get_height(node->right));
  l_node->height = 1 + MAX(avl_node_get_height(l_node->left), avl_node_get_height(l_node->right));

  return l_node;
}

static AVLNode rebalance(AVLNode node) {
  node->height = 1 + MAX(avl_node_get_height(node->left), avl_node_get_height(node->right));

  int balance = avl_node_get_height(node->left) - avl_node_get_height(node->right);
  if (balance < -1) {
    if (avl_node_get_height(node->right->left) > avl_node_get_height(node->right->right)) {
      node->right = rotate_right(node->right);  // double rotation RL
    }
    return rotate_left(node);
  } else if (balance > 1) {
    if (avl_node_get_height(node->left->right) > avl_node_get_height(node->left->left)) {
      node->left = rotate_left(node->left);  // double rotation LR
    }
    return rotate_right(node);
  }
  return node;
}

// --- Insertion ---

static void avl_node_add(AVLNode* node, const void* data, size_t size, int (*compare)(const void*, const void*)) {
  if (node == NULL) {
    return;
  }
  AVLNode* next_node;

  int cmp = compare(data, (*node)->data);
  if (cmp < 0) {
    next_node = &((*node)->left);
  } else if (cmp > 0) {
    next_node = &((*node)->right);
  } else {
    return;  // data already in tree
  }

  if (*next_node == NULL) {
    AVLNode new_node = avl_node_new(data, size);
    *next_node = new_node;
    (*node)->height = 1 + MAX(avl_node_get_height((*node)->left), avl_node_get_height((*node)->right));
    return;
  }

  avl_node_add(next_node, data, size, compare);

  *node = rebalance(*node);
}

void avl_add(AVLTree tree, const void* data) {
  if (tree->root == NULL) {
    AVLNode new_node = avl_node_new(data, tree->data_size);
    tree->root = new_node;
    return;
  }

  avl_node_add(&tree->root, data, tree->data_size, tree->compare);
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

static AVLNode tree_get_min_node(AVLNode node) {
  if (node->left == NULL) return node;
  return tree_get_min_node(node->left);
}

// --- Deletion ---

static AVLNode avl_node_remove(AVLNode* node, const void* data, size_t size, int (*compare)(const void*, const void*),
                               void (*del)(void*), bool del_data) {
  if (*node == NULL) {
    return NULL;
  }

  int cmp = compare(data, (*node)->data);
  if (cmp < 0) {
    (*node)->left = avl_node_remove(&(*node)->left, data, size, compare, del, del_data);
  } else if (cmp > 0) {
    (*node)->right = avl_node_remove(&(*node)->right, data, size, compare, del, del_data);
  } else {
    if ((*node)->left == NULL || (*node)->right == NULL) {  // One child or no child
      AVLNode temp = (*node)->left ? (*node)->left : (*node)->right;
      delete_node(*node, del, del_data);
      return temp;
    }

    if (del_data && del) del((*node)->data);
    AVLNode temp = tree_get_min_node((*node)->right);

    memcpy((*node)->data, temp->data, size);
    (*node)->right = avl_node_remove(&(*node)->right, temp->data, size, compare, del, false);
  }

  *node = rebalance(*node);
  return *node;
}

void avl_remove(AVLTree tree, const void* data) {
  if (tree->root == NULL) return;

  tree->root = avl_node_remove(&tree->root, data, tree->data_size, tree->compare, tree->delete_data, true);
}
