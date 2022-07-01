/*****************************************************************//**
 * \file   memory_swap.h
 * \brief  Memory swap function implementation module.
 * 
 * \author Sabitov Kirill
 * \date   28 June 2022
 *********************************************************************/

#pragma once

#include "base.h"

namespace scl
{
    /**
     * Swap momory blocks function.
     * 
     * \param BlockA - first memory block.
     * \param BlockB - second memory block.
     * \param Size - blocks size in bytes.
     * \return None.
     */
    inline void MemorySwap(void *BlockA, void *BlockB, size_t Size)
    {
        u8 *a_swap = (u8 *)BlockA, *b_swap = (u8 *)BlockB;
        u8 *a_end = a_swap + Size;

        while (a_swap < a_end)
        {
            char temp = *a_swap;
            *a_swap = *b_swap;
            *b_swap = temp;

            a_swap++, b_swap++;
        }
    }

#define SCL_MEMORY_SWAP(BlockA, BlockB)                             \
    SCL_CORE_ASSERT(sizeof(*BlockA) == sizeof(*BlockB),             \
                    "Swaping memory blocks are of diffrent size!"); \
    MemorySwap((void*)BlockA,(void*)BlockB, sizeof(*BlockA))
    
}
