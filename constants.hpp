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

using Block = std::array<u8, BLOCK_MAX>;
using RLE_Encoded_Block = std::vector<u16>;
using Unencoded_Block = std::array<u8, BLOCK_MAX>;

const u32 RA = 256;
const u32 RB = 257;
const u32 EOF_SYMBOL = 258;

#endif