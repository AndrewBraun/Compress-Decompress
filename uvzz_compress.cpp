/* uvzz_compress.cpp
   CSC 485B/CSC 578B/SENG 480B

   Placeholder starter code for A3

   B. Bird - 06/23/2020
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

    for (u32 i = 0; i < block.size(); ++i) {
        u16 symbol = block.at(i);
        rle_data.push_back(symbol);

        // Encode run length of zeroes
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
u32 calculate_crc(const Unencoded_Block& unencoded_block, const u32& block_size) {
    auto crc_table = CRC::CRC_32().MakeTable();
    return CRC::Calculate(unencoded_block.data(), block_size, crc_table);
}

// Creates a block of compressed RLE data.
const RLE_Block create_RLE_block(Unencoded_Block& unencoded_block, const u32& block_size) {
    u32 crc = calculate_crc(unencoded_block, block_size);

    auto bwt_pair = bwt(unencoded_block);
    std::vector<u16>& bwt_data = bwt_pair.first;

    std::cerr << "BWT Block" << std::endl;
    for (auto i = 0u; i < bwt_data.size(); ++i) {
        std::cerr << ' ' << bwt_data.at(i) << ' ';
        if (i % 15 == 15) {
            std::cerr << std::endl;
        }
    }

    move_to_front_encode(bwt_data);

    std::cerr << "Move to front Block" << std::endl;
    for (auto i = 0u; i < bwt_data.size(); ++i) {
        std::cerr << ' ' << bwt_data.at(i) << ' ';
        if (i % 15 == 15) {
            std::cerr << std::endl;
        }
    }

    auto rle_block_data = encode_into_RLE_block(bwt_data);

    return RLE_Block{rle_block_data, crc, bwt_pair.second};
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
