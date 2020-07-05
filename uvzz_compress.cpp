/* uvzz_compress.cpp
   CSC 485B/CSC 578B/SENG 480B

   Placeholder starter code for A3

   B. Bird - 06/23/2020
*/

#include <iostream>
#include "constants.hpp"
#include "output_block.hpp"
#include "CRC.h"

// Encodes a block of unencoded data into a block encoded with RLE.
// TODO: Improve on this.
const RLE_Encoded_Block encode_into_RLE_block(const Unencoded_Block& unencoded_block, const u32& block_size) {
    RLE_Encoded_Block RLE_block;

    for (u32 i = 0; i < block_size; ++i) {
        RLE_block.push_back((u16) unencoded_block.at(i));
    }

    return RLE_block;
}

// Calculates the CRC value for a block of uncompressed data.
u32 calculate_crc(const Unencoded_Block& unencoded_block, const u32& block_size) {
    auto crc_table = CRC::CRC_32().MakeTable();
    /*
    u32 crc = CRC::Calculate(unencoded_block.at(0), 1, crc_table);

    for (u32 i = 1; i < block_size; ++i) {
        crc = CRC::Calculate(unencoded_block.at(i),1, crc_table, crc);
    } */
    u32 crc = CRC::Calculate(unencoded_block.data(), block_size, crc_table);

    return crc;
}

// Compresses a block of data and writes it to standard output.
void write_block(const Unencoded_Block& unencoded_block, const u32& block_size) {

    u32 crc = calculate_crc(unencoded_block, block_size);
    auto rle_block = encode_into_RLE_block(unencoded_block, block_size);

    output_to_stream(rle_block, crc);
}

int main(){

    u32 block_size{};
    char next_byte{};
    Unencoded_Block unencoded_block{};

    if (std::cin.get(next_byte)) {
        while(1){

            unencoded_block.at(block_size++) = next_byte;
            if (!std::cin.get(next_byte))
                break;

            //If we get to this point, we just added a byte to the block AND there is at least one more byte in the input waiting to be written.
            if (block_size == unencoded_block.max_size()){
                write_block(unencoded_block, block_size);
                block_size = 0;
            }
        }
    }

    if (block_size) {
        write_block(unencoded_block, block_size);
    }
}