/**
 * @file benchmark.h
 *
 * @author agueguen-LR <adrien.gueguen@etudiant.univ-lr.fr>
 * @date 2025
 */

#pragma once

#include <stdbool.h>

/**
 * Benchmark the provided data structure operations.
 * Uses uint16_t values for testing.
 *
 * @param output_file_prefix Prefix for the output CSV files.
 * @param number_of_nodes Number of nodes to be added, searched, and removed.
 * @param add Function pointer to the add operation.
 * @param remove Function pointer to the remove operation.
 * @param search Function pointer to the search operation. Should return true if the data is found, false otherwise.
 * @param verify Function pointer to verify the integrity of the data structure after each operation. Simply return true if no verification is needed.
 * @return 0 on success, non-zero on failure.
 */
extern int benchmark(char* output_file_prefix, int number_of_nodes, void add(const void*), void remove(const void*),
              bool search(const void*), bool verify());

/**
 * Comparison function to use for data-structure being benchmarked.
 *
 * @param a Pointer to the first element.
 * @param b Pointer to the second element.
 * @return Negative value if a < b, zero if a == b, positive value if a > b.
 */
extern int benchmark_compare(const void* a, const void* b);

/**
 * Deletion function to use for data-structure being benchmarked.
 *
 * @param data Pointer to the data to be deleted.
 */
extern void benchmark_delete(void* data);
