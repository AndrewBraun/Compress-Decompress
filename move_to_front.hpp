#ifndef MOVE_TO_FRONT_H
#define MOVE_TO_FRONT_H

/* move_to_front.hpp

   Performs and undoes Move-to-Front transformations on blocks.
   
   Andrew Braun
   V00851919
*/

#include <algorithm>
#include <cassert>
#include "constants.hpp"

// Initialize the stack
// Each entry is indexed by the symbol and contains its position in the stack.
// 257 entries are used to account for the BWT symbol.
std::array<u16, 257> get_stack() {
    std::array<u16, 257> stack;
    for (u16 i = 0u; i < stack.size(); ++i) {
        stack.at(i) = i;
    }
    return stack;
}

// Transforms a block of 8-bit symbols into a move-to-front encoded version.
void move_to_front_encode(RLE_Data& block) {
    auto stack = get_stack();

    for (auto iterator = block.begin(); iterator != block.end(); ++iterator) {
        auto input_symbol = *iterator;
        auto stack_value = stack.at(input_symbol); // Position of the input symbol in the stack

        *iterator = stack_value;

        // Adjust the stack
        if (stack_value != 0) {
            // Adjust other stack values
            for (auto& other_stack_value : stack) {
                if (other_stack_value < stack_value)
                    ++other_stack_value;
            }

            stack.at(input_symbol) = 0; // Move the symbol to the top of the stack
        }
    }
}

// Decodes a Move-To-Front encoded block
void move_to_front_decode(std::vector<u16>& block) {
    auto stack = get_stack();

    for (auto iterator = block.begin(); iterator != block.end(); ++iterator) {
        auto input_index = *iterator;
        auto stack_value_ptr = std::find(stack.begin(), stack.end(), input_index);

        if (stack_value_ptr == stack.end()) {
            std::cerr << "ERROR: index not found in the Move-to-Front stack." << std::endl;
            exit(EXIT_FAILURE);
        }
        auto output_symbol = stack_value_ptr - stack.begin();

        *iterator = output_symbol;

        // Adjust the stack
        if (input_index != 0) {
            // Adjust other stack values
            for (auto& other_stack_value : stack) {
                if (other_stack_value < input_index)
                    ++other_stack_value;
            }

            stack.at(output_symbol) = 0; // Move the symbol to the top of the stack
        }
    }
}

#endif
