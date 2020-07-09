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

    for (u16 rle_symbol : rle_block) {
        unencoded_block.push_back( (u8) rle_symbol);
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
