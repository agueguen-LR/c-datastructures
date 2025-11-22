/**
 * @file red-black-tree.c
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#include "red-black-tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../min-max.h"
#include "red-black-tree.inc.h"

static bool is_red(RBNode node) {
  if (node == NULL) return false;
  return node->isRed;
}

// --- Constructor and Destructor ---

static RBNode rb_node_new(const void* data, size_t size, bool isRed) {
  RBNode node = malloc(2 * sizeof(RBNode) + sizeof(bool) + size);
  if (!node) {
    return NULL;
  }
  node->left = NULL;
  node->right = NULL;
  node->isRed = isRed;
  memcpy(node->data, data, size);
  return node;
}

RBTree rb_new(size_t size, int (*cmp)(const void*, const void*), void (*del)(void*)) {
  RBTree tree =
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

static void delete_node(RBNode node, void del(void*), bool del_data) {
  if (!node) {
    return;
  }

  if (del && del_data) {
    del(node->data);
  }
  free(node);
}

static void delete_all_nodes(RBNode node, void del(void*)) {
  if (node == NULL) {
    return;
  }

  delete_all_nodes(node->left, del);
  delete_all_nodes(node->right, del);
  delete_node(node, del, true);
}

void rb_delete(RBTree tree) {
  if (!tree) {
    return;
  }

  delete_all_nodes(tree->root, tree->delete_data);
  free(tree);
}

// --- Getters ---

RBNode rb_get_root(RBTree tree) {
  if (tree == NULL) {
    return NULL;
  }
  return tree->root;
}

int rb_get_height(RBTree tree) {
  if (tree == NULL) {
    return 0;
  }
  return rb_node_get_height(tree->root);
}

int rb_node_get_height(RBNode node) {
  if (node == NULL) {
    return 0;
  } else {
    return 1 + MAX(rb_node_get_height(node->left), rb_node_get_height(node->right));
  }
}

static int rb_node_get_size(RBNode node) {
  if (node == NULL) {
    return 0;
  } else {
    return 1 + rb_node_get_size(node->left) + rb_node_get_size(node->right);
  }
}

int rb_get_size(RBTree tree) { return rb_node_get_size(tree->root); }

RBNode rb_node_get_left(RBNode node) {
  if (node == NULL) {
    return NULL;
  }
  return node->left;
}

RBNode rb_node_get_right(RBNode node) {
  if (node == NULL) {
    return NULL;
  }
  return node->right;
}

void* rb_node_get_data(RBNode node) {
  if (node == NULL) {
    return NULL;
  }
  return node->data;
}

bool rb_node_is_red(RBNode node) { return is_red(node); }

// --- Rotations and Rebalancing ---

static void flip_colors(RBNode node) {
  node->isRed = !node->isRed;
  if (node->left != NULL) node->left->isRed = !node->left->isRed;
  if (node->right != NULL) node->right->isRed = !node->right->isRed;
}

static RBNode rotate_left(RBNode node) {
  if (node == NULL || node->right == NULL) {
    return node;
  }

  RBNode r_node = node->right;
  node->right = r_node->left;
  r_node->left = node;
  r_node->isRed = node->isRed;
  node->isRed = true;

  return r_node;
}

static RBNode rotate_right(RBNode node) {
  if (node == NULL || node->left == NULL) {
    return node;
  }

  RBNode l_node = node->left;
  node->left = l_node->right;
  l_node->right = node;
  l_node->isRed = node->isRed;
  node->isRed = true;

  return l_node;
}

static RBNode rb_fixup(RBNode* node) {
  if (is_red((*node)->right) && !is_red((*node)->left)) *node = rotate_left(*node);
  if (is_red((*node)->left) && is_red((*node)->left->left)) *node = rotate_right(*node);
  if (is_red((*node)->left) && is_red((*node)->right)) flip_colors(*node);

  return *node;
}

// rb_node_remove LLRB tree operation for easier deletion (fewer "cases" to handle)
static RBNode rb_move_red_right(RBNode* node) {
  flip_colors(*node);
  if ((*node)->left != NULL && is_red((*node)->left->left)) {
    *node = rotate_right(*node);
    flip_colors(*node);
  }
  return *node;
}

// rb_node_remove LLRB tree operation for easier deletion (fewer "cases" to handle)
static RBNode rb_move_red_left(RBNode* node) {
  flip_colors(*node);
  if ((*node)->right != NULL && is_red((*node)->right->left)) {
    (*node)->right = rotate_right((*node)->right);
    *node = rotate_left(*node);
    flip_colors(*node);
  }
  return *node;
}

// --- Insertion ---

// Creating a left-leaning red-black (LLRB) tree by adding rule that red nodes must be a left child
// This simplifies a lot of code by removing many of the "cases" that have to be managed during deletion
// see: https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf
// also: https://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html
static RBNode rb_node_add(RBNode* node, const void* data, size_t size, int (*compare)(const void*, const void*)) {
  if (*node == NULL) {
    return rb_node_new(data, size, true);
  }

  int cmp = compare(data, (*node)->data);
  if (cmp < 0)
    (*node)->left = rb_node_add(&(*node)->left, data, size, compare);
  else if (cmp > 0)
    (*node)->right = rb_node_add(&(*node)->right, data, size, compare);

  return rb_fixup(node);
}

void rb_add(RBTree tree, const void* data) {
  tree->root = rb_node_add(&tree->root, data, tree->data_size, tree->compare);
  tree->root->isRed = false;
}

// --- Search ---

RBNode rb_find_node(RBTree tree, const void* data) {
  RBNode current = tree->root;

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

void* rb_find_data(RBTree tree, const void* data) {
  RBNode node = rb_find_node(tree, data);
  return node ? node->data : NULL;
}

static RBNode rb_get_min_node(RBNode node) {
  if (node->left == NULL) return node;
  return rb_get_min_node(node->left);
}

// --- Deletion ---
// see: https://www.teachsolaisgames.com/articles/balanced_left_leaning.html (better comments than original paper)

static RBNode rb_node_remove_min(RBNode* node, const void* data, int (*compare)(const void*, const void*),
                                 void (*del)(void*), bool del_data) {
  if ((*node)->left == NULL) {
    delete_node(*node, del, del_data);
    return NULL;
  }

  if (!is_red((*node)->left) && (*node)->left != NULL && !is_red((*node)->left->left)) {
    *node = rb_move_red_left(node);
  }

  (*node)->left = rb_node_remove_min(&(*node)->left, data, compare, del, del_data);

  return rb_fixup(node);
}

// avoids "double-black" scenarios ("2-nodes" in 2-3-4 trees) by forcing the branch we descend into to never have two
// blacks in a row This simplifies the actual deletion of the node, but can cause some extra unnecessary operations
// during descent Any two reds in a row caused by these operations are fixed during ascent with the same rb_fixup as
// rb_node_add
static RBNode rb_node_remove(RBNode* node, const void* data, size_t size, int (*compare)(const void*, const void*),
                             void (*del)(void*)) {
  if (compare(data, (*node)->data) < 0) {
    if (!is_red((*node)->left) && (*node)->left != NULL && !is_red((*node)->left->left)) {
      *node = rb_move_red_left(node);
    }
    (*node)->left = rb_node_remove(&(*node)->left, data, size, compare, del);

  } else {
    if (is_red((*node)->left)) {
      *node = rotate_right(*node);
    }

    // node is leaf, explanation: https://stackoverflow.com/questions/13360369/deletion-in-left-leaning-red-black-trees
    if (compare(data, (*node)->data) == 0 && (*node)->right == NULL) {
      delete_node(*node, del, true);
      return NULL;
    }

    if (!is_red((*node)->right) && (*node)->right != NULL && !is_red((*node)->right->left)) {
      *node = rb_move_red_right(node);
    }

    // node is internal
    if (compare(data, (*node)->data) == 0) {
			if (del) del((*node)->data);
      memcpy((*node)->data, rb_get_min_node((*node)->right)->data, size);
      (*node)->right = rb_node_remove_min(&(*node)->right, data, compare, del, false);
    }

    else
      (*node)->right = rb_node_remove(&(*node)->right, data, size, compare, del);
  }

  return rb_fixup(node);
}

void rb_remove(RBTree tree, const void* data) {
  tree->root = rb_node_remove(&tree->root, data, tree->data_size, tree->compare, tree->delete_data);
  if (tree->root != NULL) tree->root->isRed = false;
}
