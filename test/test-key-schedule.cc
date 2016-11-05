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

// FIPS 197 Example A.1
static void test_expansion_128()
{
    Key key = {0x2b, 0x7e, 0x15, 0x16,
               0x28, 0xae, 0xd2, 0xa6,
               0xab, 0xf7, 0x15, 0x88,
               0x09, 0xcf, 0x4f, 0x3c};

    internal::KeySchedule schedule = internal::compute_key_schedule(key);

    g_assert_cmphex(schedule[0][0], ==, 0x2b);
    g_assert_cmphex(schedule[0][1], ==, 0x7e);
    g_assert_cmphex(schedule[0][2], ==, 0x15);
    g_assert_cmphex(schedule[0][3], ==, 0x16);

    g_assert_cmphex(schedule[1][0], ==, 0x28);
    g_assert_cmphex(schedule[1][1], ==, 0xae);
    g_assert_cmphex(schedule[1][2], ==, 0xd2);
    g_assert_cmphex(schedule[1][3], ==, 0xa6);

    g_assert_cmphex(schedule[2][0], ==, 0xab);
    g_assert_cmphex(schedule[2][1], ==, 0xf7);
    g_assert_cmphex(schedule[2][2], ==, 0x15);
    g_assert_cmphex(schedule[2][3], ==, 0x88);

    g_assert_cmphex(schedule[3][0], ==, 0x09);
    g_assert_cmphex(schedule[3][1], ==, 0xcf);
    g_assert_cmphex(schedule[3][2], ==, 0x4f);
    g_assert_cmphex(schedule[3][3], ==, 0x3c);

    g_assert_cmphex(schedule[4][0], ==, 0xa0);
    g_assert_cmphex(schedule[4][1], ==, 0xfa);
    g_assert_cmphex(schedule[4][2], ==, 0xfe);
    g_assert_cmphex(schedule[4][3], ==, 0x17);

    g_assert_cmphex(schedule[5][0], ==, 0x88);
    g_assert_cmphex(schedule[5][1], ==, 0x54);
    g_assert_cmphex(schedule[5][2], ==, 0x2c);
    g_assert_cmphex(schedule[5][3], ==, 0xb1);

    g_assert_cmphex(schedule[6][0], ==, 0x23);
    g_assert_cmphex(schedule[6][1], ==, 0xa3);
    g_assert_cmphex(schedule[6][2], ==, 0x39);
    g_assert_cmphex(schedule[6][3], ==, 0x39);

    g_assert_cmphex(schedule[7][0], ==, 0x2a);
    g_assert_cmphex(schedule[7][1], ==, 0x6c);
    g_assert_cmphex(schedule[7][2], ==, 0x76);
    g_assert_cmphex(schedule[7][3], ==, 0x05);

    g_assert_cmphex(schedule[8][0], ==, 0xf2);
    g_assert_cmphex(schedule[8][1], ==, 0xc2);
    g_assert_cmphex(schedule[8][2], ==, 0x95);
    g_assert_cmphex(schedule[8][3], ==, 0xf2);

    g_assert_cmphex(schedule[9][0], ==, 0x7a);
    g_assert_cmphex(schedule[9][1], ==, 0x96);
    g_assert_cmphex(schedule[9][2], ==, 0xb9);
    g_assert_cmphex(schedule[9][3], ==, 0x43);

    g_assert_cmphex(schedule[10][0], ==, 0x59);
    g_assert_cmphex(schedule[10][1], ==, 0x35);
    g_assert_cmphex(schedule[10][2], ==, 0x80);
    g_assert_cmphex(schedule[10][3], ==, 0x7a);

    g_assert_cmphex(schedule[11][0], ==, 0x73);
    g_assert_cmphex(schedule[11][1], ==, 0x59);
    g_assert_cmphex(schedule[11][2], ==, 0xf6);
    g_assert_cmphex(schedule[11][3], ==, 0x7f);

    g_assert_cmphex(schedule[12][0], ==, 0x3d);
    g_assert_cmphex(schedule[12][1], ==, 0x80);
    g_assert_cmphex(schedule[12][2], ==, 0x47);
    g_assert_cmphex(schedule[12][3], ==, 0x7d);

    g_assert_cmphex(schedule[13][0], ==, 0x47);
    g_assert_cmphex(schedule[13][1], ==, 0x16);
    g_assert_cmphex(schedule[13][2], ==, 0xfe);
    g_assert_cmphex(schedule[13][3], ==, 0x3e);

    g_assert_cmphex(schedule[14][0], ==, 0x1e);
    g_assert_cmphex(schedule[14][1], ==, 0x23);
    g_assert_cmphex(schedule[14][2], ==, 0x7e);
    g_assert_cmphex(schedule[14][3], ==, 0x44);

    g_assert_cmphex(schedule[15][0], ==, 0x6d);
    g_assert_cmphex(schedule[15][1], ==, 0x7a);
    g_assert_cmphex(schedule[15][2], ==, 0x88);
    g_assert_cmphex(schedule[15][3], ==, 0x3b);

    g_assert_cmphex(schedule[16][0], ==, 0xef);
    g_assert_cmphex(schedule[16][1], ==, 0x44);
    g_assert_cmphex(schedule[16][2], ==, 0xa5);
    g_assert_cmphex(schedule[16][3], ==, 0x41);

    g_assert_cmphex(schedule[17][0], ==, 0xa8);
    g_assert_cmphex(schedule[17][1], ==, 0x52);
    g_assert_cmphex(schedule[17][2], ==, 0x5b);
    g_assert_cmphex(schedule[17][3], ==, 0x7f);

    g_assert_cmphex(schedule[18][0], ==, 0xb6);
    g_assert_cmphex(schedule[18][1], ==, 0x71);
    g_assert_cmphex(schedule[18][2], ==, 0x25);
    g_assert_cmphex(schedule[18][3], ==, 0x3b);

    g_assert_cmphex(schedule[19][0], ==, 0xdb);
    g_assert_cmphex(schedule[19][1], ==, 0x0b);
    g_assert_cmphex(schedule[19][2], ==, 0xad);
    g_assert_cmphex(schedule[19][3], ==, 0x00);

    g_assert_cmphex(schedule[20][0], ==, 0xd4);
    g_assert_cmphex(schedule[20][1], ==, 0xd1);
    g_assert_cmphex(schedule[20][2], ==, 0xc6);
    g_assert_cmphex(schedule[20][3], ==, 0xf8);

    g_assert_cmphex(schedule[21][0], ==, 0x7c);
    g_assert_cmphex(schedule[21][1], ==, 0x83);
    g_assert_cmphex(schedule[21][2], ==, 0x9d);
    g_assert_cmphex(schedule[21][3], ==, 0x87);

    g_assert_cmphex(schedule[22][0], ==, 0xca);
    g_assert_cmphex(schedule[22][1], ==, 0xf2);
    g_assert_cmphex(schedule[22][2], ==, 0xb8);
    g_assert_cmphex(schedule[22][3], ==, 0xbc);

    g_assert_cmphex(schedule[23][0], ==, 0x11);
    g_assert_cmphex(schedule[23][1], ==, 0xf9);
    g_assert_cmphex(schedule[23][2], ==, 0x15);
    g_assert_cmphex(schedule[23][3], ==, 0xbc);

    g_assert_cmphex(schedule[24][0], ==, 0x6d);
    g_assert_cmphex(schedule[24][1], ==, 0x88);
    g_assert_cmphex(schedule[24][2], ==, 0xa3);
    g_assert_cmphex(schedule[24][3], ==, 0x7a);

    g_assert_cmphex(schedule[25][0], ==, 0x11);
    g_assert_cmphex(schedule[25][1], ==, 0x0b);
    g_assert_cmphex(schedule[25][2], ==, 0x3e);
    g_assert_cmphex(schedule[25][3], ==, 0xfd);

    g_assert_cmphex(schedule[26][0], ==, 0xdb);
    g_assert_cmphex(schedule[26][1], ==, 0xf9);
    g_assert_cmphex(schedule[26][2], ==, 0x86);
    g_assert_cmphex(schedule[26][3], ==, 0x41);

    g_assert_cmphex(schedule[27][0], ==, 0xca);
    g_assert_cmphex(schedule[27][1], ==, 0x00);
    g_assert_cmphex(schedule[27][2], ==, 0x93);
    g_assert_cmphex(schedule[27][3], ==, 0xfd);

    g_assert_cmphex(schedule[28][0], ==, 0x4e);
    g_assert_cmphex(schedule[28][1], ==, 0x54);
    g_assert_cmphex(schedule[28][2], ==, 0xf7);
    g_assert_cmphex(schedule[28][3], ==, 0x0e);

    g_assert_cmphex(schedule[29][0], ==, 0x5f);
    g_assert_cmphex(schedule[29][1], ==, 0x5f);
    g_assert_cmphex(schedule[29][2], ==, 0xc9);
    g_assert_cmphex(schedule[29][3], ==, 0xf3);

    g_assert_cmphex(schedule[30][0], ==, 0x84);
    g_assert_cmphex(schedule[30][1], ==, 0xa6);
    g_assert_cmphex(schedule[30][2], ==, 0x4f);
    g_assert_cmphex(schedule[30][3], ==, 0xb2);

    g_assert_cmphex(schedule[31][0], ==, 0x4e);
    g_assert_cmphex(schedule[31][1], ==, 0xa6);
    g_assert_cmphex(schedule[31][2], ==, 0xdc);
    g_assert_cmphex(schedule[31][3], ==, 0x4f);

    g_assert_cmphex(schedule[32][0], ==, 0xea);
    g_assert_cmphex(schedule[32][1], ==, 0xd2);
    g_assert_cmphex(schedule[32][2], ==, 0x73);
    g_assert_cmphex(schedule[32][3], ==, 0x21);

    g_assert_cmphex(schedule[33][0], ==, 0xb5);
    g_assert_cmphex(schedule[33][1], ==, 0x8d);
    g_assert_cmphex(schedule[33][2], ==, 0xba);
    g_assert_cmphex(schedule[33][3], ==, 0xd2);

    g_assert_cmphex(schedule[34][0], ==, 0x31);
    g_assert_cmphex(schedule[34][1], ==, 0x2b);
    g_assert_cmphex(schedule[34][2], ==, 0xf5);
    g_assert_cmphex(schedule[34][3], ==, 0x60);

    g_assert_cmphex(schedule[35][0], ==, 0x7f);
    g_assert_cmphex(schedule[35][1], ==, 0x8d);
    g_assert_cmphex(schedule[35][2], ==, 0x29);
    g_assert_cmphex(schedule[35][3], ==, 0x2f);

    g_assert_cmphex(schedule[36][0], ==, 0xac);
    g_assert_cmphex(schedule[36][1], ==, 0x77);
    g_assert_cmphex(schedule[36][2], ==, 0x66);
    g_assert_cmphex(schedule[36][3], ==, 0xf3);

    g_assert_cmphex(schedule[37][0], ==, 0x19);
    g_assert_cmphex(schedule[37][1], ==, 0xfa);
    g_assert_cmphex(schedule[37][2], ==, 0xdc);
    g_assert_cmphex(schedule[37][3], ==, 0x21);

    g_assert_cmphex(schedule[38][0], ==, 0x28);
    g_assert_cmphex(schedule[38][1], ==, 0xd1);
    g_assert_cmphex(schedule[38][2], ==, 0x29);
    g_assert_cmphex(schedule[38][3], ==, 0x41);

    g_assert_cmphex(schedule[39][0], ==, 0x57);
    g_assert_cmphex(schedule[39][1], ==, 0x5c);
    g_assert_cmphex(schedule[39][2], ==, 0x00);
    g_assert_cmphex(schedule[39][3], ==, 0x6e);

    g_assert_cmphex(schedule[40][0], ==, 0xd0);
    g_assert_cmphex(schedule[40][1], ==, 0x14);
    g_assert_cmphex(schedule[40][2], ==, 0xf9);
    g_assert_cmphex(schedule[40][3], ==, 0xa8);

    g_assert_cmphex(schedule[41][0], ==, 0xc9);
    g_assert_cmphex(schedule[41][1], ==, 0xee);
    g_assert_cmphex(schedule[41][2], ==, 0x25);
    g_assert_cmphex(schedule[41][3], ==, 0x89);

    g_assert_cmphex(schedule[42][0], ==, 0xe1);
    g_assert_cmphex(schedule[42][1], ==, 0x3f);
    g_assert_cmphex(schedule[42][2], ==, 0x0c);
    g_assert_cmphex(schedule[42][3], ==, 0xc8);

    g_assert_cmphex(schedule[43][0], ==, 0xb6);
    g_assert_cmphex(schedule[43][1], ==, 0x63);
    g_assert_cmphex(schedule[43][2], ==, 0x0c);
    g_assert_cmphex(schedule[43][3], ==, 0xa6);
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    g_test_init(&argc, &argv, nullptr);

    g_test_add_func("/KeySchedule/expansion128", test_expansion_128);

    return g_test_run();
}
