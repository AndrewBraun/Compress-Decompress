/* decompressor.cpp

   Decompresses an inputted compressed file.
   
   Andrew Braun
   With code portions from B. Bird
*/

#include <iostream>
#include <cstdlib>
#include "BWT.hpp"
#include "constants.hpp"
#include "input_file.hpp"
#include "move_to_front.hpp"
#include "CRC.h"

// Undoes RLE for the given block.
std::vector<u16> decompress_RLE(const RLE_Data& rle_block) {
    std::vector<u16> decoded_block;

    auto iterator = rle_block.begin();
    while (iterator != rle_block.end()) {
        auto symbol = *iterator;
        decoded_block.push_back(symbol);

        if (symbol == 0) { // If a 0 is detected, a run length follows immediately after
            u8 run_length = *(++iterator);
            for (u16 i = 0; i < run_length; ++i) {
                decoded_block.push_back(0);
            }
        }

        ++iterator;
    }

    return decoded_block;
}

// Decompresses the given block of compressed data
void decompress_block(const RLE_Block& rle_block) {
    // Undoes RLE
    auto rle_decoded_block = decompress_RLE(rle_block.data);

    // Undoes the Move-to-Front transformation
    move_to_front_decode(rle_decoded_block);

    // Undoes BWT
    auto unencoded_block = inverse_bwt(rle_decoded_block, rle_block.row_index);

    // Check that the given CRC matches the calculated value.
    auto crc_table = CRC::CRC_32().MakeTable();
    u32 crc = CRC::Calculate(unencoded_block.data(), unencoded_block.size(), crc_table);

    if (crc != rle_block.crc) {
        std::cerr << "ERROR: compressed file's CRC code does not match the compressed input." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Output the block.
    for (u8 symbol : unencoded_block) {
        std::cout.put(symbol);
    }
}

int main(){

    RLE_Block compressed_block = read_block();
    while (compressed_block.data.size() != 0) {
        decompress_block(compressed_block);
        compressed_block = read_block();
    }
}
