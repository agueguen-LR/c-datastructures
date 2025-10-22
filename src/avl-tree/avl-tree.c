/**
 * @file avl-tree.c
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#include "avl-tree.h"

#include <stdlib.h>
#include <string.h>

#include "avl-tree.inc.h"

AVLTree tree_new(
		const void *data,
		size_t size,
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

	AVLNode root = malloc(3*sizeof(AVLNode) + sizeof(size_t) + size);
	if (!root) {
		free(tree);
		return NULL;
	}
	tree->root = root;
	root->parent = NULL;
	root->left = NULL;
	root->right = NULL;
	root->balance = 0;
	memcpy(root->data, data, size);
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

void tree_delete(AVLTree tree) {
	if (!tree) {
		return;
	}
  
	__delete_all_nodes(tree->root, tree->delete_data);
	free(tree);
}
