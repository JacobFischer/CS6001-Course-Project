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

#include "polynomial.h"
#include "polynomial-polynomial.h"
#include <cstdint>
#include <cstring>
#include <utility>

namespace dumbaes {

using namespace internal;

static uint8_t sbox[16][16] = {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16},
};

static uint8_t inverse_sbox[16][16] = {
    {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
    {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
    {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
    {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
    {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
    {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
    {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
    {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
    {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
    {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
    {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
    {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
    {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
    {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
    {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
    {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d},
};

static State block_to_state(const Block& block)
{
    State state;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < block_size; j++)
            state[i][j] = block[i + 4*j];
    return state;
}

static Block state_to_block(State&& state)
{
    Block block;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < block_size; j++)
            block[i + 4*j] = state[i][j];
    return block;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
// For debugging only... obviously.
static void print_state(const State& state)
{
    std::fprintf(stderr,
                 "\n%02x%02x%02x%02x\n%02x%02x%02x%02x\n%02x%02x%02x%02x\n%02x%02x%02x%02x\n\n",
                 state[0][0], state[0][1], state[0][2], state[0][3],
                 state[1][0], state[1][1], state[1][2], state[1][3],
                 state[2][0], state[2][1], state[2][2], state[2][3],
                 state[3][0], state[3][1], state[3][2], state[3][3]);
}
#pragma GCC diagnostic pop

static uint8_t substitute_byte(uint8_t byte)
{
   return sbox[(byte >> 4) & 0x0f][byte & 0x0f];
}

static void substitute_bytes(State& state)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < block_size; j++)
            state[i][j] = substitute_byte(state[i][j]);
}

static uint8_t inverse_substitute_byte(uint8_t byte)
{
   return inverse_sbox[(byte >> 4) & 0x0f][byte & 0x0f];
}

static void inverse_substitute_bytes(State& state)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < block_size; j++)
            state[i][j] = inverse_substitute_byte(state[i][j]);
}

static void shift_rows(State& state)
{
    State temp = state;

    state[1][0] = temp[1][1];
    state[1][1] = temp[1][2];
    state[1][2] = temp[1][3];
    state[1][3] = temp[1][0];

    state[2][0] = temp[2][2];
    state[2][1] = temp[2][3];
    state[2][2] = temp[2][0];
    state[2][3] = temp[2][1];

    state[3][0] = temp[3][3];
    state[3][1] = temp[3][0];
    state[3][2] = temp[3][1];
    state[3][3] = temp[3][2];
}

static void inverse_shift_rows(State& state)
{
    State temp = state;

    state[1][0] = temp[1][3];
    state[1][1] = temp[1][0];
    state[1][2] = temp[1][1];
    state[1][3] = temp[1][2];

    state[2][0] = temp[2][2];
    state[2][1] = temp[2][3];
    state[2][2] = temp[2][0];
    state[2][3] = temp[2][1];

    state[3][0] = temp[3][1];
    state[3][1] = temp[3][2];
    state[3][2] = temp[3][3];
    state[3][3] = temp[3][0];
}

static void do_mix_columns(State& state, PolynomialPolynomial&& a)
{
    for (int c = 0; c < 4; c++) {
        PolynomialPolynomial b{Polynomial{state[3][c]},
                               Polynomial{state[2][c]},
                               Polynomial{state[1][c]},
                               Polynomial{state[0][c]}};
        PolynomialPolynomial result = a*b;

        state[0][c] = result.value()[0];
        state[1][c] = result.value()[1];
        state[2][c] = result.value()[2];
        state[3][c] = result.value()[3];
    }
}

static void mix_columns(State& state)
{
    do_mix_columns(state, PolynomialPolynomial{Polynomial{0x03},
                                               Polynomial{0x01},
                                               Polynomial{0x01},
                                               Polynomial{0x02}});
}

static void inverse_mix_columns(State& state)
{
    do_mix_columns(state, PolynomialPolynomial{Polynomial{0x0b},
                                               Polynomial{0x0d},
                                               Polynomial{0x09},
                                               Polynomial{0x0e}});
}

static void add_round_key(State& state, Word w0, Word w1, Word w2, Word w3)
{
   state[0][0] ^= w0[0];
   state[1][0] ^= w0[1];
   state[2][0] ^= w0[2];
   state[3][0] ^= w0[3];

   state[0][1] ^= w1[0];
   state[1][1] ^= w1[1];
   state[2][1] ^= w1[2];
   state[3][1] ^= w1[3];

   state[0][2] ^= w2[0];
   state[1][2] ^= w2[1];
   state[2][2] ^= w2[2];
   state[3][2] ^= w2[3];

   state[0][3] ^= w3[0];
   state[1][3] ^= w3[1];
   state[2][3] ^= w3[2];
   state[3][3] ^= w3[3];
}

Block encrypt_block(const Block& block, const Key& key)
{
    return internal::encrypt_block(block, key, [](const State&, int){});
}

// See FIPS 197, Fig. 12
Block decrypt_block(const Block& block, const Key& key)
{
    State state = block_to_state(block);
    KeySchedule w = compute_key_schedule(key);
    add_round_key(state,
                  w[num_rounds*block_size],
                  w[num_rounds*block_size + 1],
                  w[num_rounds*block_size + 2],
                  w[num_rounds*block_size + 3]);

    for (int i = num_rounds - 1; i > 0; i--) {
        inverse_shift_rows(state);
        inverse_substitute_bytes(state);
        add_round_key(state,
                      w[i*block_size],
                      w[i*block_size + 1],
                      w[i*block_size + 2],
                      w[i*block_size + 3]);
        inverse_mix_columns(state);
    }

    inverse_shift_rows(state);
    inverse_substitute_bytes(state);
    add_round_key(state, w[0], w[1], w[2], w[3]);

    return state_to_block(std::move(state));
}

std::vector<Block> encrypt_cbc(const std::vector<Block>& plaintext, size_t& length,
                               const Key& key, const Block& iv)
{
    Block pad_block, xor_result;
    std::vector<Block> ciphertext;
    
    size_t num_blocks = length /16 +1;
    size_t pad = length % 16;
    char pad_char = '0' + pad;
    
    for (int i=0; i<16; i++)
    {
        xor_result[i] = static_cast<char>(plaintext[0][i]) 
                      ^ static_cast<char>(iv[i]);
    }
    ciphertext.push_back(encrypt_block(xor_result, key));
    for (size_t block=1; block<num_blocks-1; block++)
    {
        for (int i=0; i<16; i++)
        {
            xor_result[i] = static_cast<char>(plaintext[block][i]) 
                          ^ static_cast<char>(ciphertext[block-1][i]);
        }
        ciphertext.push_back(encrypt_block(xor_result, key));
    }
    
    pad_block.fill(pad_char);
    if (pad != 0)
    {
        std::memcpy(pad_block.data(), plaintext[num_blocks-1].data(), pad);
    }
    for (int i=0; i<16; i++)
    {
        xor_result[i] = static_cast<char>(pad_block[i])  
                      ^ static_cast<char>(ciphertext[num_blocks-2][i]);
    }
    ciphertext.push_back(encrypt_block(xor_result, key));
    
    length = (num_blocks) * 16;
    return ciphertext;
}             

std::vector<Block> decrypt_cbc(const std::vector<Block>& ciphertext, size_t& length,
                               const Key& key, const Block& iv)
{
    Block pad_block, xor_input;
    size_t num_blocks = length /16;
    std::vector<Block> plaintext(num_blocks);
    
    std::memcpy(pad_block.data(), ciphertext[num_blocks].data(), 16);
    
    xor_input = decrypt_block(ciphertext[0],key);
    for (int i=0; i<16; i++)
    {
        plaintext[0][i] = static_cast<char>(xor_input[i]) 
                        ^ static_cast<char>(iv[i]);
    }
    
    for (size_t block=1; block<num_blocks; block++)
    {
        xor_input = decrypt_block(ciphertext[block],key);
        for (int i=0; i<16; i++)
        {
            plaintext[block][i] = static_cast<char>(xor_input[i]) 
                                ^ static_cast<char>(ciphertext[block-1][i]);
        }           
    }
    
    char pad_char = plaintext[num_blocks-1][15];
    if (pad_char == '0')
    {
        length -= 16;
    }
    else
    {
        size_t pad = pad_char-'0';
        length -= (16-pad);
    }
    
    return plaintext;
}  

std::vector<Block> encrypt_ecb(const std::vector<Block>& plaintext, size_t& length,
                               const Key& key)
{
    Block pad_block;
    
    size_t num_blocks = (length / 16) + 1;
    size_t pad = length % 16;
    char pad_char = '0' + pad;
    std::vector<Block> ciphertext;
    for (size_t block=0; block<num_blocks-1; block++)
    {
        ciphertext.push_back(encrypt_block(plaintext[block], key));
    }
    
    pad_block.fill(pad_char);
    if (pad != 0)
    {
        std::memcpy(pad_block.data(), plaintext[num_blocks-1].data(), pad);
    }
    
    ciphertext.push_back(encrypt_block(pad_block, key));
    
    length = (num_blocks) * 16;
    
    return ciphertext;
} 

std::vector<Block> decrypt_ecb(const std::vector<Block>& ciphertext, size_t& length,
                               const Key& key)
{
    size_t num_blocks = length / 16;
    size_t pad = 0;
    std::vector<Block> plaintext;

    for (size_t block = 0; block < num_blocks; block++)
    {
        plaintext.push_back(decrypt_block(ciphertext[block], key));
    }
    
    char pad_char = plaintext[num_blocks-1][15];
    if (pad_char == '0')
    {
        length -= 16;
    }
    else
    {
        pad = pad_char-'0';
        length -= (16-pad);
    }
    
    return plaintext;
}
           
Block generate_iv()
{
    Block iv;
    int readbytes = 0;
    while (readbytes != 16)
    {
        readbytes = 0;
        readbytes = syscall(SYS_getrandom, iv.data(), 16, 0);
    }
    
    return iv;
}

static Word substitute_word(Word word)
{
    return Word{substitute_byte(word[0]),
                substitute_byte(word[1]),
                substitute_byte(word[2]),
                substitute_byte(word[3])};
}

static Word rotate_word(Word word)
{
    return Word{word[1], word[2], word[3], word[0]};
}

namespace internal {

// FIPS 197 Fig. 11
KeySchedule compute_key_schedule(const Key& key)
{
    static_assert(num_rounds == 10);
    static const uint8_t round_constant[10] = {
        0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
    };

    KeySchedule schedule;
    // First four round keys are copied straight from |key|.
    std::memcpy(schedule.data(), key.data(), 4*key_size);

    for (int i = key_size; i < block_size * (num_rounds+1); i++) {
        Word temp = schedule[i-1];
        if (i % key_size == 0) {
            temp = substitute_word(rotate_word(temp));
            temp[0] ^= round_constant[i/key_size - 1];
        }
        // AES-256 would need an extra substitute_word here.
        schedule[i] = schedule[i-key_size] ^ temp;
    }

    return schedule;
}

// See FIPS 197, Fig. 5
Block encrypt_block(
    const Block& block,
    const Key& key,
    std::function<void (const State&, int)>&& test_hook)
{
    State state = block_to_state(block);
    test_hook(state, 0);

    KeySchedule w = compute_key_schedule(key);
    add_round_key(state, w[0], w[1], w[2], w[3]);

    for (int i = 1; i < num_rounds; i++) {
        test_hook(state, i);
        substitute_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state,
                      w[i*block_size],
                      w[i*block_size + 1],
                      w[i*block_size + 2],
                      w[i*block_size + 3]);
    }

    test_hook(state, num_rounds);
    substitute_bytes(state);
    shift_rows(state);
    add_round_key(state,
                  w[num_rounds*block_size],
                  w[num_rounds*block_size + 1],
                  w[num_rounds*block_size + 2],
                  w[num_rounds*block_size + 3]);

    return state_to_block(std::move(state));
}

} // namespace internal

} // namespace dumbaes
