/**
 * @file avl-tree.h
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#pragma once

#include <stddef.h>

/**
 * @brief AVL tree type.
 */
typedef struct _AVLTree* AVLTree;

/**
 * @brief AVL tree node type.
 */
typedef struct _TreeNode* AVLNode;

/**
 * @brief Create a new AVL tree.
 *
 * @param data Pointer to the data to be stored in the root node.
 * @param size Size of the data in bytes.
 * @param cmp Comparison function for the data.
 * @param del Deletion function for the data.
 * @return The newly created AVL tree.
 */
AVLTree tree_new(
		const void *data,
		size_t size,
		int (*cmp)(const void *, const void *),
		void (*del)(void *)
);

/**
 * @brief Delete an AVL tree, freeing all associated memory.
 *
 * @param tree The AVL tree to be deleted.
 */
void tree_delete(AVLTree tree);
