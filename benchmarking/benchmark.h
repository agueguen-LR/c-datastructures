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
 * @param search Function pointer to the search operation.
 * @return 0 on success, non-zero on failure.
 */
extern int benchmark(char* output_file_prefix, int number_of_nodes, bool add(const void*), bool remove(const void*), bool search(const void*));
