/* uvzz_compress.cpp
   CSC 485B/CSC 578B/SENG 480B

   Placeholder starter code for A3

   B. Bird - 06/23/2020
*/

#include <iostream>
#include <vector>
#include "constants.hpp"
#include "move_to_front.hpp"
#include "output_file.hpp"
#include "CRC.h"

// Encodes a block of unencoded data into a block encoded with RLE.
// TODO: Improve on this.
const RLE_Data encode_into_RLE_block(const Unencoded_Block& unencoded_block, const u32& block_size) {
    RLE_Data rle_data;

    for (u32 i = 0; i < block_size; ++i) {
        rle_data.push_back((u16) unencoded_block.at(i));
    }

    return rle_data;
}

// Calculates the CRC value for a block of uncompressed data.
u32 calculate_crc(const Unencoded_Block& unencoded_block, const u32& block_size) {
    auto crc_table = CRC::CRC_32().MakeTable();
    return CRC::Calculate(unencoded_block.data(), block_size, crc_table);
}

// Creates a block of compressed RLE data.
const RLE_Block create_RLE_block(Unencoded_Block& unencoded_block, const u32& block_size) {
    u32 crc = calculate_crc(unencoded_block, block_size);
    move_to_front_encode(unencoded_block, block_size);
    auto rle_block_data = encode_into_RLE_block(unencoded_block, block_size);

    return RLE_Block{rle_block_data, crc, 123456789};
}

int main(){

    u32 block_size{};
    char next_byte{};

    std::vector <RLE_Block> all_encoded_blocks;

    Unencoded_Block unencoded_block{};

    if (std::cin.get(next_byte)) {
        while(1){

            unencoded_block.push_back(next_byte);
            ++block_size;
            if (!std::cin.get(next_byte))
                break;

            //If we get to this point, we just added a byte to the block AND there is at least one more byte in the input waiting to be written.
            if (block_size == BLOCK_MAX){
                all_encoded_blocks.push_back(create_RLE_block(unencoded_block, block_size));
                unencoded_block.clear();
                block_size = 0;
            }
        }
    }

    if (block_size) {
        all_encoded_blocks.push_back(create_RLE_block(unencoded_block, block_size));
    }

    output_to_stream(all_encoded_blocks);
}
