#ifndef BWT_H
#define BWT_H

/* BWT.hpp

   Andrew Braun

   Performs the BWT and inverse BWT transformations on blocks of data.
*/

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include "constants.hpp"

// Creates a suffix array from a block.
// Each entry in the suffix array is the starting position of a suffix
// in the block, starting at 1.
// Entries in the suffix array are sorted by their suffixes lexicographically.
std::vector<u32> compute_suffix_array(const std::vector<u16> block) {

    // Create all the possible suffixes.
    std::vector<std::vector<u16>> suffix_array;
    for (auto i = 0u; i < block.size(); ++i) {
        std::vector <u16> new_suffix(block.size() - i);
        std::copy(block.begin() + i, block.end(), new_suffix.begin());

        suffix_array.push_back(new_suffix);
    }

    // Sort the suffixes
    std::sort(suffix_array.begin(), suffix_array.end());

    // Transform the vector of suffixes into an array of starting positions
    std::vector<u32> compressed_suffix_array;
    compressed_suffix_array.reserve(suffix_array.size());
    for (auto& suffix : suffix_array) {
        compressed_suffix_array.push_back( block.size() - suffix.size() + 1);
    }

    return compressed_suffix_array;
}

// Computes the BWT
// Returns a pair containing the BWT data and the row index
// Based on an algorithm from C. Kingsford, Carnegie Mellon University
// www.cs.cmu.edu/~ckingsf/bioinfo-lectures/bwt.pdf
std::pair<std::vector<u16>, u32> bwt(Unencoded_Block& unencoded_block) {
    // Transform the block of input data into a block of 16-bit symbols.
    // Increment each value by 1 and append a special BWT SYMBOL (with value 0)
    std::vector<u16> u16_block;
    for (auto& symbol : unencoded_block) {
        u16_block.push_back(symbol + 1);
    }
    u16_block.push_back(BWT_SYMBOL);

    std::vector<u32> suffix_array = compute_suffix_array(u16_block);

    std::vector<u16> bwt_block;
    bwt_block.reserve(suffix_array.size());
    u32 original_row_index{0}; // Index of the original, unshifted block.

    // Compute the BWT from the suffix array.
    for (auto iterator = suffix_array.begin(); iterator != suffix_array.end(); ++iterator) {
        auto& suffix_position = *iterator;

        if (suffix_position == 1) { // The original row
            bwt_block.push_back(BWT_SYMBOL);
            original_row_index = iterator - suffix_array.begin();
        } else {
            bwt_block.push_back(u16_block.at(suffix_position - 2));
        }
    }

    return std::make_pair(bwt_block, original_row_index);
}

// Computes the BWT using fast inversion
std::vector<u8> inverse_bwt(const std::vector<u16>& last_column, const u32& original_row_index) {
    std::vector<u16> first_column(last_column); // The first column of the BWT matrix.
    std::sort(first_column.begin(), first_column.end());

    // Create a mapping from the entries in the first column to the entries in the last column.
    // run_count counts the number of times a symbol has been seen in the first column.
    // A symbol that has appeared run_length times in the first column so far corresponds
    // to the row in the last column for the matching symbol that has appeared run_length times.
    std::vector<u32> mapping;
    mapping.reserve(first_column.size());
    u32 run_count{1};

    for (auto first_col_iter = first_column.begin(); first_col_iter != first_column.end(); ++first_col_iter) {
        // Count the number of times the given symbol has appeared in the first column so far.
        if (first_col_iter != first_column.begin() && *(first_col_iter - 1) == *first_col_iter) {
            ++run_count;
        } else {
            run_count = 1;
        }

        u32 instances_found{0}; // Number of times the same symbol has been seen in the last column
        for (auto bwt_iter = last_column.begin(); bwt_iter != last_column.end(); ++bwt_iter) {
            if (*bwt_iter == *first_col_iter) { // If an instance of the same symbol appears in the last column
                ++instances_found;

                // Create the mapping if this is the right symbol in the last column
                if (instances_found == run_count) {
                    mapping.push_back(bwt_iter - last_column.begin());
                    break;
                }
            }
        }
        
        // If the matching entry in the last column was not found
        if (instances_found != run_count) {
            std::cerr << "ERROR: compressed file's BWT block was corrupted." << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Iterate through the mapping to get the decoded data
    std::vector<u8> decoded_data;
    decoded_data.reserve(first_column.size());
    u32 mapping_ptr{original_row_index};

    for (auto counter = 0u; counter < first_column.size(); ++counter) {
        u16& symbol = first_column.at(mapping_ptr);
        decoded_data.push_back( (u8) (symbol - 1)); // Decrement the symbol to get the original value
        mapping_ptr = mapping.at(mapping_ptr);
    }

    // Remove the BWT symbol at the end.
    decoded_data.pop_back();

    return decoded_data;
}

#endif
