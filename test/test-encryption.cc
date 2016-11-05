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

#include <glib.h>
#include <locale.h>

using namespace dumbaes;

// FIPS 197 Example A.2
static void test_encryption_state()
{
    Block input = {0x32, 0x43, 0xf6, 0xa8,
                   0x88, 0x5a, 0x30, 0x8d,
                   0x31, 0x31, 0x98, 0xa2,
                   0xe0, 0x37, 0x07, 0x34};

    Key key = {0x2b, 0x7e, 0x15, 0x16,
               0x28, 0xae, 0xd2, 0xa6,
               0xab, 0xf7, 0x15, 0x88,
               0x09, 0xcf, 0x4f, 0x3c};

    Block result = internal::encrypt_block(
        input,
        key,
        [](const internal::State& state, int round) {
            switch (round) {
            case 0:
                g_assert_cmpuint(state[0][0], ==, 0x32);
                g_assert_cmpuint(state[0][1], ==, 0x88);
                g_assert_cmpuint(state[0][2], ==, 0x31);
                g_assert_cmpuint(state[0][3], ==, 0xe0);

                g_assert_cmpuint(state[1][0], ==, 0x43);
                g_assert_cmpuint(state[1][1], ==, 0x5a);
                g_assert_cmpuint(state[1][2], ==, 0x31);
                g_assert_cmpuint(state[1][3], ==, 0x37);

                g_assert_cmpuint(state[2][0], ==, 0xf6);
                g_assert_cmpuint(state[2][1], ==, 0x30);
                g_assert_cmpuint(state[2][2], ==, 0x98);
                g_assert_cmpuint(state[2][3], ==, 0x07);

                g_assert_cmpuint(state[3][0], ==, 0xa8);
                g_assert_cmpuint(state[3][1], ==, 0x8d);
                g_assert_cmpuint(state[3][2], ==, 0xa2);
                g_assert_cmpuint(state[3][3], ==, 0x34);
                break;
            case 1:
                g_assert_cmpuint(state[0][0], ==, 0x19);
                g_assert_cmpuint(state[0][1], ==, 0xa0);
                g_assert_cmpuint(state[0][2], ==, 0x9a);
                g_assert_cmpuint(state[0][3], ==, 0xe9);

                g_assert_cmpuint(state[1][0], ==, 0x3d);
                g_assert_cmpuint(state[1][1], ==, 0xf4);
                g_assert_cmpuint(state[1][2], ==, 0xc6);
                g_assert_cmpuint(state[1][3], ==, 0xf8);

                g_assert_cmpuint(state[2][0], ==, 0xe3);
                g_assert_cmpuint(state[2][1], ==, 0xe2);
                g_assert_cmpuint(state[2][2], ==, 0x8d);
                g_assert_cmpuint(state[2][3], ==, 0x48);

                g_assert_cmpuint(state[3][0], ==, 0xbe);
                g_assert_cmpuint(state[3][1], ==, 0x2b);
                g_assert_cmpuint(state[3][2], ==, 0x2a);
                g_assert_cmpuint(state[3][3], ==, 0x08);
                break;
            case 2:
                g_assert_cmpuint(state[0][0], ==, 0xa4);
                g_assert_cmpuint(state[0][1], ==, 0x68);
                g_assert_cmpuint(state[0][2], ==, 0x6b);
                g_assert_cmpuint(state[0][3], ==, 0x02);

                g_assert_cmpuint(state[1][0], ==, 0x9c);
                g_assert_cmpuint(state[1][1], ==, 0x9f);
                g_assert_cmpuint(state[1][2], ==, 0x5b);
                g_assert_cmpuint(state[1][3], ==, 0x6a);

                g_assert_cmpuint(state[2][0], ==, 0x7f);
                g_assert_cmpuint(state[2][1], ==, 0x35);
                g_assert_cmpuint(state[2][2], ==, 0xea);
                g_assert_cmpuint(state[2][3], ==, 0x50);

                g_assert_cmpuint(state[3][0], ==, 0xf2);
                g_assert_cmpuint(state[3][1], ==, 0x2b);
                g_assert_cmpuint(state[3][2], ==, 0x43);
                g_assert_cmpuint(state[3][3], ==, 0x49);
                break;
            case 3:
                g_assert_cmpuint(state[0][0], ==, 0xaa);
                g_assert_cmpuint(state[0][1], ==, 0x61);
                g_assert_cmpuint(state[0][2], ==, 0x82);
                g_assert_cmpuint(state[0][3], ==, 0x68);

                g_assert_cmpuint(state[1][0], ==, 0x8f);
                g_assert_cmpuint(state[1][1], ==, 0xdd);
                g_assert_cmpuint(state[1][2], ==, 0xd2);
                g_assert_cmpuint(state[1][3], ==, 0x32);

                g_assert_cmpuint(state[2][0], ==, 0x5f);
                g_assert_cmpuint(state[2][1], ==, 0xe3);
                g_assert_cmpuint(state[2][2], ==, 0x4a);
                g_assert_cmpuint(state[2][3], ==, 0x46);

                g_assert_cmpuint(state[3][0], ==, 0x03);
                g_assert_cmpuint(state[3][1], ==, 0xef);
                g_assert_cmpuint(state[3][2], ==, 0xd2);
                g_assert_cmpuint(state[3][3], ==, 0x9a);
                break;
            case 4:
                g_assert_cmpuint(state[0][0], ==, 0x48);
                g_assert_cmpuint(state[0][1], ==, 0x67);
                g_assert_cmpuint(state[0][2], ==, 0x4d);
                g_assert_cmpuint(state[0][3], ==, 0xd6);

                g_assert_cmpuint(state[1][0], ==, 0x6c);
                g_assert_cmpuint(state[1][1], ==, 0x1d);
                g_assert_cmpuint(state[1][2], ==, 0xe3);
                g_assert_cmpuint(state[1][3], ==, 0x5f);

                g_assert_cmpuint(state[2][0], ==, 0x4e);
                g_assert_cmpuint(state[2][1], ==, 0x9d);
                g_assert_cmpuint(state[2][2], ==, 0xb1);
                g_assert_cmpuint(state[2][3], ==, 0x58);

                g_assert_cmpuint(state[3][0], ==, 0xee);
                g_assert_cmpuint(state[3][1], ==, 0x0d);
                g_assert_cmpuint(state[3][2], ==, 0x38);
                g_assert_cmpuint(state[3][3], ==, 0xe7);
                break;
            case 5:
                g_assert_cmpuint(state[0][0], ==, 0xe0);
                g_assert_cmpuint(state[0][1], ==, 0xc8);
                g_assert_cmpuint(state[0][2], ==, 0xd9);
                g_assert_cmpuint(state[0][3], ==, 0x85);

                g_assert_cmpuint(state[1][0], ==, 0x92);
                g_assert_cmpuint(state[1][1], ==, 0x63);
                g_assert_cmpuint(state[1][2], ==, 0xb1);
                g_assert_cmpuint(state[1][3], ==, 0xb8);

                g_assert_cmpuint(state[2][0], ==, 0x7f);
                g_assert_cmpuint(state[2][1], ==, 0x63);
                g_assert_cmpuint(state[2][2], ==, 0x35);
                g_assert_cmpuint(state[2][3], ==, 0xbe);

                g_assert_cmpuint(state[3][0], ==, 0xe8);
                g_assert_cmpuint(state[3][1], ==, 0xc0);
                g_assert_cmpuint(state[3][2], ==, 0x50);
                g_assert_cmpuint(state[3][3], ==, 0x01);
                break;
            case 6:
                g_assert_cmpuint(state[0][0], ==, 0xf1);
                g_assert_cmpuint(state[0][1], ==, 0xc1);
                g_assert_cmpuint(state[0][2], ==, 0x7c);
                g_assert_cmpuint(state[0][3], ==, 0x5d);

                g_assert_cmpuint(state[1][0], ==, 0x00);
                g_assert_cmpuint(state[1][1], ==, 0x92);
                g_assert_cmpuint(state[1][2], ==, 0xc8);
                g_assert_cmpuint(state[1][3], ==, 0xb5);

                g_assert_cmpuint(state[2][0], ==, 0x6f);
                g_assert_cmpuint(state[2][1], ==, 0x4c);
                g_assert_cmpuint(state[2][2], ==, 0x8b);
                g_assert_cmpuint(state[2][3], ==, 0xd5);

                g_assert_cmpuint(state[3][0], ==, 0x55);
                g_assert_cmpuint(state[3][1], ==, 0xef);
                g_assert_cmpuint(state[3][2], ==, 0x32);
                g_assert_cmpuint(state[3][3], ==, 0x0c);
                break;
            case 7:
                g_assert_cmpuint(state[0][0], ==, 0x26);
                g_assert_cmpuint(state[0][1], ==, 0x3d);
                g_assert_cmpuint(state[0][2], ==, 0xe8);
                g_assert_cmpuint(state[0][3], ==, 0xfd);

                g_assert_cmpuint(state[1][0], ==, 0x0e);
                g_assert_cmpuint(state[1][1], ==, 0x41);
                g_assert_cmpuint(state[1][2], ==, 0x64);
                g_assert_cmpuint(state[1][3], ==, 0xd2);

                g_assert_cmpuint(state[2][0], ==, 0x2e);
                g_assert_cmpuint(state[2][1], ==, 0xb7);
                g_assert_cmpuint(state[2][2], ==, 0x72);
                g_assert_cmpuint(state[2][3], ==, 0x8b);

                g_assert_cmpuint(state[3][0], ==, 0x17);
                g_assert_cmpuint(state[3][1], ==, 0x7d);
                g_assert_cmpuint(state[3][2], ==, 0xa9);
                g_assert_cmpuint(state[3][3], ==, 0x25);
                break;
            case 8:
                g_assert_cmpuint(state[0][0], ==, 0x5a);
                g_assert_cmpuint(state[0][1], ==, 0x19);
                g_assert_cmpuint(state[0][2], ==, 0xa3);
                g_assert_cmpuint(state[0][3], ==, 0x7a);

                g_assert_cmpuint(state[1][0], ==, 0x41);
                g_assert_cmpuint(state[1][1], ==, 0x49);
                g_assert_cmpuint(state[1][2], ==, 0xe0);
                g_assert_cmpuint(state[1][3], ==, 0x8c);

                g_assert_cmpuint(state[2][0], ==, 0x42);
                g_assert_cmpuint(state[2][1], ==, 0xdc);
                g_assert_cmpuint(state[2][2], ==, 0x19);
                g_assert_cmpuint(state[2][3], ==, 0x04);

                g_assert_cmpuint(state[3][0], ==, 0xb1);
                g_assert_cmpuint(state[3][1], ==, 0x1f);
                g_assert_cmpuint(state[3][2], ==, 0x65);
                g_assert_cmpuint(state[3][3], ==, 0x0c);
                break;
            case 9:
                g_assert_cmpuint(state[0][0], ==, 0xea);
                g_assert_cmpuint(state[0][1], ==, 0x04);
                g_assert_cmpuint(state[0][2], ==, 0x65);
                g_assert_cmpuint(state[0][3], ==, 0x85);

                g_assert_cmpuint(state[1][0], ==, 0x83);
                g_assert_cmpuint(state[1][1], ==, 0x45);
                g_assert_cmpuint(state[1][2], ==, 0x5d);
                g_assert_cmpuint(state[1][3], ==, 0x96);

                g_assert_cmpuint(state[2][0], ==, 0x5c);
                g_assert_cmpuint(state[2][1], ==, 0x33);
                g_assert_cmpuint(state[2][2], ==, 0x98);
                g_assert_cmpuint(state[2][3], ==, 0xb0);

                g_assert_cmpuint(state[3][0], ==, 0xf0);
                g_assert_cmpuint(state[3][1], ==, 0x2d);
                g_assert_cmpuint(state[3][2], ==, 0xad);
                g_assert_cmpuint(state[3][3], ==, 0xc5);
                break;
            case 10:
                g_assert_cmpuint(state[0][0], ==, 0xeb);
                g_assert_cmpuint(state[0][1], ==, 0x59);
                g_assert_cmpuint(state[0][2], ==, 0x8b);
                g_assert_cmpuint(state[0][3], ==, 0x1b);

                g_assert_cmpuint(state[1][0], ==, 0x40);
                g_assert_cmpuint(state[1][1], ==, 0x2e);
                g_assert_cmpuint(state[1][2], ==, 0xa1);
                g_assert_cmpuint(state[1][3], ==, 0xc3);

                g_assert_cmpuint(state[2][0], ==, 0xf2);
                g_assert_cmpuint(state[2][1], ==, 0x38);
                g_assert_cmpuint(state[2][2], ==, 0x13);
                g_assert_cmpuint(state[2][3], ==, 0x42);

                g_assert_cmpuint(state[3][0], ==, 0x1e);
                g_assert_cmpuint(state[3][1], ==, 0x84);
                g_assert_cmpuint(state[3][2], ==, 0xe7);
                g_assert_cmpuint(state[3][3], ==, 0xd2);
                break;
            default:
                g_assert_not_reached();
            }
        });

    g_assert_cmpuint(result[0], ==, 0x39);
    g_assert_cmpuint(result[1], ==, 0x25);
    g_assert_cmpuint(result[2], ==, 0x84);
    g_assert_cmpuint(result[3], ==, 0x1d);

    g_assert_cmpuint(result[4], ==, 0x02);
    g_assert_cmpuint(result[5], ==, 0xdc);
    g_assert_cmpuint(result[6], ==, 0x09);
    g_assert_cmpuint(result[7], ==, 0xfb);

    g_assert_cmpuint(result[8], ==, 0xdc);
    g_assert_cmpuint(result[9], ==, 0x11);
    g_assert_cmpuint(result[10], ==, 0x85);
    g_assert_cmpuint(result[11], ==, 0x97);

    g_assert_cmpuint(result[12], ==, 0x19);
    g_assert_cmpuint(result[13], ==, 0x6a);
    g_assert_cmpuint(result[14], ==, 0x0b);
    g_assert_cmpuint(result[15], ==, 0x32);
}

static void test_encryption_decrypt()
{
    // FIXME: This test should be run many times for random inputs and keys.
    Block input = {0x32, 0x43, 0xf6, 0xa8,
                   0x88, 0x5a, 0x30, 0x8d,
                   0x31, 0x31, 0x98, 0xa2,
                   0xe0, 0x37, 0x07, 0x34};

    Key key = {0x2b, 0x7e, 0x15, 0x16,
               0x28, 0xae, 0xd2, 0xa6,
               0xab, 0xf7, 0x15, 0x88,
               0x09, 0xcf, 0x4f, 0x3c};

    Block ciphertext = encrypt_block(input, key);
    Block plaintext = decrypt_block(ciphertext, key);

    for (int i = 0; i < 16; i++)
        g_assert_cmpuint(input[i], ==, plaintext[i]);
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    g_test_init(&argc, &argv, nullptr);

    g_test_add_func("/Encryption/state", test_encryption_state);
    g_test_add_func("/Encryption/decrypt", test_encryption_decrypt);

    return g_test_run();
}
