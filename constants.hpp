/* constants.hpp

    Contains constants for the compressor.
   
    Andrew Braun
    V00851919
    With code portions from B. Bird
*/

#ifndef UVZZ_CONSTANTS_HPP
#define UVZZ_CONSTANTS_HPP

#include <cstdint>
#include <array>
#include <vector>

/* These definitions are more reliable for fixed width types than using "int" and assuming its width */
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

const int BLOCK_MAX = 50000;

using Unencoded_Block = std::vector<u8>;
using RLE_Data = std::vector<u16>;

struct RLE_Block {
    RLE_Data data; // block of data encrypted with RLE
    u32 crc; // CRC for the block of data
    u32 row_index; // Row index for BWT
};

const u32 RA = 256; // Run-length symbol for RLE
const u32 RB = 257; // Run-length symbol for RLE
const u32 EOB_SYMBOL = 258; // Indicates the end of an RLE block
const u32 EOF_SYMBOL = 258; // Indicates the end of the data stream.

#endif