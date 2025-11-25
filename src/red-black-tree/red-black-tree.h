/**
 * @file red-black-tree.h
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>

// --- Type Definitions ---

/**
 * @brief RB tree type.
 */
typedef struct _RBTree* RBTree;

/**
 * @brief RB tree node type.
 */
typedef struct _TreeNode* RBNode;

// --- Constructors and Destructors ---

/**
 * @brief Create a new RB tree.
 *
 * @param size Size of the stored data in bytes.
 * @param cmp Comparison function for the data.
 * @param del Deletion function for the data.
 * @return The newly created RB tree.
 */
extern RBTree rb_new(size_t size, int (*cmp)(const void*, const void*), void (*del)(void*));

/**
 * @brief Delete an RB tree, freeing all associated memory.
 *
 * @param tree The RB tree to be deleted.
 */
extern void rb_delete(RBTree tree);

// --- Getters ---

/**
 * @brief Get the root node of the RB tree.
 *
 * @param tree The RB tree.
 * @return The root node of the RB tree.
 */
extern RBNode rb_get_root(RBTree tree);

/**
 * @brief Get the height of the RB tree.
 *
 * @param tree The RB tree.
 * @return The height of the tree.
 */
extern int rb_get_height(RBTree tree);

/**
 * @brief Get the height of a given RB tree node.
 *
 * @param node The RB tree node.
 * @return The height of the node.
 */
extern int rb_node_get_height(RBNode node);

/**
 * @brief Get the size (number of nodes) of the RB tree.
 *
 * @param tree The RB tree.
 * @return The size of the tree.
 */
extern int rb_get_size(RBTree tree);

/**
 * @brief Get the left child of a given RB tree node.
 *
 * @param node The RB tree node.
 * @return The left child of the node.
 */
extern RBNode rb_node_get_left(RBNode node);

/**
 * @brief Get the right child of a given RB tree node.
 *
 * @param node The RB tree node.
 * @return The right child of the node.
 */
extern RBNode rb_node_get_right(RBNode node);

/**
 * @brief Get the data stored in a given RB tree node.
 *
 * @param node The RB tree node.
 * @return Pointer to the data stored in the node.
 */
extern void* rb_node_get_data(RBNode node);

/**
 * @brief Get the balance factor of a given RB tree node.
 *
 * @param node The RB tree node.
 * @return The balance factor of the node.
 */
extern bool rb_node_is_red(RBNode node);

/**
 * @brief Check if the RB tree is valid (i.e., satisfies all RB tree properties).
 *
 * @param tree The RB tree to be checked.
 * @return true if the tree is valid, false otherwise.
 */
extern bool rb_is_valid(RBTree tree);

// --- Insertion ---

/**
 * @brief Add data to the RB tree.
 *
 * @param tree The RB tree where data will be inserted.
 * @param data Pointer to the data to be inserted.
 */
extern void rb_add(RBTree tree, const void* data);

// --- Deletion ---

/**
 * @brief Remove data from the RB tree.
 *
 * @param tree The RB tree from which data will be removed.
 * @param data Pointer to the data to be removed.
 */
extern void rb_remove(RBTree tree, const void* data);

// --- Search ---

/**
 * @brief Find a node in the RB tree containing the specified data.
 *
 * @param tree The RB tree to search.
 * @param data Pointer to the data to search for.
 * @return The node containing the data, or NULL if not found.
 */
extern RBNode rb_find_node(RBTree tree, const void* data);

/**
 * @brief Find data in the RB tree.
 *
 * @param tree The RB tree to search.
 * @param data Pointer to the data to search for.
 * @return Pointer to the found data, or NULL if not found.
 */
extern void* rb_find_data(RBTree tree, const void* data);
