/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosiolib/memory.h>
#include <eosiolib/print.hpp>

void* sbrk(size_t num_bytes);

  /**
   *  @defgroup memorycppapi Memory C++ API
   *  @brief Defines common memory functions
   *
   *  @{
   */

extern "C" {

/**
 * Allocate additional memory
 *
 * @param size - Number of additional bytes to be allocated
 * @return void* - Pointer to start of the new allocated memory
 */
void* malloc(size_t size);

/**
 * Allocate a block of memory for an array of **count** elements, each of them **size** bytes long, and initializes all bits with 0
 *
 * @param count - Number of elements to allocate
 * @param size - Size of each element
 * @return void* - Pointer to start of the new allocated memory
 */
void* calloc(size_t count, size_t size);

/**
 * Reallocate the given area of memory, which is allocated by malloc(), calloc(), or realloc() previously
 *
 * @param ptr - Pointer to the memory area to be reallocated
 * @param size - New size of the memory
 * @return void* - Pointer to the new reallocated memory
 */
void* realloc(void* ptr, size_t size);

/**
 *
 * Deallocates the given area of memory which is previously allocated by malloc(), calloc(), or realloc()
 *
 * @param ptr - Pointer to the memory to be deallocated
 */
void free(void* ptr);

}

/// @}memorycppapi
