#ifndef BWT_H
#define BWT_H

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include "constants.hpp"

std::vector<u32> compute_suffix_array(const std::vector<u16> block) {

    std::vector<std::vector<u16>> suffix_array;

    for (auto i = 0u; i < block.size(); ++i) {
        std::vector <u16> new_suffix(block.size() - i);
        std::copy(block.begin() + i, block.end(), new_suffix.begin());

        suffix_array.push_back(new_suffix);
    }

    std::sort(suffix_array.begin(), suffix_array.end());

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
    std::vector<u16> u16_block;
    for (auto& symbol : unencoded_block) {
        u16_block.push_back(symbol + 1);
    }
    u16_block.push_back(BWT_SYMBOL);

    std::vector<u32> suffix_array = compute_suffix_array(u16_block);

    std::vector<u16> bwt_block;
    bwt_block.reserve(suffix_array.size());
    u32 original_row_index{0};

    for (auto iterator = suffix_array.begin(); iterator != suffix_array.end(); ++iterator) {
        auto& suffix_position = *iterator;

        if (suffix_position == 1) {
            bwt_block.push_back(BWT_SYMBOL);
            original_row_index = iterator - suffix_array.begin();
        } else {
            bwt_block.push_back(u16_block.at(suffix_position - 2));
        }
    }

    return std::make_pair(bwt_block, original_row_index);
}

std::vector<u8> inverse_bwt(const std::vector<u16>& last_column, const u32& original_row_index) {
    std::vector<u16> first_column(last_column);
    std::sort(first_column.begin(), first_column.end());

    // Create mapping
    std::vector<u32> mapping;
    mapping.reserve(first_column.size());
    u32 run_count{1};

    for (auto first_col_iter = first_column.begin(); first_col_iter != first_column.end(); ++first_col_iter) {
        if (first_col_iter != first_column.begin() && *(first_col_iter - 1) == *first_col_iter) {
            ++run_count;
        } else {
            run_count = 1;
        }

        u32 instances_found{0};
        for (auto bwt_iter = last_column.begin(); bwt_iter != last_column.end(); ++bwt_iter) {
            if (*bwt_iter == *first_col_iter) {
                ++instances_found;

                if (instances_found == run_count) {
                    mapping.push_back(bwt_iter - last_column.begin());
                    break;
                }
            }
        }
        if (instances_found != run_count) {
            std::cerr << "ERROR: compressed file's BWT block was corrupted." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
/*
    std::cerr << "First column" << std::endl;
    for (auto x : first_column) {
        std::cerr << '"' << (u8) x << '"' << std::endl;
    }

    std::cerr << "Mapping" << std::endl;
    for (auto x : mapping) {
        std::cerr << '"' << x << '"' << std::endl;
    } */

    // Iterate through the mapping to get the decoded data
    std::vector<u8> decoded_data;
    decoded_data.reserve(first_column.size());
    u32 mapping_ptr{original_row_index};

    for (auto counter = 0u; counter < first_column.size(); ++counter) {
        u16& symbol = first_column.at(mapping_ptr);
        decoded_data.push_back( (u8) (symbol - 1));
        mapping_ptr = mapping.at(mapping_ptr);
    }

    decoded_data.pop_back();
    /*
    std::cerr << "BWT Decoded! Decoded vector size: " << decoded_data.size() << std::endl;
    for (auto symbol : decoded_data) {
        std::cerr << '"' << symbol << '"' << std::endl;
    } */

    return decoded_data;
}

#endif