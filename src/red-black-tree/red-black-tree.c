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

// --- Constructor and Destructor ---

static RBNode rb_node_new(const void* data, size_t size, RBNode parent, bool isRed) {
  RBNode node = malloc(3 * sizeof(RBNode) + sizeof(bool) + sizeof(size_t) + size);
  if (!node) {
    return NULL;
  }
  node->parent = parent;
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

static void delete_node(RBNode node, void del(void*)) {
  if (!node) {
    return;
  }

  if (del) {
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
  delete_node(node, del);
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

bool rb_node_is_red(RBNode node) {
  if (node == NULL) {
    return 0;
  }
  return node->isRed;
}

// --- Rotations and Rebalancing ---

static void rotate_right(RBNode node);

static void rotate_left(RBNode node) {
  if (node == NULL || node->right == NULL) {
    return;
  }

  RBNode r_node = node->right;
  node->right = r_node->left;

  if (r_node->left != NULL) {
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
}

static void rotate_right(RBNode node) {
  if (node == NULL || node->left == NULL) {
    return;
  }

  RBNode l_node = node->left;
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
}

// static void rebalance(RBNode node) {
//   node->balance = rb_node_get_height(node->left) - rb_node_get_height(node->right);
//
//   if (node->balance < -1) {
//     rotate_left(node);
//   } else if (node->balance > 1) {
//     rotate_right(node);
//   }
// }

// --- Insertion ---

static bool rb_node_add(RBNode node, const void* data, size_t size, int (*compare)(const void*, const void*)) {
  if (node == NULL) {
    return false;
  }
  if (node->left != NULL && node->right != NULL && node->left->isRed && node->right->isRed) {
    node->left->isRed = false;
    node->right->isRed = false;
    if (node->parent != NULL) {
      node->isRed = true;
    }
  }

  RBNode* next_node;

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
      perror("Adding duplicate data is not allowed in rb_node_add");
      return false;
    default:
      perror("Unexpected comparison result in rb_node_add");
      return false;
  }

  if (*next_node == NULL) {
    RBNode new_node = rb_node_new(data, size, node, true);
    if (!new_node) {
      return false;
    }
    *next_node = new_node;
  } else {
    // don't do recursion if adding node
    if (!rb_node_add(*next_node, data, size, compare)) return false;
  }

  if (node->isRed && (*next_node) != NULL && (*next_node)->isRed) {
    node->parent->isRed = true;
    RBNode rotator = node->parent;
    if (rotator->left == node) {
      if (node->right == *next_node) {
        (*next_node)->isRed = false;
        rotate_left(node);
      } else
        node->isRed = false;
      rotate_right(rotator);
    } else {
      if (node->left == *next_node) {
        (*next_node)->isRed = false;
        rotate_right(node);
      } else
        node->isRed = false;
      rotate_left(rotator);
    }
  }
  return true;
}

bool rb_add(RBTree tree, const void* data) {
  if (tree->root == NULL) {
    RBNode new_node = rb_node_new(data, tree->data_size, NULL, false);
    if (!new_node) {
      return false;
    }
    tree->root = new_node;
    return true;
  }

  if (rb_find_node(tree, data) != NULL) {
    // Data already exists in the tree
    printf("Data already exists in the tree, ignoring.\n");
    return false;
  }

  if (!rb_node_add(tree->root, data, tree->data_size, tree->compare)) {
    return false;
  }
  // update root node after potential rotations
  while (tree->root->parent != NULL) {
    tree->root = tree->root->parent;
  }
  return true;
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

static RBNode tree_get_min_node(RBNode node) {
  if (node->left == NULL) return node;
  return tree_get_min_node(node->left);  // ! Not necessarily a leaf, can have a right branch !
}

static RBNode tree_get_max_node(RBNode node) {
  if (node->right == NULL) return node;
  return tree_get_max_node(node->right);  // ! Not necessarily a leaf, can have a left branch !
}

// --- Deletion ---

// bool rb_remove(RBTree tree, const void* data) {
//   if (tree->root == NULL) return false;
//
//   RBNode node = rb_find_node(tree, data);
//   if (node == NULL) {
//     return false;
//   }
//   RBNode parent = node->parent;
//
//   int child_count = (node->left != NULL) + (node->right != NULL);
//   switch (child_count) {
//     case 2:
//       RBNode substitute = tree_get_min_node(node->right);  // Get in-order successor
//
//       // store node data temporarily, substitute will need to have it when delete_data is called in delete_node
//       void* temp = malloc(tree->data_size);
//       if (!temp) return false;
//       memcpy(temp, node->data, tree->data_size);
//
//       memcpy(node->data, substitute->data, tree->data_size);
//
//       if (substitute->right != NULL) rotate_left(substitute);  // logically, min now has to be a leaf
//       parent = substitute->parent;                             // Rebalancing will start from substitute's parent
//
//       if (parent->right == substitute) {
//         parent->right = NULL;  // Special case where in-order successor is direct child
//       } else
//         parent->left = NULL;
//
//       memcpy(substitute->data, temp, tree->data_size);  // put back original data to be deleted for delete_data call
//       free(temp);
//       delete_node(substitute, tree->delete_data);
//       break;
//
//     case 1:
//       RBNode child = (node->left != NULL) ? node->left : node->right;
//       if (parent != NULL) {
//         if (parent->left == node) {
//           parent->left = child;
//         } else {
//           parent->right = child;
//         }
//         child->parent = parent;
//       } else {
//         child->parent = NULL;
//         tree->root = child;
//       }
//       delete_node(node, tree->delete_data);
//       break;
//
//     case 0:
//       if (parent != NULL) {
//         if (parent->left == node) {
//           parent->left = NULL;
//         } else {
//           parent->right = NULL;
//         }
//       } else {
//         // Removing the last node in the tree
//         tree->root = NULL;
//         delete_node(node, tree->delete_data);
//         return true;
//       }
//       delete_node(node, tree->delete_data);
//       break;
//
//     default:
//       perror("Unexpected child count in rb_remove");
//   }
//
//   while (parent != NULL) {
//     rebalance(parent);
//     parent = parent->parent;
//   }
//   // update root node after potential rotations
//   while (tree->root->parent != NULL) {
//     tree->root = tree->root->parent;
//   }
//
//   return true;
// }
