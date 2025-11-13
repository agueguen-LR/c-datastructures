/**
 * @file avl-tree.h
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>

// --- Type Definitions ---

/**
 * @brief AVL tree type.
 */
typedef struct _AVLTree* AVLTree;

/**
 * @brief AVL tree node type.
 */
typedef struct _TreeNode* AVLNode;

// --- Constructors and Destructors ---

/**
 * @brief Create a new AVL tree.
 *
 * @param size Size of the stored data in bytes.
 * @param cmp Comparison function for the data.
 * @param del Deletion function for the data.
 * @return The newly created AVL tree.
 */
extern AVLTree avl_new(size_t size, int (*cmp)(const void*, const void*), void (*del)(void*));

/**
 * @brief Delete an AVL tree, freeing all associated memory.
 *
 * @param tree The AVL tree to be deleted.
 */
extern void avl_delete(AVLTree tree);

// --- Getters ---

/**
 * @brief Get the root node of the AVL tree.
 *
 * @param tree The AVL tree.
 * @return The root node of the AVL tree.
 */
extern AVLNode avl_get_root(AVLTree tree);

/**
 * @brief Get the height of the AVL tree.
 *
 * @param tree The AVL tree.
 * @return The height of the tree.
 */
extern int avl_get_height(AVLTree tree);

/**
 * @brief Get the height of a given AVL tree node.
 *
 * @param node The AVL tree node.
 * @return The height of the node.
 */
extern int avl_node_get_height(AVLNode node);

/**
 * @brief Get the left child of a given AVL tree node.
 *
 * @param node The AVL tree node.
 * @return The left child of the node.
 */
extern AVLNode avl_node_get_left(AVLNode node);

/**
 * @brief Get the right child of a given AVL tree node.
 *
 * @param node The AVL tree node.
 * @return The right child of the node.
 */
extern AVLNode avl_node_get_right(AVLNode node);

/**
 * @brief Get the data stored in a given AVL tree node.
 *
 * @param node The AVL tree node.
 * @return Pointer to the data stored in the node.
 */
extern void* avl_node_get_data(AVLNode node);

/**
 * @brief Get the balance factor of a given AVL tree node.
 *
 * @param node The AVL tree node.
 * @return The balance factor of the node.
 */
extern int avl_node_get_balance(AVLNode node);

// --- Insertion ---

/**
 * @brief Add data to the AVL tree.
 *
 * @param tree The AVL tree where data will be inserted.
 * @param data Pointer to the data to be inserted.
 * @return true if insertion was successful, false otherwise.
 */
extern bool avl_add(AVLTree tree, const void* data);

// --- Deletion ---

/**
 * @brief Remove data from the AVL tree.
 *
 * @param tree The AVL tree from which data will be removed.
 * @param data Pointer to the data to be removed.
 * @return true if removal was successful, false otherwise.
 */
extern bool avl_remove(AVLTree tree, const void* data);

// --- Search ---

/**
 * @brief Find a node in the AVL tree containing the specified data.
 *
 * @param tree The AVL tree to search.
 * @param data Pointer to the data to search for.
 * @return The node containing the data, or NULL if not found.
 */
extern AVLNode avl_find_node(AVLTree tree, const void* data);

/**
 * @brief Find data in the AVL tree.
 *
 * @param tree The AVL tree to search.
 * @param data Pointer to the data to search for.
 * @return Pointer to the found data, or NULL if not found.
 */
extern void* avl_find_data(AVLTree tree, const void* data);
