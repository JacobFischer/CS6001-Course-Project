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

#include "dumbaes.h"

#include "src/dumbaes.h"
#include <cstdlib>
#include <cstring>
#include <memory>

using namespace dumbaes;

unsigned char* dumbaes_128_encrypt_block(const unsigned char* plaintext,
                                         const unsigned char* key)
{
    Block datablock;
    Key keyblock;

    std::memcpy(datablock.data(), plaintext, 16);
    std::memcpy(keyblock.data(), key, 16);

    Block ciphertext = encrypt_block(datablock, keyblock);

    auto result = static_cast<unsigned char*>(std::malloc(16));
    std::memcpy(result, ciphertext.data(), 16);
    return result;
}

unsigned char* dumbaes_128_decrypt_block(const unsigned char* ciphertext,
                                         const unsigned char* key)
{
    Block datablock;
    Key keyblock;

    std::memcpy(datablock.data(), ciphertext, 16);
    std::memcpy(keyblock.data(), key, 16);

    Block plaintext = decrypt_block(datablock, keyblock);

    auto result = static_cast<unsigned char*>(std::malloc(16));
    std::memcpy(result, plaintext.data(), 16);
    return result;
}

unsigned char* dumbaes_128_encrypt_cbc(const unsigned char* plaintext,
                                       size_t* length,
                                       const unsigned char* key,
                                       const unsigned char* iv)
{
    Key keyblock;
    std::memcpy(keyblock.data(), key, 16);
    
    Block iv_block;
    std::memcpy(iv_block.data(), iv, 16);
    
    size_t num_blocks = (*length / 16) + 1;
    std::vector<Block> plaintext_vector(num_blocks);
    
    std::memcpy(plaintext_vector[0].data(), plaintext, *length);
    
    std::vector<Block> ciphertext_vector = encrypt_cbc(plaintext_vector, *length,
                                                       keyblock, iv_block);
    auto result = static_cast<unsigned char*>(std::malloc(*length));
    std::memcpy(result, ciphertext_vector.data(), *length);
    
    return result;
}

unsigned char* dumbaes_128_decrypt_cbc(const unsigned char* ciphertext,
                                       size_t* length,
                                       const unsigned char* key,
                                       const unsigned char* iv)
{
    Key keyblock;
    std::memcpy(keyblock.data(), key, 16);
    
    Block iv_block;
    std::memcpy(iv_block.data(), iv, 16);
    
    size_t num_blocks = *length / 16;
    std::vector<Block> ciphertext_vector(num_blocks);
    
    std::memcpy(ciphertext_vector[0].data(), ciphertext, *length);
    std::vector<Block> plaintext_vector = decrypt_cbc(ciphertext_vector, *length,
                                                      keyblock, iv_block);
    auto result = static_cast<unsigned char*>(std::malloc(*length));
    std::memcpy(result, plaintext_vector.data(), *length);

    return result;
}

unsigned char* dumbaes_128_encrypt_ecb(const unsigned char* plaintext,
                                       size_t* length,
                                       const unsigned char* key)
{
    Key keyblock;
    std::memcpy(keyblock.data(), key, 16);

    size_t num_blocks = *length / 16 + 1;
    
    std::vector<Block> plaintext_vector(num_blocks);
    std::memcpy(plaintext_vector[0].data(), plaintext, *length);
    
    std::vector<Block> ciphertext_vector = encrypt_ecb(plaintext_vector, *length,
                                                       keyblock);
                                                       
    auto result = static_cast<unsigned char*>(std::malloc(*length));
    std::memcpy(result, ciphertext_vector[0].data(), *length);
    
    return result;
}

unsigned char* dumbaes_128_decrypt_ecb(const unsigned char* ciphertext,
                                       size_t* length,
                                       const unsigned char* key)
{
    Key keyblock;
    std::memcpy(keyblock.data(), key, 16);

    size_t num_blocks = *length / 16;
    
    std::vector<Block> ciphertext_vector(num_blocks);
    std::memcpy(&ciphertext_vector[0], ciphertext, *length);
    
    std::vector<Block> plaintext_vector = decrypt_ecb(ciphertext_vector, *length,
                                                      keyblock);
    auto result = static_cast<unsigned char*>(std::malloc(*length));
    std::memcpy(result, &plaintext_vector[0], *length);
    
    return result;
}

unsigned char* dumbaes_128_generate_iv()
{
    Block iv = generate_iv();
    auto result = static_cast<unsigned char*>(std::malloc(16));
    std::memcpy(result, iv.data(), 16);

    return result;
}
