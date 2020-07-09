#ifndef MOVE_TO_FRONT_H
#define MOVE_TO_FRONT_H

#include <algorithm>
#include "constants.hpp"

// Transforms a block of 8-bit symbols into a move-to-front encoded version.
void move_to_front_encode(Unencoded_Block& block, const u32& block_size) {
    // Initialize the stack
    // Each entry is indexed by the symbol and contains its position in the stack.
    std::array<u8, 256> stack;
    for (u16 i = 0; i <= 255; ++i) {
        stack.at(i) = (u8) i;
    }

    for (u32 i = 0; i < block_size; ++i) {
        u8 input_symbol = block.at(i);
        u8 stack_value = stack.at(input_symbol); // Position of the input symbol in the stack
        block.at(i) = stack_value;

        // Adjust the stack
        if (stack_value != 0) {
            // Adjust other stack values
            for (u8& other_stack_value : stack) {
                if (other_stack_value < stack_value)
                    ++other_stack_value;
            }

            stack.at(input_symbol) = 0; // Move the symbol to the top of the stack
        }
    }
}

// Decodes a Move-To-Front encoded block
void move_to_front_decode(Unencoded_Block& block) {
    // Initialize the stack
    // Each entry is indexed by the symbol and contains its position in the stack.
    std::array<u8, 256> stack;
    for (u16 i = 0; i <= 255; ++i) {
        stack.at(i) = (u8) i;
    }

    for (u32 i = 0; i < block.size(); ++i) {
        u8 input_index = block.at(i);
        auto stack_value_ptr = std::find(stack.begin(), stack.end(), input_index);
        u8 output_symbol = stack_value_ptr - stack.begin();
        block.at(i) = output_symbol;

        // Adjust the stack
        if (input_index != 0) {
            // Adjust other stack values
            for (u8& other_stack_value : stack) {
                if (other_stack_value < input_index)
                    ++other_stack_value;
            }

            stack.at(output_symbol) = 0; // Move the symbol to the top of the stack
        }
    }
}

#endif