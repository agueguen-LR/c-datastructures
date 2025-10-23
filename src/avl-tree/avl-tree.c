/**
 * @file avl-tree.c
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#include "avl-tree.h"

#include <stdlib.h>
#include <string.h>

#include "../min-max.h"
#include "avl-tree.inc.h"

// --- Constructor and Destructor ---

static AVLNode __avl_node_new(
		const void *data, size_t size,
		AVLNode parent
) {
	AVLNode node = malloc(3*sizeof(AVLNode) + sizeof(int) + sizeof(size_t) + size);
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

AVLTree avl_new(
		const void *data, size_t size,
		int (*cmp)(const void *, const void *),
		void (*del)(void *)
) {
  AVLTree tree = malloc(sizeof(void*) + sizeof(size_t) + sizeof(int (*)(const void*, const void*)) + sizeof(void (*)(void*)));
	if (!tree) {
		return NULL;
	}

	tree->data_size = size;
	tree->compare = cmp;
	tree->delete_data = del;
	tree->root = __avl_node_new(data, size, NULL);
	if (!tree->root) {
		free(tree);
		return NULL;
	}

	return tree;
}

static void __delete_node(AVLNode node, void del (void *)) {
	if (!node) {
		return;
	}

	if (del) {
		del(node->data);
	}
	free(node);
}

static void __delete_all_nodes(AVLNode node, void del (void *)) {
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
	if (node == NULL){
		return 0;
	} else {
		return 1 + MAX(avl_node_get_height(node->left), avl_node_get_height(node->right));
	}
}

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

	if (node->right->balance > 0){
		__rotate_right(node->right); //double rotation scenario
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

	if (node->left->balance < 0){
		__rotate_left(node->left); //double rotation scenario
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

static void __rebalance(AVLNode node){
	node->balance = avl_node_get_height(node->left) - avl_node_get_height(node->right); 

	if (node->balance < -1) {
		__rotate_left(node);
	} else if (node->balance > 1) {
		__rotate_right(node);
	} 
}

// --- Insertion ---

static bool __avl_node_add(AVLNode node, const void *data, size_t size, int (*compare)(const void *, const void *)) {
	if (node == NULL) {
		return false;
	}
	AVLNode *next_node;

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
			return true;
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

	if (!__avl_node_add(*next_node, data, size, compare)) { //recursion
		return false;
	}

	__rebalance(node);
	return true;
}

bool avl_add(AVLTree tree, const void *data) {
	if (!__avl_node_add(tree->root, data, tree->data_size, tree->compare)) {
		return false;
	}
	// update root node after potential rotations
	while (tree->root->parent != NULL) {
		tree->root = tree->root->parent;
	}
	return true;
}

// --- Deletion ---

// --- Search ---
