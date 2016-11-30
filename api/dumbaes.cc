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

using namespace dumbaes;

unsigned char* dumbaes_128_encrypt_block(const unsigned char* plaintext,
                                         const unsigned char* key)
{
    Block datablock;
    Key keyblock;

    std::memcpy(datablock.data(), plaintext, 16);
    std::memcpy(keyblock.data(), key, 16);

    Block ciphertext = encrypt_block(datablock, keyblock);

    unsigned char* result = static_cast<unsigned char*>(std::malloc(16));
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

    unsigned char* result = static_cast<unsigned char*>(std::malloc(16));
    std::memcpy(result, plaintext.data(), 16);
    return result;
}

#if 0
unsigned char* dumbaes_128_encrypt_cbc(const unsigned char* plaintext,
                                       size_t               length,
                                       const unsigned char* key)
{
    // TODO
}

unsigned char* dumbaes_128_decrypt_cbc(const unsigned char* ciphertext,
                                       size_t               length,
                                       const unsigned char* key)
{
    // TODO
}
#endif
