/* uvzz_compress.cpp

   Compresses an inputted file.
   
   Andrew Braun
   V00851919
   With code portions from B. Bird
*/

#include <iostream>
#include <vector>
#include "BWT.hpp"
#include "constants.hpp"
#include "move_to_front.hpp"
#include "output_file.hpp"
#include "CRC.h"

// Encodes a block of unencoded data into a block encoded with RLE.
const RLE_Data encode_into_RLE_block(const RLE_Data& block) {
    RLE_Data rle_data;

    for (auto i = 0u; i < block.size(); ++i) {
        auto symbol = block.at(i);
        rle_data.push_back(symbol);

        // Encode a run length of the current symbol is 0
        if (symbol == 0) {
            u16 run_length = 0;
            while (i + 1 < block.size() && block.at(i + 1) == 0 && run_length < 255) {
                ++i;
                ++run_length;
            }

            rle_data.push_back(run_length);
        }
    }

    return rle_data;
}

// Calculates the CRC value for a block of uncompressed data.
u32 calculate_crc(const Unencoded_Block& unencoded_block) {
    auto crc_table = CRC::CRC_32().MakeTable();
    return CRC::Calculate(unencoded_block.data(), unencoded_block.size(), crc_table);
}

// Creates a block of compressed RLE data.
// is_last indicates if this is the last block
const void output_RLE_block(Unencoded_Block& unencoded_block, const bool is_last) {
    u32 crc = calculate_crc(unencoded_block);

    // Perform BWT on the unecoded block
    auto bwt_pair = bwt(unencoded_block);
    std::vector<u16>& bwt_data = bwt_pair.first;

    // Perform Move-to-Front encoding on the block
    move_to_front_encode(bwt_data);

    // Perform RLE on the block
    auto rle_block_data = encode_into_RLE_block(bwt_data);

    output_to_stream(RLE_Block{rle_block_data, crc, bwt_pair.second}, is_last);
}

int main(){

    char next_byte{};

    Unencoded_Block unencoded_block{};

    if (std::cin.get(next_byte)) {
        while(1){

            unencoded_block.push_back(next_byte);
            if (!std::cin.get(next_byte))
                break;

            // Finish a block and start a new one
            if (unencoded_block.size() == BLOCK_MAX){
                output_RLE_block(unencoded_block, false);
                unencoded_block.clear();
            }
        }
    }

    // Create a new block if there still is data.
    if (unencoded_block.size()) {
        output_RLE_block(unencoded_block, false);
    }

    output_RLE_block(unencoded_block, true);
}