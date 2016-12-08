/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (c) 2016 Michael Catanzaro <michael.catanzaro@mst.edu>
 * Copyright (c) 2016 Jacob Fischer <jtf3m8@mst.edu>
 * Copyright (c) 2016 Christian Storer <cs9yb@mst.edu>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

//#define _GNU_SOURCE

#include "word.h"
#include <array>
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <vector>
#include <unistd.h>
#include <linux/random.h>
#include <syscall.h>


namespace dumbaes {

// 16 bytes * 8 bits/byte = 128 bits. Same for all versions of AES.
using Block = std::array<uint8_t, 16>;

// This would need to be changed to 24 or 32 for AES-192 or AES-256.
using Key = std::array<uint8_t, 16>;


/**
 * It encrypts one block of data with the specified key. This probably shouldn't
 * ever be used directly by applications; it's intended for use by block cipher
 * mode implementations.
 */
Block encrypt_block(const Block& block, const Key& key);

/**
 * It decrypts one block of data with the specified key. This probably shouldn't
 * ever be used directly by applications; it's intended for use by block cipher
 * mode implementations.
 */
Block decrypt_block(const Block& block, const Key& key);

/**
 *  Takes a vector of Blocks containing the plaintext
 *    along with a key and length (in bytes) of the data being encrypted.
 *  All Blocks in 'plaintext' must be filled completely with data except for
 *    the last block which can contain NULLs only after the data 
 *  Will encrypt each plaintext block starting with the first block in 'plaintext'
 *    using cipher block chaining. 
 *  The IV must be in plaintext form.
 *  The length will be changed to reflect the new size (in bytes) of the ciphertext.
 */ 
std::vector<Block> encrypt_cbc(const std::vector<Block>& plaintext, size_t& length,
                        const Key& key, const Block& new_iv);

/**
 *  Takes a vector of Blocks pcontaining the ciphertext 
 *    along with a key and length (in bytes) of the data to be decrypoted and returns
 *    the plaintext in a vector.
 *  All Blocks in 'ciphertext' must be completely filled with data.  
 *  Will decrypt each ciphertext block individually as per cbc mode operations
 *    and store it in palintext blocks.
 *  The length will be modified to account for padding removal.
 *  The IV must be in plaintext form.
 *  All blocks in the plaintext vector will be completely filled with data, but only *    the first contigous updated 'length' bytes will contain meaningful data. The  
 *    extra data is the leftover padding but can be ignored. 
 */                         
std::vector<Block> decrypt_cbc(const std::vector<Block>& ciphertext, size_t& length,
                        const Key& key, const Block& new_iv);

/**
 *  Takes a vector of Blocks containing the plaintext
 *    along with a key and length (in bytes) of the data being encrypted.
 *  All Blocks in 'plaintext' must be filled completely with data except for
 *    the last block which can contain NULLs only after the data 
 *  Will encrypt each plaintext block individually as per ecb mode operations
 *    and store it in ciphertext blocks
 *  The length will be changed to reflect the size of of new ciphertext.
 *  Will return a ciphertext vector of blocks containing the ciphertext
 *    blocks.
 *  The return ciphertext vector will contain 'length' bytes of encrypted data with *    all Blocks completely filled with data.
 */
std::vector<Block> encrypt_ecb(const std::vector<Block>& plaintext, size_t& length,
                        const Key& key); 
                        
/**
 *  Takes a vector of Blocks containing the ciphertext 
 *    along with a key and length (in bytes) of the data to be decrypoted and returns
 *    the plaintext in a vector.
 *  All Blocks in 'ciphertext' must be completely filled with data.  
 *  Will decrypt each ciphertext block individually as per ecb mode operations
 *    and store it in palintext blocks.
 *  The length will be modified to account for padding removal;
 *  All blocks in the plaintext vector will be completely filled with data, but only *    the first contigous updated 'length' bytes will contain meaningful data. The  
 *    extra data is the leftover padding but can be ignored. 
 */                
std::vector<Block> decrypt_ecb(const std::vector<Block>& ciphertext, size_t& length,
                        const Key& key);

/**
 *  Function returns a BLock containing a randomly generated 
 *    IV using the Linux getrandom system call.
 *  As such a Linux system is required for this function to work
 */
Block generate_iv(); 
                
// Exposed only for unit tests.
namespace internal {

// Nk. This would need to be changed to 6 for AES-192 or 8 for AES-256.
const int key_size = 4;
// Nb. This is always 4 for AES.
const int block_size = 4;
// Nr. This would need to be changed to 12 for AES-192 or 14 for AES-256.
const int num_rounds = 10;

using KeySchedule = std::array<Word, block_size*(num_rounds+1)>;

KeySchedule compute_key_schedule(const Key&);

// The Rindjael algorithm state is a 4xNb table.
using State = std::array<std::array<uint8_t, block_size>, 4>;

Block encrypt_block(
    const Block&,
    const Key&,
    std::function<void (const State&, int)>&& test_hook);

} // namespace internal

} // namespace dumbaes
