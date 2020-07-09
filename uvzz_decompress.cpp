/* uvzz_decompress.cpp
   CSC 485B/CSC 578B/SENG 480B

   Placeholder starter code for A3

   B. Bird - 06/23/2020
*/

#include <iostream>
#include <cstdlib>
#include "constants.hpp"
#include "input_file.hpp"
#include "move_to_front.hpp"
#include "CRC.h"

Unencoded_Block decompress_RLE(const RLE_Data& rle_block) {
    Unencoded_Block unencoded_block;

    auto iterator = rle_block.begin();
    while (iterator != rle_block.end()) {
        u8 symbol = (u8) *iterator;
        unencoded_block.push_back(symbol);

        if (symbol == 0) {
            u8 run_length = *(++iterator);
            for (u16 i = 0; i < run_length; ++i) {
                unencoded_block.push_back(0);
            }
        }

        ++iterator;
    }

    return unencoded_block;
}

void decompress_block(const RLE_Block& rle_block) {
    
    auto unencoded_block = decompress_RLE(rle_block.data);
    move_to_front_decode(unencoded_block);

    auto crc_table = CRC::CRC_32().MakeTable();
    u32 crc = CRC::Calculate(unencoded_block.data(), unencoded_block.size(), crc_table);

    if (crc != rle_block.crc) {
        std::cerr << "ERROR: compressed file's CRC code does not match the compressed input.";
        exit(EXIT_FAILURE);
    }
    for (u8 symbol : unencoded_block) {
        std::cout.put(symbol);
    }
}

int main(){

    std::vector<RLE_Block> all_blocks = read_input();

    // Uncompress all the blocks
    for(RLE_Block& block : all_blocks) {
        // Process the compressed block
        decompress_block(block);
    }
}
