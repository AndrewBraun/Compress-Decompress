/* uvzz_decompress.cpp
   CSC 485B/CSC 578B/SENG 480B

   Placeholder starter code for A3

   B. Bird - 06/23/2020
*/

#include <iostream>
#include <cstdlib>
#include "constants.hpp"
#include "input_block.hpp"
#include "CRC.h"

Unencoded_Block decompress_RLE(const RLE_Encoded_Block& rle_block) {
    Unencoded_Block unencoded_block;

    for (auto i = 0u; i < rle_block.size(); ++i) {
        unencoded_block.at(i) = (u8) rle_block.at(i);
    }

    return unencoded_block;
}

void decompress_block(const input_result& input) {
    for (auto symbol : input.rle_block) {
        std::cout.put((u8) symbol);
    }
    /*
    auto unencoded_block = decompress_RLE(input.rle_block);

    auto crc_table = CRC::CRC_32().MakeTable();
    u32 crc = CRC::Calculate(unencoded_block.data(), input.rle_block.size(), crc_table);
    //if (crc != input.crc) {
    //    std::cerr << "ERROR: compressed file's CRC code does not match the compressed input.";
    //    exit(EXIT_FAILURE);
    //}
    for (auto i = 0u; i < input.rle_block.size(); ++i) {
        std::cout.put(unencoded_block.at(i));
    } */
}

int main(){

    input_result compressed_block_input = read_block_from_input();

    // Keep reading blocks until the input is done.
    while(!compressed_block_input.done) {
        // Process the compressed block
        decompress_block(compressed_block_input);

        compressed_block_input = read_block_from_input();
    }

    decompress_block(compressed_block_input);
}